#pragma once

#include "abstractEngine.h"
#include "iengine.h"
#include "../move/movesGenerator.h"

class BruteForceEngine : public AbstractEngine {
public:
	explicit BruteForceEngine(const unsigned int depth): AbstractEngine(depth, "BruteForceEngine") {};

	void _calculateMove(Game& game, Move* moves, const MovesAmount amount) override {
		for (unsigned int i = 0; i < amount.total; i++) {
			if (moves[i]) {
				game.save();
				game.applyMove(moves[i]);
				const double value = -negaMax(game, depth - 1);

				if (value > best.value) {
					best.move = moves[i];
					best.value = value;
				}

				game.rollbackLastMove();
			}
		}
	}

	double negaMax(Game& game, const unsigned int depth) {
		game.verifyChecks();
		Move* moves = pool->getArray();
		MovesAmount amount;
		generateLegalMoves(game, moves, &amount);
		const EndGameType endGame = game.checkEndGame(amount.legal);

		if (endGame != NONE) {
			pool->release(moves);
			return endGame == CHECKMATE ? WIN_VALUE : DRAW_VALUE;
		}

		if (depth == 0) {
			pool->release(moves);
			const double value = evaluator->evaluate(game);
			//messenger.send(MSG_LOG, description, format("{} --> {:.2f}", game.printMovesHistory(this->depth), value));
			return value;
		}

		double best = LOSS_VALUE;

		for (unsigned int i = 0; i < amount.total; i++) {
			if (moves[i]) {
				game.save();
				game.applyMove(moves[i]);
				const double value = -negaMax(game, depth - 1);
				game.rollbackLastMove();

				if (value > best) {
					best = value;
				}
			}
		}

		pool->release(moves);

		//messenger.send(MSG_LOG, description, format("{} --> {:.2f}", game.printMovesHistory(this->depth - depth), best));
		return best;
	}

};