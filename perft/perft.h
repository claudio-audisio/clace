#pragma once

#include <string>

#include "../game/game.h"
#include "result.h"

using namespace std;


class Perft {
public:
	Perft(const string& fenGame, const unsigned int depth);
	~Perft();

	Result* runBulk();

private:
	Game* game;
	unsigned int depth;
	string fenGame;
	Result* result;

	unsLL runBulkPerft(const unsigned int depth);
	unsLL runBulkPerft2(const unsigned int depth);

};