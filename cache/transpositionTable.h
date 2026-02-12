#pragma once
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <unordered_map>

#include "zobrist.h"
#include "../common/structures.h"
#include "../game/game.h"
#include "../move/movesGenerator.h"
#include "utils/arrayPool.h"

using namespace std;

class TranspositionTable {

    unordered_map<unsLL, Transposition*> tables;
    ArrayPool<Move> *pool;
    unsigned int usage, allocation;

public:
    TranspositionTable() {
        allocation = usage = 0;
        pool = new ArrayPool<Move>(1500000, MAX_MOVES);
    }

    TranspositionTable(const unsigned int preallocation) {
        allocation = usage = 0;
        pool = new ArrayPool<Move>(preallocation, MAX_MOVES);
    }

    ~TranspositionTable() {
        clear();
    }

    unsigned int getMoves(const Game& game, Move*& moves) {
        unsigned int amount;

        if (tables.contains(game.key)) {
            const Transposition *table = tables.at(game.key);
            moves = table->moves;
            amount = table->amount;
            usage++;
        } else {
            moves = pool->getArray();
            amount = generateLegalMoves(game, moves);
            addMoves(game.key, moves, amount);
            allocation++;
        }

        return amount;
    }

    void addMoves(const unsLL key, Move* moves, const unsigned int amount) {
        if (!tables.contains(key)) {
            const auto transposition = static_cast<Transposition*>(malloc(sizeof(Transposition)));
            transposition->moves = moves;
            transposition->amount = amount;
            tables.insert(make_pair(key, transposition));
        } else {
            throw runtime_error("transposition table already present");
        }
    }

    int size() const {
        return tables.size();
    }

    int getUsagePercentage() const {
        return (usage * 100) / (usage + allocation);
    }

    void remove(const unsLL key) {
        if (tables.contains(key)) {
            const auto transposition = tables.at(key);
            pool->release(transposition->moves);
            free(transposition);
            tables.erase(key);
        }
    }

    void clear() {
        auto it = tables.begin();

        while (it != tables.end()) {
            pool->release(it->second->moves);
            free(it->second);
            ++it;
        }

        tables.clear();
        delete pool;
    }

};
