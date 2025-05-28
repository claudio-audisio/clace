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

	Evaluation calculateMove(Game& game, vector<Move>& moves) {
		return make_pair(moves[getRandom(moves.size())], 0);
	}

	void setEvaluator(IEvaluator* evaluator) {
		this->evaluator = evaluator;
	};

	static unsigned int getRandom(unsigned int upper) {
		return rand() % upper;
	}

};