#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../common/types.h"
#include "../game/game.h"
#include "../utils/fen.h"

using namespace std;


class MovesCache {
public:
    explicit MovesCache(unsigned int size) {
        cache.reserve(size);
    }

	~MovesCache() {
        clear();
    }

    void add(Game& game, Move moves[], int total, int valid) {
        add(FEN::gameToFENKey(game), moves, total, valid);
    }

    void add(const string& game, Move moves[], int total, int valid) {
        Moves* cacheEntry = static_cast<Moves*>(malloc(sizeof(Moves)));
        cacheEntry->movesList = new Move[total];;
        memcpy(cacheEntry->movesList, moves, total * sizeof(Move));
        cacheEntry->amount.total = total;
        cacheEntry->amount.legal = valid;
        cache.insert(make_pair(game, cacheEntry));
    }

    bool get(Game& game, Move moves[], MovesAmount& amount) {
        return get(FEN::gameToFENKey(game), moves, amount);
    }

    bool get(const string& game, Move*& moves, MovesAmount& amount) {
        auto it = cache.find(game);

        if (it == cache.end()) {
            return false;
        }

        moves = it->second->movesList;
        amount.total = it->second->amount.total;
        amount.legal = it->second->amount.legal;

        return true;
    }

    unsigned int sizeNew() {
        return cache.size();
    }

    void clear() {
        auto it = cache.begin();

        while (it != cache.end()) {
            delete it->second->movesList;
            free(it->second);
            ++it;
        }

        cache.clear();
    }

private:
    struct Moves {
        Move* movesList;
        MovesAmount amount;
    };

    unordered_map<string, Moves*> cache;

};