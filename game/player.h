#pragma once

#include "../clace.h"
#include "../engine/iengine.h"

using namespace std;


class Player {
	public:
		Player();
		~Player();

		void init(string& name, bool computer, bool white, IEngine* engine);
		void resetPieces();
		void incrementPieces(Piece piece);

	private:
		string name;
		bool white;
		bool computer;
		IEngine* engine = 0;
		int gameTime = 0;
		int currentMoveTime;
		char* pieces = new char[7];

		void initPieces();
};
