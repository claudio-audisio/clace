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
		this->depth = (depth * 2) - 1;
		this->pool = new VectorPool<Move>(this->depth, MAX_MOVES);
		this->evaluator = new BasicEvaluator();
	};

	unsigned int depth;
	IEvaluator* evaluator = nullptr;
	VectorPool<Move>* pool = nullptr;
	Move moveUnderEvaluation;
	Logger& logger = Logger::getInstance();

	void draw() const {};

	Evaluation calculateMove(Game& game, vector<Move>& moves) {
		logger.log(format("{} evaluation ({})", game.isWhiteToMove() ? "white" : "black", game.fullMoves));
		auto time = chrono::steady_clock::now();
		const Evaluation evaluation = calculateMove(game, moves, 0);
		logger.log(format("best: {} --> {:.2f} evaluated in {} us", MoveHelper::toString(evaluation.first), evaluation.second, Utils::getElapsedMicros(time)));
		return evaluation;
	}

	Evaluation calculateMove(Game& game, vector<Move>& moves, int depth) override {
		Evaluation best = make_pair(0, 0);

		for (Move move : moves) {
			game.save();
			game.applyMove(move);
			moveUnderEvaluation = move;
			Evaluation evaluation = evaluate(game, depth + 1);

			if (isBestEvaluation(MoveHelper::getSide(move), evaluation.second, best.second) || best.first == 0) {
				best.first = move;
				best.second = evaluation.second;
			}

			game.rollbackLastMove();
		}

		if (depth + 1 >= this->depth) {
			//logger.log(format("best ({}): {}, {} --> {:.2f}", depth, game.printMovesHistory(depth - 1), MoveHelper::toString(best.first), best.second));
		}

		return best;
	}

	Evaluation evaluate(Game& game, unsigned int currentDepth) {
		game.verifyChecks();
		vector<Move>& moves = pool->getVector(currentDepth - 1);
		MovesGenerator::generateLegalMoves(game, moves);
		EndGameType endGame = game.checkEndGame(moves.empty());

		if (endGame != EndGameType::NONE) {
			if (endGame == EndGameType::CHECKMATE) {
				//cout << "Checkmate on {}" << printMovesHistory() << endl;
				return game.isWhiteToMove() ? LOSS_EVALUATION : WIN_EVALUATION;
			} else {
				// TODO si vero la valutazione e' zero ma la partita e' finita, bisogna considerare il fatto che magari si vorrebbe provare altre strade anche se peggiori
				// Quando e' patta devo valutare se io o il mio avversario puo' ancora vincerla
				// Se posso ancora vinverla la patta e' una mezza sconfitta, soprattutto se l'avversario non puo' piu' vicnerla (e viceversa)
				return DRAW_EVALUATION;
			}
		}

		if (currentDepth >= depth) {
			double value = evaluator->evaluate(game);
			//logger.log(format("{} --> {:.2f}", game.printMovesHistory(depth), value));
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
		if (side == WHITE) {
			return evaluation > bestEvaluation;
		}

		return evaluation < bestEvaluation;
	}

};