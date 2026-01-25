#pragma once

#include "iengine.h"
#include "../utils/messenger.h"
#include "../evaluation/basicEvaluator.h"
#include "../move/movesGenerator.h"
#include "../utils/arrayPool.h"
#include "../utils/toString.h"


class AbstractEngine : public IEngine {
public:
    AbstractEngine(const unsigned int depth, const string& description) {
        if (depth < 1) {
            throw runtime_error("engine calculation invoked on depth < 1");
        }

        this->description = description;
        this->depth = depth;
        this->pool = new ArrayPool<Move>(this->depth + 1, MAX_MOVES);
        this->evaluator = new BasicEvaluator();
    }

    string description;
    unsigned int depth;
    IEvaluator *evaluator = nullptr;
    ArrayPool<Move> *pool = nullptr;
    Messenger &messenger = Messenger::getInstance();
    Evaluation best;

    virtual void _calculateMove(Game& game, Move *moves, MovesAmount amount) = 0;

    Evaluation calculateMove(Game& game) override {
        messenger.send(MSG_LOG, description, format("{} evaluation at depth {} ({})", game.isWhiteToMove() ? "white" : "black", depth, game.fullMoves));
        auto time = chrono::steady_clock::now();

        game.verifyChecks();
        Move* moves = pool->getArray();
        MovesAmount amount;
        generateLegalMoves(game, moves, &amount);
        const EndGameType endGame = game.checkEndGame(amount.legal);
        best = {0, LOSS_VALUE, endGame};

        if (!endGame) {
            _calculateMove(game, moves, amount);
        }

        pool->release(moves);

        if (!game.isWhiteToMove()) {
            best.value = -best.value;
        }

        if (best.endGameType == NONE) {
            messenger.send(MSG_ALL, description, format("best: {} --> {:.2f} ({} ms)", moveToString(best.move), best.value, getElapsedMillis(time)));
        } else {
            messenger.send(MSG_ALL, description, format("best: --> {} ({} ms)", endGameToString(best.endGameType), getElapsedMillis(time)));
        }

        return best;
    }

    void setEvaluator(IEvaluator* evaluator) override {
        this->evaluator = evaluator;
    }

    string getDescription() override {
        return description;
    }

};
