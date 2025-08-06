#pragma once

#include "game.h"
#include "statistics.h"
#include "player.h"
#include "../engine/iengine.h"
#include "../engine/r_engine.h"
#include "../engine/bf_engine.h"
#include "../ui/userInterface.h"

using namespace std;


class GameRunner {
public:

	Game* game;
	Player* whitePlayer;
	Player* blackPlayer;
	Statistics* statistics;
	string fenBoard;
	ArrayPool<Move>* pool;
	string humanMove = "";
	mutex moveMtx;
	Logger& logger = Logger::getInstance();
	bool running = false;

	GameRunner(Statistics* statistics, GameType gameType = HvsC, const string& fenBoard = "") {
		pool = new ArrayPool<Move>(1);
		game = new Game();

		whitePlayer = gameType == CvsC ?
			new Player(true, new BF_Engine(2)) :
			new Player(true);

		blackPlayer = new Player(false, new BF_Engine(3));

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
		running = true;

		do {
			if (game->getCurrentPlayer()->computer) {
				endGame = processComputerMove();
			} else {
				if (statistics->consoleOutput) {
					UI::printGame(*game);
				}
				endGame = processHumanMove();
			}
		} while (running && !isGameEnded(endGame));
	}

	EndGameType processComputerMove() {
		game->getCurrentPlayer()->startMoveTime();
		Evaluation evaluation = game->calculateMove();

		if (evaluation.endGameType == NONE) {
			logger.log(format("{}: {}", game->getCurrentPlayer()->name, MoveHelper::toString(evaluation.move)));
			game->applyMove(evaluation.move);
			game->currentEvaluation = evaluation.value;
		}

		game->getCurrentPlayer()->stopMoveTime();

		return evaluation.endGameType;
	}

	EndGameType processHumanMove() {
		game->verifyChecks();
		Move* moves = pool->getArray();
		MovesAmount amount = MovesGenerator::generateLegalMoves(*game, moves);
		EndGameType endGame = game->checkEndGame(amount.second);

		if (endGame != NONE) {
			game->getCurrentPlayer()->stopMoveTime();
			return endGame;
		}

		Move move;

		while (running) {
			game->getCurrentPlayer()->startMoveTime();
			string humanMove = waitForHumanMove();
			move = MoveHelper::getMove(humanMove, game->getCurrentPlayer()->white);
			MoveHelper::decorate(move, game->board.getPiece(MoveHelper::getSourcePosition(move)), game->board.enPassantPosition, game->isComputerToMove());

			if (MoveHelper::isPawnPromotion(move)) {
				MoveHelper::setPromotion(move, UI::choosePromotionType(game->isWhiteToMove()));
			}

			game->getCurrentPlayer()->stopMoveTime();

			if (MoveHelper::isPresent(move, moves, amount.first)) {
				break;
			}

			UI::printMessage(" invalid move");
		}

		logger.log(format("{}: {}", game->getCurrentPlayer()->name, MoveHelper::toString(move)));
		game->applyMove(move);
		game->currentEvaluation = game->evaluator->evaluate(*game);

		return NONE;
	}

	string waitForHumanMove() {
		while (running) {
			std::this_thread::sleep_for(100ms);
			moveMtx.lock();
			if (!humanMove.empty()) {
				string move = humanMove;
				humanMove = "";
				moveMtx.unlock();
				return move;
			}
			moveMtx.unlock();
		}
		return nullptr;
	}

	void setHumanMove(const string& move) {
		moveMtx.lock();
		humanMove = move;
		moveMtx.unlock();
	}

	bool isGameEnded(EndGameType endGame) {
		if (endGame != NONE) {
			if (endGame == CHECKMATE) {
				statistics->gameEnded(game->isWhiteToMove() ? -1 : 1);
				logger.log(format("Game ended: {} WINS ({} moves in {} ms)",
					game->isWhiteToMove() ? "black" : "white", game->fullMoves, game->whitePlayer->gameTime + game->blackPlayer->gameTime));
				logger.log(game->printMovesHistory(0));
				logger.log(FEN::gameToFEN(*game));
			} else {
				statistics->gameEnded(0);
				logger.log(format("Game ended: DRAW{} ({} moves in {} ms)",
					endGame == STALEMATE ? " (stalemate)" : "", game->fullMoves, game->whitePlayer->gameTime + game->blackPlayer->gameTime));
			}

			if (statistics->consoleOutput) {
				UI::printGame(*game);
				if (endGame == CHECKMATE) {
					cout << (game->isWhiteToMove() ? " black" : " white") << " wins" << endl << endl;
				} else {
					cout << " draw" << (endGame == STALEMATE ? " (stalemate)" : "") << endl << endl;
				}
			}

			return true;
		}

		return false;
	}

	void stop() {
		running = false;
		delete game;
		delete pool;
		game = nullptr;
		pool = nullptr;
	}
};