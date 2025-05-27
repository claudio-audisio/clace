#pragma once

#include <chrono>

#include "../engine/iengine.h"

using namespace std;


class Player {
public:
	Player(bool white);
	Player(bool white, IEngine* engine);
	~Player();

	void init(bool computer, bool white, IEngine* engine);
	/*string getCapturedList();
	string& addToCapturedList(string& capturedList, unsigned int times, Piece piece);*/
	void startMoveTime();
	void stopMoveTime();
	string getMoveTime();

	bool white;
	bool computer;
	IEngine* engine = nullptr;
	unsLL gameTime = 0;
	chrono::time_point<chrono::steady_clock> currentMoveTime;
	string name;
};
