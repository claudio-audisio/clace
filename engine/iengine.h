#pragma once

#include "../game/game.h"
#include "../evaluation/ievaluator.h"

class IEngine {
public:
	virtual void setEvaluator(IEvaluator* evaluator) = 0;

	virtual Evaluation calculateMove(Game& game, vector<Move>& moves) = 0;

    virtual ~IEngine() = default;
};
