#pragma once

#include "../game/game.h"
#include "../evaluation/ievaluator.h"
#include "../utils/arrayPool.h"


class IEngine {
public:

	virtual void setEvaluator(IEvaluator* evaluator) = 0;

	virtual Evaluation calculateMove(Game& game) = 0;

	virtual string getDescription() = 0;

    virtual ~IEngine() = default;

};
