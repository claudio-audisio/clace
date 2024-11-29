#pragma once

#include "../game/game.h"
#include "../evaluation/ievaluator.h"

class IEngine {
public:
    virtual void draw() const = 0;

	virtual Move calculateMove(Game& game, vector<Move>& moves) = 0;

	virtual Move calculateMove(Game& game, vector<Move>& moves, int depth) = 0;

	virtual void setEvaluator(IEvaluator* evaluator) = 0;

	virtual bool isOpponent(Game& game) const = 0;

    virtual ~IEngine() = default;
};
