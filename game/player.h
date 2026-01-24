#pragma once

#include <chrono>

#include "../engine/iengine.h"

using namespace std;


class Player {
public:
	explicit Player(Side side);
	Player(Side side, IEngine* engine);
	~Player();

	void init(bool computer, Side side, IEngine* engine);
	/*string getCapturedList();
	string& addToCapturedList(string& capturedList, unsigned int times, Piece piece);*/
	void startMoveTime();
	void stopMoveTime();
	string getMoveTime();

	Side side;
	bool computer;
	IEngine* engine = nullptr;
	unsLL gameTime = 0;
	chrono::time_point<chrono::steady_clock> currentMoveTime;
	string name;

	string getDescription() const {
		if (engine) {
			return engine->getDescription();
		}

		return "human";
	}

};
