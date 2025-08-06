#pragma once

#include "iengine.h"
#include "../utils/logger.h"
#include "../evaluation/basicEvaluator.h"
#include "../move/movesGenerator.h"


class Abstract_Engine : public IEngine {
public:
    Abstract_Engine(const unsigned int depth) {
        if (depth < 1) {
            throw runtime_error("engine calculation invoked on depth < 1");
        }

        this->depth = depth;
        this->pool = new ArrayPool<Move>(this->depth + 1);
        this->evaluator = new BasicEvaluator();
    }

    unsigned int depth;
    IEvaluator* evaluator = nullptr;
    ArrayPool<Move>* pool = nullptr;
    Logger& logger = Logger::getInstance();
    Evaluation best;


    virtual void _calculateMove(Game& game, Move* moves, MovesAmount amount) = 0;

    Evaluation calculateMove(Game& game) override {
        logger.log(format("{} evaluation at depth {} ({})", game.isWhiteToMove() ? "white" : "black", depth, game.fullMoves));
        auto time = chrono::steady_clock::now();

        game.verifyChecks();
        Move* moves = pool->getArray(depth);
        const MovesAmount amount = MovesGenerator::generateLegalMoves(game, moves);
        const EndGameType endGame = game.checkEndGame(amount.second);
        best = {0, LOSS_VALUE, endGame};

        if (!endGame) {
            _calculateMove(game, moves, amount);
        }

        logger.log(format("best: {} --> {:.2f} evaluated in {} us", MoveHelper::toString(best.move), best.value, Utils::getElapsedMicros(time)));

        return best;
    }

    void setEvaluator(IEvaluator* evaluator) override {
        this->evaluator = evaluator;
    };

};
