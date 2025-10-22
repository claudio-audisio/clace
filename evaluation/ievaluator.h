#pragma once

//#include "../game/game.h"

class Game;

class IEvaluator {
public:
	virtual ~IEvaluator() = default;
	virtual double evaluate(Game& game) = 0;
};
