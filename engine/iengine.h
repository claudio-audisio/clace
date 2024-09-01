#pragma once

class IEngine {
public:

    virtual void draw() const = 0;

	virtual Move calculateMove(Game board) = 0;

	virtual Move calculateMove(Game board, int depth) = 0;

	virtual void setEvaluator(IEvaluator evaluator) = 0;

	virtual bool isOpponent(Game board) const = 0;

    virtual ~IEngine() {}
};
