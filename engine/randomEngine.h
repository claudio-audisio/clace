#pragma once

#include <cstdlib>
#include <ctime>

#include "abstractEngine.h"
#include "../move/movesGenerator.h"

using namespace std;

class RandomEngine : public AbstractEngine {
public:
	RandomEngine() : AbstractEngine(1, "RandomEngine") {
		srand(time(nullptr));
	};

	void _calculateMove(Game& game, Move* moves, const MovesAmount amount) override {
		int index;

		do {
			index = getRandom(amount.total);
		} while (moves[index] == 0);

		best.move = moves[index];
	}

	static unsigned int getRandom(unsigned int upper) {
		return rand() % upper;
	}

};