#pragma once

#include "iengine.h"
#include "../evaluation/basicEvaluator.h"
#include "../move/move.h"
#include "../move/movesGenerator.h"
#include "../utils/vectorPool.h"

class AB_Engine : public Abstract_Engine {
public:
	AB_Engine();

	void _calculateMove(Game& game, Move* moves, MovesAmount amount) {
	};

	// TODO questo engine dovra' implementare la search alfa-beta mozzando i branch non produttivi
	// quindi la lista delle mosse necessita' anche di un ordinamento
	// https://www.chessprogramming.org/Alpha-Beta


	/*explicit AB_Engine(unsigned int depth) {
		//this->depth = (depth * 2) - 1;
		this->depth = depth;
		this->pool = new VectorPool<Move>(this->depth, MAX_MOVES);
		this->evaluator = new BasicEvaluator();
	};

	Messenger& messenger = Messenger::getInstance();
	unsigned int depth;
	IEvaluator* evaluator = nullptr;
	VectorPool<Move>* pool = nullptr;
	Move moveUnderEvaluation;
	double alpha = LOSS_VALUE;	// punteggio minimo di cui il bianco e' sicuro
	double beta = WIN_VALUE;	// punteggio massimo di cui il nero e' sicuro

	void draw() const {};

	Evaluation calculateMove(Game& game, vector<Move>& moves) {


















	}

	Evaluation calculateMove(Game& game, vector<Move>& moves, int depth) {
		Evaluation best = make_pair(0, game.isWhiteToMove() ? LOSS_VALUE : WIN_VALUE);

		for (Move move : moves) {
			game.save();
			game.applyMove(move);
			moveUnderEvaluation = move;
			Evaluation evaluation = evaluate(game, depth + 1);
			const Side side = getMoveSide(move);

			if (isBestEvaluation(side, evaluation.second, best.second)) {
				best.first = move;
				best.second = evaluation.second;

				if (side == _WHITE) {
					if (best.second > alpha) {
						alpha = best.second;
						logger.log(format("new alpha {} --> {:.2f}", moveToString(best.first), best.second));
					}
				} else {
					if (best.second < beta) {
						beta = best.second;
						logger.log(format("new beta {} --> {:.2f}", moveToString(best.first), best.second));
					}
				}
			}

			if (depth > 0) {
				if (side == _WHITE) {
					if (evaluation.second > beta) {
						logger.log(format("{} --> {:.2f} vs beta {:.2f}: beta-cutoff", moveToString(move), evaluation.second, beta));
						game.rollbackLastMove();
						return evaluation;
					}
				} else {
					if (evaluation.second < alpha) {
						logger.log(format("{} --> {:.2f} vs alpha {:.2f}: alpha-cutoff", moveToString(move), evaluation.second, alpha));
						game.rollbackLastMove();
						return evaluation;
					}
				}
			} else {
				logger.log(format("{} --> alpha {:.2f} - beta {:.2f}", moveToString(move), alpha, beta));
			}

			game.rollbackLastMove();
		}

		logger.log(format("best ({}): {}, {} --> {:.2f}", depth, game.printMovesHistory(depth - 1), moveToString(best.first), best.second));
		return best;
	}

	Evaluation evaluate(Game& game, unsigned int currentDepth) {
		game.verifyChecks();
		vector<Move>& moves = pool->getVector(currentDepth - 1);
		generateLegalMoves(game, moves);
		EndGameType endGame = game.checkEndGame(moves.empty());

		if (endGame != NONE) {
			if (endGame == CHECKMATE) {
				return game.isWhiteToMove() ? LOSS_EVALUATION : WIN_EVALUATION;
			} else {
				return DRAW_EVALUATION;
			}
		}

		if (currentDepth >= depth) {
			double value = evaluator->evaluate(game);
			logger.log(format("{} --> {:.2f}", game.printMovesHistory(depth), value));
			return Evaluation(0, value);
		}

		return calculateMove(game, moves, currentDepth);
	}

	void setEvaluator(IEvaluator* evaluator) {
		this->evaluator = evaluator;
	}

	bool isOpponent(Game& game) const {
		return true;	// TODO
	}

	bool isBestEvaluation(Side side, double evaluation, double bestEvaluation) {
		if (side == _WHITE) {
			return evaluation > bestEvaluation;
		}

		return evaluation < bestEvaluation;
	}*/

};