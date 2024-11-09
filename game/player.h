#pragma once

#include <chrono>

#include "../clace.h"
#include "../engine/iengine.h"

using namespace std;


class Player {
public:
	Player();
	Player(bool white);
	Player(bool white, IEngine* engine);
	Player(const string& name, bool white);
	~Player();

	void init(const string& name, bool computer, bool white, IEngine* engine);
	string getNameAndColor();
	/*string getCapturedList();
	string& addToCapturedList(string& capturedList, unsigned int times, Piece piece);*/
	void startMoveTime();
	void stopMoveTime();
	string getMoveTime();

	string name;
	bool white;
	bool computer;
	IEngine* engine = nullptr;
	unsLL gameTime = 0;
	chrono::time_point<chrono::steady_clock> currentMoveTime;

};
