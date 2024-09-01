#pragma once;

#include "../clace.h"
#include "../engine/iengine.h"

using namespace std;


class Player {
	public:
		Player();
		~Player();

	private:
		string name;
		bool white;
		bool computer;
		IEngine* engine;
		int gameTime = 0;
		int currentMoveTime;
		char* pieces;
};
