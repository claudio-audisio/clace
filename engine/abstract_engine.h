#pragma once

#include "iengine.h"
#include "../utils/messenger.h"
#include "../evaluation/basicEvaluator.h"
#include "../move/movesGenerator.h"
#include "../utils/toString.h"


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
    Messenger& messenger = Messenger::getInstance();
    Evaluation best;


    virtual void _calculateMove(Game& game, Move* moves, MovesAmount amount) = 0;

    Evaluation calculateMove(Game& game) override {
        messenger.send(MSG_LOG, "abstractEngine", format("{} evaluation at depth {} ({})", game.isWhiteToMove() ? "white" : "black", depth, game.fullMoves));
        auto time = chrono::steady_clock::now();

        game.verifyChecks();
        Move* moves = pool->getArray(depth);
        const MovesAmount amount = generateLegalMoves(game, moves);
        const EndGameType endGame = game.checkEndGame(amount.second);
        best = {0, LOSS_VALUE, endGame};

        if (!endGame) {
            _calculateMove(game, moves, amount);
        }

        if (!game.isWhiteToMove()) {
            best.value = -best.value;
        }

        if (best.endGameType == NONE) {
            messenger.send(MSG_ALL, "abstractEngine", format("best: {} --> {:.2f} ({} ms)", moveToString(best.move), best.value, getElapsedMillis(time)));
        } else {
            messenger.send(MSG_ALL, "abstractEngine", format("best: --> {} ({} ms)", endGameToString(best.endGameType), getElapsedMillis(time)));
        }

        return best;
    }

    void setEvaluator(IEvaluator* evaluator) override {
        this->evaluator = evaluator;
    };

};
