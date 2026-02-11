#pragma once

#include "abstractEngine.h"


class AlphaBetaEngine final : public AbstractEngine {
public:
	explicit AlphaBetaEngine(const unsigned int depth) :
		AbstractEngine(depth, "AlphaBetaEngine") {
		evaluator = new BasicEvaluator();
	}

	Evaluation _calculateMove(Game& game) override {
		double alpha = LOSS_VALUE * 2;	// punteggio minimo di cui il bianco e' sicuro
		double beta = WIN_VALUE * 2;	// punteggio massimo di cui il nero e' sicuro

		return alphaBeta(game, alpha, beta, depth);
	};

	Evaluation alphaBeta(Game& game, double alpha, double beta, const unsigned int depth) {
		// TODO le mosse dovrebbero essere ordinate
		// https://www.chessprogramming.org/Move_Ordering
		// Ecco l'ordine di valutazione delle mosse che dovrebbe essere adottato

		game.verifyChecks();
		Move *moves;
		MovesAmount amount;
		table->getMoves(game, moves, amount);
		const EndGameType endGame = game.checkEndGame(amount.legal);

		if (endGame != NONE) {
			const Evaluation eval = evaluator->evaluateEndGame(game, this->depth - depth, endGame);
			/*if (moveToString(game.movesHistory[0]) == "c3c2" && endGame == CHECKMATE) {
				messenger.send(MSG_LOG, description, format("{} --> {:.2f}", game.printMovesHistory(eval.depth), eval.value));
			}*/
			return eval;
		}

		if (depth == 0) {
			// TODO dovremmo fare un ricerca quiescente
			// https://www.chessprogramming.org/Quiescence_Search
			const Evaluation eval = evaluator->evaluate(game, this->depth);
			/*if (moveToString(game.movesHistory[0]) == "c3c2") {
				messenger.send(MSG_LOG, description, format("{} --> {:.2f}", game.printMovesHistory(eval.depth), eval.value));
			}*/
			return eval;
		}

		Evaluation best = {0, LOSS_VALUE * 2, NONE, MAX_DEPTH, nullptr};

		for (unsigned int i = 0; i < amount.total; i++) {
			if (moves[i]) {
				game.save();
				game.applyMove(moves[i]);

				Evaluation eval = alphaBeta(game, -beta, -alpha, depth - 1);
				Evaluator::changeSign(eval);
				eval.pvMoves[this->depth - depth] = moves[i];

				/*if (depth == this->depth) {
					messenger.send(MSG_LOG, description, evalToString(eval));
				}*/

				game.rollbackLastMove();

				if (Evaluator::isBetter(eval, best)) {
					/*if (moveToString(game.movesHistory[0]) == "c3c2" && depth == 3) {
						messenger.send(MSG_LOG, description, format("side to move {}", sideToString(game.sideToMove)));
						messenger.send(MSG_LOG, description, format("old best {}", evalToString(best)));
						messenger.send(MSG_LOG, description, format("new best {}", evalToString(eval)));
					}*/
					delete best.pvMoves;
					best = eval;
					best.move = moves[i];

					if (eval.value > alpha) {
						alpha = eval.value;
					}
				} else {
					delete eval.pvMoves;
				}

				if (eval.value >= beta) {
					/*if (moveToString(game.movesHistory[0]) == "c3c2") {
						messenger.send(MSG_LOG, description, format("{} --> {:.2f} - cutoff", game.printMovesHistory(best.depth), best.value));
					}*/
					return best;
				}
			}
		}

		/*if (moveToString(game.movesHistory[0]) == "c3c2" && depth == 3) {
			messenger.send(MSG_LOG, description, format("best {}", evalToString(best)));
		}*/
		return best;
	}

};