#pragma once

#include "iengine.h"
#include "../utils/messenger.h"
#include "../evaluation/basicEvaluator.h"
#include "../utils/toString.h"
#include "../cache/transpositionTable.h"


class AbstractEngine : public IEngine {
public:
    string description;
    unsigned int depth;
    TranspositionTable *table = nullptr;
    Evaluator *evaluator = nullptr;
    Messenger &messenger = Messenger::getInstance();
    Evaluation best;


    AbstractEngine(const unsigned int depth, const string& description) {
        if (depth < 1) {
            throw runtime_error("engine calculation invoked on depth < 1");
        }

        this->description = description;
        this->depth = depth;
        this->table = new TranspositionTable();
        this->evaluator = new BasicEvaluator();
    }

    ~AbstractEngine() override {
        delete this->table;
        delete this->evaluator;
    }

    virtual void _calculateMove(Game& game, Move *moves, MovesAmount amount) = 0;

    Evaluation calculateMove(Game& game) override {
        messenger.send(MSG_LOG, description, format("{} evaluation at depth {} ({})", game.isWhiteToMove() ? "white" : "black", depth, game.fullMoves));
        auto time = chrono::steady_clock::now();

        game.verifyChecks();
        Move* moves;
        MovesAmount amount;
        table->getMoves(game, moves, amount);
        const EndGameType endGame = game.checkEndGame(amount.legal);
        best = {0, LOSS_VALUE * 2, endGame, MAX_DEPTH, nullptr};

        if (!endGame) {
            _calculateMove(game, moves, amount);
        }

        if (!game.isWhiteToMove()) {
            best.value = -best.value;
        }

        messenger.send(MSG_ALL, description, format("best: {} ({} ms)", evalToString(best), getElapsedMillis(time)));

        return best;
    }

    void setEvaluator(Evaluator* evaluator) override {
        this->evaluator = evaluator;
    }

    string getDescription() override {
        return description;
    }

};
