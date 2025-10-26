#pragma once

#include "abstract_engine.h"
#include "iengine.h"
#include "../move/movesGenerator.h"

class BF_Engine : public Abstract_Engine {
public:
	explicit BF_Engine(unsigned int depth): Abstract_Engine(depth) {};

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
		Move* moves = pool->getArray(depth);
		MovesAmount amount;
		generateLegalMoves(game, moves, &amount);
		const EndGameType endGame = game.checkEndGame(amount.legal);

		if (endGame != NONE) {
			return endGame == CHECKMATE ? WIN_VALUE : DRAW_VALUE;
		}

		if (depth == 0) {
			const double value = evaluator->evaluate(game);
			//messenger.send(MSG_LOG, "bfEngine", format("{} --> {:.2f}", game.printMovesHistory(this->depth), value));
			return value;
		}

		double best = LOSS_VALUE;

		for (unsigned int i = 0; i < amount.total; i++) {
			if (moves[i]) {
				game.save();
				game.applyMove(moves[i]);
				const double value = -negaMax(game, depth - 1);

				if (value > best) {
					best = value;
				}

				game.rollbackLastMove();
			}
		}

		messenger.send(MSG_LOG, "bfEngine", format("{} --> {:.2f}", game.printMovesHistory(this->depth - depth), best));
		return best;
	}

	string getDescription() override {
		return format("BF_Engine({})", depth);
	}

};