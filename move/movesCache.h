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
        cacheNew.reserve(size);
    }

	~MovesCache() {
        clear();
        clearNew();
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

    void addNew(Game& game, Move moves[], int total, int notValid) {
        addNew(FEN::gameToFENKey(game), moves, total, notValid);
    }

    void addNew(const string& game, Move moves[], int total, int notValid) {
        Moves* cacheEntry = static_cast<Moves*>(malloc(sizeof(Moves)));
        cacheEntry->movesList = new Move[total];;
        memcpy(cacheEntry->movesList, moves, total * sizeof(Move));
        cacheEntry->total = total;
        cacheEntry->notValid = notValid;
        cacheNew.insert(make_pair(game, cacheEntry));
    }

    bool getNew(Game& game, Move moves[], pair<unsigned int, unsigned int>& res) {
        return getNew(FEN::gameToFENKey(game), moves, res);
    }

    bool getNew(const string& game, Move*& moves, pair<unsigned int, unsigned int>& res) {
        auto it = cacheNew.find(game);

        if (it == cacheNew.end()) {
            return false;
        }

        moves = it->second->movesList;
        res.first = it->second->total;
        res.second = it->second->notValid;

        return true;
    }

    unsigned int sizeNew() {
        return cacheNew.size();
    }

    void clearNew() {
        auto it = cacheNew.begin();

        while (it != cacheNew.end()) {
            delete it->second->movesList;
            free(it->second);
            ++it;
        }

        cacheNew.clear();
    }

private:
    struct Moves {
        Move* movesList;
        unsigned int total;
        unsigned int notValid;
    };

    unordered_map<string, vector<Move>> cache;
    unordered_map<string, Moves*> cacheNew;

};