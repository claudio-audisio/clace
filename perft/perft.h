#pragma once

#include <string>

#include "../game/game.h"
#include "result.h"
#include "../utils/vectorPool.h"
#include "../move/movesCache.h"

using namespace std;


class Perft {
public:
	Perft(const string& fenGame, unsigned int depth);
	~Perft();

	Result* runBulk();
    Result* run(bool consoleMode = false);

private:
	Game* game;
	unsigned int depth;
	string fenGame;
	Result* result;
    VectorPool<Move>* pool;
    MovesCache* cache;
    unsigned int cacheUsage = 0;
    unsigned int generatorUsage = 0;

	unsLL runBulkPerft(unsigned int depth);
    void runPerft(unsigned int depth);
};