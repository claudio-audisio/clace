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
	VectorPool<Move>* pool;
	string humanMove = "";
	mutex moveMtx;
	Logger& logger = Logger::getInstance();
	bool running = false;

	GameRunner(Statistics* statistics, GameType gameType = HvsC, const string& fenBoard = "") {
		pool = new VectorPool<Move>(1, MAX_MOVES);
		game = new Game();

		whitePlayer = gameType == CvsC ?
			new Player(true, new BF_Engine(1)) :
			new Player(true);

		blackPlayer = new Player(false, new BF_Engine(2));

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
		vector<Move>& moves = pool->getVector(0);
		MovesGenerator::generateLegalMoves(*game, moves);
		EndGameType endGame = game->checkEndGame(moves.empty());

		if (endGame != EndGameType::NONE) {
			game->getCurrentPlayer()->stopMoveTime();
			return endGame;
		}

		Evaluation evaluation = game->getCurrentPlayer()->engine->calculateMove(*game, moves);
		game->getCurrentPlayer()->stopMoveTime();
		logger.log(format("{}: {}", game->getCurrentPlayer()->name, MoveHelper::toString(evaluation.first)));
		game->applyMove(evaluation.first);
		game->currentEvaluation = evaluation.second;

		if (game->checkFiveFoldRepetitions()) {
			game->getCurrentPlayer()->stopMoveTime();
			return EndGameType::FIVEFOLD_REPETITION;
		}

		game->verifyChecks();

		return EndGameType::NONE;
	}

	EndGameType processHumanMove() {
		vector<Move>& moves = pool->getVector(0);
		MovesGenerator::generateLegalMoves(*game, moves);
		EndGameType endGame = game->checkEndGame(moves.empty());

		if (endGame != EndGameType::NONE) {
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

			if (MoveHelper::isPresent(move, moves)) {
				break;
			}

			UI::printMessage(" invalid move");
		}

		logger.log(format("{}: {}", game->getCurrentPlayer()->name, MoveHelper::toString(move)));
		game->applyMove(move);
		game->currentEvaluation = game->evaluator->evaluate(*game);

		if (game->checkFiveFoldRepetitions()) {
			game->getCurrentPlayer()->stopMoveTime();
			return EndGameType::FIVEFOLD_REPETITION;
		}

		game->verifyChecks();

		return EndGameType::NONE;
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
		if (endGame != EndGameType::NONE) {
			if (endGame == EndGameType::CHECKMATE) {
				statistics->gameEnded(game->isWhiteToMove() ? -1 : 1);
				logger.log(format("Game ended: {} WINS ({} moves in {} ms)",
					game->isWhiteToMove() ? "black" : "white", game->fullMoves, game->whitePlayer->gameTime + game->blackPlayer->gameTime));
				logger.log(game->printMovesHistory(0));
				logger.log(FEN::gameToFEN(*game));
			} else {
				statistics->gameEnded(0);
				logger.log(format("Game ended: DRAW{} ({} moves in {} ms)",
					endGame == EndGameType::STALEMATE ? " (stalemate)" : "", game->fullMoves, game->whitePlayer->gameTime + game->blackPlayer->gameTime));
			}

			if (statistics->consoleOutput) {
				UI::printGame(*game);
				if (endGame == EndGameType::CHECKMATE) {
					cout << (game->isWhiteToMove() ? " black" : " white") << " wins" << endl << endl;
				} else {
					cout << " draw" << (endGame == EndGameType::STALEMATE ? " (stalemate)" : "") << endl << endl;
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