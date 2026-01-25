#pragma once

#include "iengine.h"
#include "../evaluation/basicEvaluator.h"
#include "../move/move.h"
#include "../move/movesGenerator.h"
#include "../utils/vectorPool.h"

// https://www.chessprogramming.org/Alpha-Beta
class AlphaBetaEngine : public AbstractEngine {
public:
	explicit AlphaBetaEngine(unsigned int depth): AbstractEngine(depth, "AlphaBetaEngine") {};

	void _calculateMove(Game& game, Move* moves, MovesAmount amount) override {
		// TODO le mosse dovrebbero essere ordinate
		// https://www.chessprogramming.org/Move_Ordering
		// Ecco l'ordine di valutazione delle mosse che dovrebbe essere adottato


		double alpha = LOSS_VALUE;	// punteggio minimo di cui il bianco e' sicuro
		double beta = WIN_VALUE;	// punteggio massimo di cui il nero e' sicuro

		for (unsigned int i = 0; i < amount.total; i++) {
			if (moves[i]) {
				game.save();
				game.applyMove(moves[i]);
				const double value = -alphaBeta(game, alpha, beta, depth - 1);

				if (value >= best.value) {
					best.move = moves[i];
					best.value = value;
				}

				game.rollbackLastMove();
			}
		}
	};

	double alphaBeta(Game& game, double alpha, double beta, const unsigned int depth) {
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
			// TODO dovremmo fare un ricerca quiescente
			// https://www.chessprogramming.org/Quiescence_Search
			const double value = evaluator->evaluate(game);
			//messenger.send(MSG_LOG, description, format("{} --> {:.2f}", game.printMovesHistory(this->depth), value));
			return value;
		}

		double best = LOSS_VALUE;

		for (unsigned int i = 0; i < amount.total; i++) {
			if (moves[i]) {
				game.save();
				game.applyMove(moves[i]);
				const double value = -alphaBeta(game, -beta, -alpha, depth - 1);
				game.rollbackLastMove();

				if (value > best) {
					best = value;

					if (value > alpha) {
						alpha = value;
					}
				}

				if (value >= beta) {
					pool->release(moves);
					//messenger.send(MSG_LOG, description, format("{} --> {:.2f} - cutoff", game.printMovesHistory(this->depth - depth), best));
					return best;
				}
			}
		}

		pool->release(moves);
		//messenger.send(MSG_LOG, description, format("{} --> {:.2f}", game.printMovesHistory(this->depth - depth), best));
		return best;
	}

};