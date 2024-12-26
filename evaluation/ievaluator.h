#pragma once

#include "../game/game.h"

class IEvaluator {
public:
	virtual double evaluate(Game& game) = 0;
};
