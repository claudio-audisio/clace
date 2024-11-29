#pragma once

#include "game.h"
#include "statistics.h"
#include "player.h"
#include "../engine/iengine.h"
#include "../engine/r_engine.h"
#include "../ui/userInterface.h"

using namespace std;


class GameRunner {
public:

	Game* game;
	Player* whitePlayer;
	Player* blackPlayer;
	Statistics* statistics;
	string fenBoard;
	VectorPool<Move>* pool;

	GameRunner(const string& whiteName, const string& blackName, Statistics* statistics, const string& fenBoard) {
		pool = this->pool = new VectorPool<Move>(1, MAX_MOVES);
		game = new Game();

		if (whiteName.empty()) {
			IEngine* whiteEngine = new R_Engine();
			whitePlayer = new Player("H725", true,whiteEngine);
		} else {
			whitePlayer = new Player(whiteName, true);
		}

		if (blackName.empty()) {
			IEngine* blackEngine = new R_Engine();
			blackPlayer = new Player("HAL9000", true, blackEngine);
		} else {
			blackPlayer = new Player(blackName, false);
		}

		this->statistics = statistics;
		this->fenBoard = fenBoard;
	}

	void run() {
		if (fenBoard.empty()) {
			game->init();
		} else {
			game->initFromFEN(fenBoard);
		}

		game->initPlayers(whitePlayer, blackPlayer);
		EndGameType endGame;

		do {
			if (statistics->consoleOutput) {
				UI::clearScreen();
				UI::printBoard(game->board);
				UI::printData(*game);
			}

			if (game->getCurrentPlayer()->computer) {
				endGame = processComputerMove();
			} else {
				endGame = processHumanMove();
			}
		} while (!isGameEnded(endGame));
	}

	EndGameType processComputerMove() {
		game->getCurrentPlayer()->startMoveTime();
		game->verifyChecks();
		vector<Move>& moves = pool->getVector(0);
		MovesGenerator::generateLegalMoves(*game, moves);
		EndGameType endGame = game->checkEndGame(moves.empty());

		if (endGame != EndGameType::NONE) {
			game->getCurrentPlayer()->stopMoveTime();
			return endGame;
		}

		Move move = game->getCurrentPlayer()->engine->calculateMove(*game, moves);
		game->getCurrentPlayer()->stopMoveTime();
		game->applyMove(move);

		if (game->checkFiveFoldRepetitions()) {
			game->getCurrentPlayer()->stopMoveTime();
			return EndGameType::FIVEFOLD_REPETITION;
		}

		return EndGameType::NONE;
	}

	EndGameType processHumanMove() {
		game->verifyChecks();
		vector<Move>& moves = pool->getVector(0);
		MovesGenerator::generateLegalMoves(*game, moves);
		EndGameType endGame = game->checkEndGame(moves.empty());

		if (endGame != EndGameType::NONE) {
			game->getCurrentPlayer()->stopMoveTime();
			return endGame;
		}

		Move move;

		while (true) {
			game->getCurrentPlayer()->startMoveTime();
			move = UI::getNextMove(game->getCurrentPlayer());
			MoveHelper::decorate(move, game->board.getPiece(MoveHelper::getSourcePosition(move)), game->board.enPassantPosition, game->isComputerToMove());

			if (MoveHelper::isPawnPromotion(move)) {
				MoveHelper::setPromotion(move, UI::choosePromotionType(game->isWhiteToMove()));
			}

			game->getCurrentPlayer()->stopMoveTime();

			if (MoveHelper::isPresent(move, moves)) {
				break;
			}

			cout << "  --> Invalid move!" << endl;
		}

		game->applyMove(move);

		if (game->checkFiveFoldRepetitions()) {
			game->getCurrentPlayer()->stopMoveTime();
			return EndGameType::FIVEFOLD_REPETITION;
		}

		return EndGameType::NONE;
	}

	bool isGameEnded(EndGameType endGame) {
		if (endGame != EndGameType::NONE) {
			if (statistics->consoleOutput) {
				UI::printBoard(game->board);

				if (endGame == EndGameType::CHECKMATE) {
					cout << "  --> Game ended: " << (game->isWhiteToMove() ? game->blackPlayer->name : game->whitePlayer->name) << " WINS" << endl;
				} else {
					cout << "  --> Game ended: DRAW" << (endGame == EndGameType::STALEMATE ? " (stalemate)" : "") << endl;
				}
			} else {
				// TODO introduce logging
				/*if (EndGameType.CHECKMATE.equals(endGame)) {
					log.info("Game ended: {} WINS ({} moves in {} msec)", board.isWhiteToMove() ? board.getBlackPlayer().getName() : board.getWhitePlayer().getName(), board.getFullMoves(), board.getWhitePlayer().getGameTime() + board.getBlackPlayer().getGameTime());
					log.info(board.getMovesHistory().toString());
					log.info(FENConverter.boardToFEN(board));
					log.info("\n");
				} else {
					log.info("Game ended: DRAW{} ({} moves in {} msec)", EndGameType.STALEMATE.equals(endGame) ? " (stalemate)" : "", board.getFullMoves(), board.getWhitePlayer().getGameTime() + board.getBlackPlayer().getGameTime());
				}*/
			}

			if (endGame == EndGameType::CHECKMATE) {
				statistics->gameEnded(game->isWhiteToMove() ? -1 : 1);
			} else {
				statistics->gameEnded(0);
			}

			return true;
		}

		return false;
	}

};