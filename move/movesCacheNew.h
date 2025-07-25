#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../common/types.h"
#include "../game/game.h"
#include "../utils/fen.h"

using namespace std;


class MovesCacheNew {
public:
    explicit MovesCacheNew(unsigned int size) {
        cache.reserve(size);
    }

	~MovesCacheNew() = default;

    void add(Game& game, vector<Move>* moves) {
        add(FEN::gameToFENKey(game), moves);
    }

    void add(const string& game, vector<Move>* moves) {
        cache.insert(make_pair(game, moves));
    }

    bool get(Game& game, vector<Move>*& moves) {
        return get(FEN::gameToFENKey(game), moves);
    }

    bool get(const string& game, vector<Move>*& moves) {
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
    unordered_map<string, vector<Move>*> cache;

};