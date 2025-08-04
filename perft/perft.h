#pragma once

#include <string>

#include "../game/game.h"
#include "result.h"
#include "../utils/vectorPool.h"
#include "../utils/arrayPool.h"
#include "../move/movesCache.h"

using namespace std;


class Perft {
public:
	Perft(const string& fenGame, unsigned int depth);
	~Perft();

	Result* runBulk();
	Result* runBulk_new();
    Result* run(bool consoleMode = false);
	Result* runNew(bool consoleMode = false);

private:
	Game* game;
	unsigned int depth;
	string fenGame;
	Result* result;
    VectorPool<Move>* pool;
	ArrayPool<Move>* pool_new;
    MovesCache* cache;
    unsigned int cacheUsage = 0;
    unsigned int generatorUsage = 0;

	unsLL runBulkPerft(unsigned int depth);
	unsLL runBulkPerft_new(unsigned int depth);
    void runPerft(unsigned int depth);
	void runPerftNew(unsigned int depth);

};