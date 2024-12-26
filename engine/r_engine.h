#pragma once

#include <cstdlib>
#include <ctime>

#include "iengine.h"
#include "../move/move.h"
#include "../move/movesGenerator.h"
#include "../utils/vectorPool.h"
#include "../evaluation/basicEvaluator.h"

using namespace std;

class R_Engine : public IEngine {
public:
	R_Engine() {
		evaluator = nullptr;
		srand(time(nullptr));
	};

	IEvaluator* evaluator;

	void draw() const {};

	Evaluation calculateMove(Game& game, vector<Move>& moves) {
		return make_pair(moves[getRandom(moves.size())], 0);
	}

	Evaluation calculateMove(Game& game, vector<Move>& moves, int depth) {
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