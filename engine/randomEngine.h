#pragma once

#include <cstdlib>
#include <ctime>

#include "abstractEngine.h"
#include "../move/movesGenerator.h"

using namespace std;

class RandomEngine : public AbstractEngine {
public:
	RandomEngine() : AbstractEngine(1, "RandomEngine") {
		evaluator = new BasicEvaluator();
		srand(time(nullptr));
	};

	Evaluation _calculateMove(Game& game) override {
		game.verifyChecks();
		Move* moves;
		MovesAmount amount;
		table->getMoves(game, moves, amount);
		const EndGameType endGame = game.checkEndGame(amount.legal);

		if (endGame != NONE) {
			return evaluator->evaluateEndGame(game, 0, endGame);
		}

		int index;

		do {
			index = getRandom(amount.total);
		} while (moves[index] == 0);

		return {moves[index], 0, NONE, 0, nullptr};
	}

	static unsigned int getRandom(unsigned int upper) {
		return rand() % upper;
	}

};