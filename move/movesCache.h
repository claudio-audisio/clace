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

    void add(Game& game, const vector<Move>& moves) {
        add(FEN::gameToFENKey(game), moves);
    }

    void add(const string& game, const vector<Move>& moves) {
        cache.insert(make_pair(game, moves));
    }

    bool get(Game& game, vector<Move>& moves) {
        return get(FEN::gameToFENKey(game), moves);
    }

    bool get(const string& game, vector<Move>& moves) {
        auto it = cache.find(game);

        if (it == cache.end()) {
            return false;
        }

        moves = it->second;
        return true;
    }

    unsigned int size() {
        return cache.size();
    }

    void clear() {
        cache.clear();
    }

private:
    unordered_map<string, vector<Move>> cache;

};