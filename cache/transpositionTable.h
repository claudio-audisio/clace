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

    unsigned int getMoves(Game& game, Move*& moves) {
        unsigned int amount;

        if (tables.contains(game.key)) {
            const Transposition *table = tables.at(game.key);
            game.checkStatus.check = table->check;
            moves = table->moves;
            amount = table->amount;
            usage++;
        } else {
            // TODO nella generazione delle mosse faccio solo il controllo del double check
            // quindi la verify checks viene chiamata solo per quello
            // valutare se conviene utilizzare una versione light basata su allAttacks che e' 10 volte piu' veloce per calcolare solo il check
            // ed eliminare il controllo del double check all'interno del generatore
            // Quando nel generatore verra' gestita tutta l'informazione generata dal verify check allora postremo chiamarlo
            //game.verifyChecks();

            moves = pool->getArray();
            game.verifyChecks(false);
            amount = generateLegalMoves(game, moves);
            addTransposition(game.key, moves, amount, game.checkStatus.check);
            allocation++;
        }

        return amount;
    }

    // for perft where verify check is already done by caller
    unsigned int getOnlyMoves(const Game& game, Move*& moves) {
        unsigned int amount;

        if (tables.contains(game.key)) {
            const Transposition *table = tables.at(game.key);
            moves = table->moves;
            amount = table->amount;
            usage++;
        } else {
            moves = pool->getArray();
            amount = generateLegalMoves(game, moves);
            addTransposition(game.key, moves, amount, game.checkStatus.check);
            allocation++;
        }

        return amount;
    }

    void addTransposition(const unsLL key, Move* moves, const unsigned int amount, const bool check) {
        if (!tables.contains(key)) {
            const auto transposition = static_cast<Transposition*>(malloc(sizeof(Transposition)));
            transposition->moves = moves;
            transposition->amount = amount;
            transposition->check = check;
            tables.insert(make_pair(key, transposition));
        } else {
            throw runtime_error("transposition table already present");
        }
    }

    void remove(const unsLL key) {
        if (tables.contains(key)) {
            const auto transposition = tables.at(key);
            pool->release(transposition->moves);
            free(transposition);
            tables.erase(key);
        }
    }

    int size() const {
        return tables.size();
    }

    int getUsagePercentage() const {
        return (usage * 100) / (usage + allocation);
    }

    string getStatistics() const {
        return format("usage {} % ({} entries)", getUsagePercentage(), size());
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
