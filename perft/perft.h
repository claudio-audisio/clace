#pragma once

#include <string>

#include "../game/game.h"
#include "result.h"
#include "../move/movesCache.h"
#include "../utils/logger.h"

using namespace std;


class MovePool;

class Perft {
public:
	Perft(const string& fenGame, unsigned int depth);
	~Perft();

	Result* runBulk();
	Result* runComplete(bool consoleMode = false);

private:
	Game* game;
	unsigned int depth;
	string fenGame;
	Result* result;
	MovePool* pool;
    MovesCache* cache;
    unsigned int cacheUsage = 0;
    unsigned int generatorUsage = 0;

	unsLL runBulkPerft(unsigned int depth);
	void runCompletePerft(unsigned int depth);
	unsLL runBulkPerft_NEW(unsigned int depth);

	Messenger& messenger = Messenger::getInstance();
};