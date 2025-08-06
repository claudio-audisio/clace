#pragma once

#include <cstdlib>
#include <ctime>

#include "abstract_engine.h"
#include "../move/movesGenerator.h"

using namespace std;

class R_Engine : public Abstract_Engine {
public:
	R_Engine() : Abstract_Engine(1) {
		srand(time(nullptr));
	};

	void _calculateMove(Game& game, Move* moves, MovesAmount amount) override {
		int index;

		do {
			index = getRandom(amount.first);
		} while (moves[index] == 0);

		best.move = moves[index];
	}

	static unsigned int getRandom(unsigned int upper) {
		return rand() % upper;
	}

};