#pragma once

#include <thread>

#include "game.h"
#include "statistics.h"
#include "player.h"
#include "../engine/iengine.h"
#include "../engine/randomEngine.h"
#include "../engine/bruteForceEngine.h"
#include "../ui/userInterface.h"
#include "../ui/gui/igui.h"
#include "engine/alphaBetaEngine.h"

using namespace std;


class GameRunner {
public:
	Game* game;
	Player* whitePlayer;
	Player* blackPlayer;
	Statistics* statistics;
	string fenBoard;
	ArrayPool<Move> *pool;
	string humanMove = "";
	mutex moveMtx;
	Messenger& messenger = Messenger::getInstance();
	bool running = false;
	IGui *gui = nullptr;

	GameRunner(Statistics* statistics, GameType gameType = HvsC, const string& fenBoard = "") {
		pool = new ArrayPool<Move>(1, MAX_MOVES, 1);
		game = new Game();

		whitePlayer = gameType == CvsC ?
			new Player(_WHITE, new AlphaBetaEngine(4)) :
			new Player(_WHITE);

		blackPlayer = new Player(_BLACK, new BruteForceEngine(3));

		this->statistics = statistics;
		this->fenBoard = fenBoard;
	}

	void run(IGui *gui = new IGui) {
		this->gui = gui;

		if (fenBoard.empty()) {
			game->init();
		} else {
			game->initFromFEN(fenBoard);
		}

		game->initPlayers(whitePlayer, blackPlayer);
		messenger.send(MSG_ALL, "gameRunner", format("new game: {}", game->getDescription()));
		Evaluation eval;
		running = true;
		gui->onBoardChange();

		do {
			//messenger.send(MSG_LOG, "fen: ", FEN::gameToFEN(*game));

			if (game->getCurrentPlayer()->computer) {
				eval = processComputerMove();
				gui->onBoardChange(true);
			} else {
				if (statistics->consoleOutput) {
					UI::printGame(*game);
				}
				eval = processHumanMove();
				gui->onBoardChange();
			}
		} while (running && !isGameEnded(eval));
	}

	Evaluation processComputerMove() const {
		game->getCurrentPlayer()->startMoveTime();
		Evaluation evaluation = game->calculateMove();
		game->getCurrentPlayer()->stopMoveTime();

		if (evaluation.move) {
			messenger.send(MSG_LOG, "gameRunner", format("{}: {}", game->getCurrentPlayer()->name, moveToString(evaluation.move)));
			game->applyMove(evaluation.move);
			game->currentEvaluation = evaluation;
		}

		return evaluation;
	}

	Evaluation processHumanMove() {
		game->verifyChecks();
		gui->setGameInfo(game->sideToMove,
				game->checkStatus.check && !game->checkStatus.checkmate,
				game->fullMoves,
				game->currentEvaluation.value,
				FEN::gameToFEN(*game),
				moveToString(game->lastMove),
				game->whitePlayer->getMoveTime(),
				game->blackPlayer->getMoveTime());

		Move* moves = pool->getArray();
		MovesAmount amount;
		generateLegalMoves(*game, moves, &amount);
		EndGameType endGame = game->checkEndGame(amount.legal);

		if (endGame != NONE) {
			game->getCurrentPlayer()->stopMoveTime();
			return {0, 0, endGame, 0};
		}

		Move move;

		while (running) {
			game->getCurrentPlayer()->startMoveTime();
			string newMove = waitForHumanMove();
			move = createMove(newMove, game->getCurrentPlayer()->side);
			decorate(move, getPiece(game->board, getSourcePosition(move)), game->board->enPassantPosition);

			if (isPawnPromotion(move)) {
				Piece promotion = gui->choosePromotionType();

				if (!promotion) {
					promotion = UI::choosePromotionType(game->isWhiteToMove());
				}

				setPromotion(move, promotion);
				messenger.send(MSG_ALL, "gameRunner", "pawn promoted to " + pieceToString(promotion));
			}

			game->getCurrentPlayer()->stopMoveTime();

			if (isPresent(move, moves, amount.total)) {
				break;
			}

			gui->onBoardChange();
			gui->showMessage(" invalid move");
			UI::printMessage(" invalid move");
		}

		messenger.send(MSG_LOG, "gameRunner", format("{}: {}", game->getCurrentPlayer()->name, moveToString(move)));
		game->applyMove(move);
		game->currentEvaluation = game->evaluator->evaluate(*game, 0);

		pool->release(moves);

		return game->currentEvaluation;
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

	bool isGameEnded(const Evaluation& evaluation) {
		if (evaluation.endGameType != NONE && !evaluation.move) {
			if (evaluation.endGameType == CHECKMATE) {
				statistics->gameEnded(game->isWhiteToMove() ? -1 : 1);
				messenger.send(MSG_ALL, "gameRunner", format("Game ended: {} WINS ({} moves in {} ms)",
					game->isWhiteToMove() ? "black" : "white", game->fullMoves, game->whitePlayer->gameTime + game->blackPlayer->gameTime));
				messenger.send(MSG_LOG, "gameRunner", game->printMovesHistory(0));
				messenger.send(MSG_LOG, "gameRunner", FEN::gameToFEN(*game));
			} else {
				statistics->gameEnded(0);
				messenger.send(MSG_ALL, "gameRunner", format("Game ended: DRAW{} ({} moves in {} ms)",
					evaluation.endGameType == STALEMATE ? " (stalemate)" : "", game->fullMoves, game->whitePlayer->gameTime + game->blackPlayer->gameTime));
			}

			if (statistics->consoleOutput) {
				UI::printGame(*game);
				if (evaluation.endGameType == CHECKMATE) {
					cout << (game->isWhiteToMove() ? " black" : " white") << " wins" << endl << endl;
				} else {
					cout << " draw" << (evaluation.endGameType == STALEMATE ? " (stalemate)" : "") << endl << endl;
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