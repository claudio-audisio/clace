#pragma once

#include <cstdlib>
#include <ctime>

#include "iengine.h"
#include "../move/move.h"
#include "../move/movesGenerator.h"
#include "../utils/vectorPool.h"

using namespace std;

class R_Engine : public IEngine {
public:
	R_Engine() {
		srand(time(nullptr));
	};

	IEvaluator* evaluator;

	void draw() const {};

	Move calculateMove(Game& game, vector<Move>& moves) {
		return moves[getRandom(moves.size())];
	}

	Move calculateMove(Game& game, vector<Move>& moves, int depth) {
		return calculateMove(game, moves);
	}

	void setEvaluator(IEvaluator* evaluator) {
		this->evaluator = evaluator;
	};

	bool isOpponent(Game& board) const {
		return true;
	};

	static unsigned int getRandom(unsigned int upper) {
		return rand() % upper;
	}

};