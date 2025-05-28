#pragma once

#include "iengine.h"
#include "../evaluation/basicEvaluator.h"
#include "../move/move.h"
#include "../move/movesGenerator.h"
#include "../utils/vectorPool.h"
#include "../utils/logger.h"

class BF_Engine : public IEngine {
public:
	explicit BF_Engine(unsigned int depth) {
		this->depth = depth;
		this->pool = new VectorPool<Move>(this->depth, MAX_MOVES);
		this->evaluator = new BasicEvaluator();
	};

	unsigned int depth;
	IEvaluator* evaluator = nullptr;
	VectorPool<Move>* pool = nullptr;
	Logger& logger = Logger::getInstance();

	void draw() const {};

	Evaluation calculateMove(Game& game, vector<Move>& moves) override {
		logger.log(format("{} evaluation at depth {} ({})", game.isWhiteToMove() ? "white" : "black", depth, game.fullMoves));
		auto time = chrono::steady_clock::now();
		Evaluation best = make_pair(0, LOSS_VALUE);

		for (Move move : moves) {
			game.save();
			game.applyMove(move);
			const double value = -negaMax(game, depth - 1);

			if (value > best.second) {
				best.first = move;
				best.second = value;
			}

			game.rollbackLastMove();
		}

		logger.log(format("best: {} --> {:.2f} evaluated in {} us", MoveHelper::toString(best.first), best.second, Utils::getElapsedMicros(time)));
		return best;
	}

	double negaMax(Game& game, const unsigned int depth) {
		game.verifyChecks();
		vector<Move>& moves = pool->getVector(depth);
		MovesGenerator::generateLegalMoves(game, moves);
		EndGameType endGame = game.checkEndGame(moves.empty());

		if (endGame != EndGameType::NONE) {
			return endGame == EndGameType::CHECKMATE ? WIN_VALUE : DRAW_VALUE;
		}

		if (depth == 0) {
			const double value = evaluator->evaluate(game);
			//logger.log(format("{} --> {:.2f}", game.printMovesHistory(this->depth), value));
			return value;
		}

		double best = LOSS_VALUE;

		for (Move move : moves) {
			game.save();
			game.applyMove(move);
			const double value = -negaMax(game, depth - 1);

			if (value > best) {
				best = value;
			}

			game.rollbackLastMove();
		}

		//logger.log(format("{} --> {:.2f}", game.printMovesHistory(this->depth - depth), best));
		return best;
	}

	void setEvaluator(IEvaluator* evaluator) override {}
};