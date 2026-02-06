#pragma once

#include "common/structures.h"
#include "common/constants.h"
#include "utils/number.h"

class Game;

class Evaluator {
public:
	virtual ~Evaluator() = default;

	virtual Evaluation evaluate(Game& game, unsigned int depth) = 0;

	virtual Evaluation evaluateEndGame(Game& game, unsigned int depth, EndGameType endGame) = 0;

	double static signForSide(const Side side) {
		return 1.0 + side * -2.0;
	}

	static bool isBetter(const Evaluation &a, const Evaluation &b) {
		if (areNotEqual(a.value, b.value)) {
			return a.value > b.value;
		}

		if (isPositive(a.value)) {
			return a.depth < b.depth;
		}

		return a.depth > b.depth;
	}

	static bool isBest(const Evaluation& best) {
		return areEqual(best.value, WIN_VALUE) && best.depth == 1;
	}

	static void changeSign(Evaluation &eval) {
		eval.value = -eval.value;
	}

};
