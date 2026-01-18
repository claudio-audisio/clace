#pragma once
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <unordered_map>

#include "zobrist.h"
#include "../common/structures.h"
#include "../game/game.h"
#include "../move/movesGenerator.h"

using namespace std;

class TranspositionTable {
public:
    TranspositionTable() {
        usage = 0;
    }

    ~TranspositionTable() {
        clear();
    }

    unordered_map<unsLL, Transposition*> tables;
    unsigned int usage;

#ifdef USE_CACHE
    void getMoves(const Game& game, Move*& moves, MovesAmount& amount) {
        if (tables.contains(game.key)) {
            const Transposition *table = tables.at(game.key);
            moves = table->moves;
            amount = table->amount;
            usage++;
        } else {
            generateLegalMoves(game, moves, &amount);
            addMoves(game.key, moves, amount);
        }
    }
#else
    void getMoves(const Game& game, Move*& moves, MovesAmount& amount) {
        generateLegalMoves(game, moves, &amount);
    }
#endif

    void addMoves(const unsLL key, const Move* moves, const MovesAmount& movesAmount) {
        if (!tables.contains(key)) {
            const auto transposition = static_cast<Transposition*>(malloc(sizeof(Transposition)));
            transposition->moves = static_cast<Move*>(malloc(sizeof(Move) * movesAmount.total));
            memcpy(transposition->moves, moves, sizeof(Move) * movesAmount.total);
            transposition->amount.total = movesAmount.total;
            transposition->amount.legal = movesAmount.legal;
            tables.insert(make_pair(key, transposition));
        } else {
            throw runtime_error("transposition table already present");
        }
    }

    int size() const {
        return tables.size();
    }

#ifdef USE_CACHE
    int getUsage() const {
        return usage;
    }
#else
    int getUsage() const {
        return -1;
    }
#endif

    void remove(const unsLL key) {
        if (tables.contains(key)) {
            free(tables.at(key)->moves);
            free(tables.at(key));
            tables.erase(key);
        }
    }

    void clear() {
        auto it = tables.begin();

        while (it != tables.end()) {
            free(it->second->moves);
            free(it->second);
            ++it;
        }

        tables.clear();
    }

};
