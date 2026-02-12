#pragma once

#include "abstractEngine.h"


class BruteForceEngine : public AbstractEngine {
public:
	explicit BruteForceEngine(const unsigned int depth):
		AbstractEngine(depth, "BruteForceEngine") {
		evaluator = new BasicEvaluator();
	}

	Evaluation _calculateMove(Game& game) override {
		return negaMax(game, depth);
	}

	Evaluation negaMax(Game& game, const unsigned int depth) {
		game.verifyChecks();
		Move* moves;
		const unsigned int amount = table->getMoves(game, moves);
		const EndGameType endGame = game.checkEndGame(amount);

		if (endGame != NONE) {
			const Evaluation eval = evaluator->evaluateEndGame(game, this->depth - depth, endGame);
			/*if (moveToString(game.movesHistory[0]) == "c3c2" && endGame == CHECKMATE) {
				messenger.send(MSG_LOG, description, format("{} --> {:.2f}", game.printMovesHistory(eval.depth), eval.value));
			}*/
			return eval;
		}

		if (depth == 0) {
			const Evaluation eval = evaluator->evaluate(game, this->depth);
			/*if (moveToString(game.movesHistory[0]) == "c3b2") {
				messenger.send(MSG_LOG, description, format("{} --> {:.2f}", game.printMovesHistory(eval.depth), eval.value));
			}*/
			return eval;
		}

		Evaluation best = {0, LOSS_VALUE * 2, NONE, MAX_DEPTH, nullptr};

		for (unsigned int i = 0; i < amount; i++) {
			game.save();
			game.applyMove(moves[i]);

			Evaluation eval = negaMax(game, depth - 1);
			Evaluator::changeSign(eval);
			eval.pvMoves[this->depth - depth] = moves[i];

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
			} else {
				delete eval.pvMoves;
			}
		}

		/*if (moveToString(game.movesHistory[0]) == "c3c2" && depth == 3) {
			messenger.send(MSG_LOG, description, format("best {}", evalToString(best)));
		}*/
		return best;
	}

};