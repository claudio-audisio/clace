#pragma once

#include "../clace.h"
#include "../engine/iengine.h"

using namespace std;


class Player {
	public:
		Player();
		Player(const bool white);
		Player(const bool white, IEngine* engine);
		Player(const string& name, const bool white);
		~Player();

		void init(const string& name, const bool computer, const bool white, IEngine* engine);
		void setPieces(char* pieces);
		void resetPieces();
		void incrementPieces(const Piece piece);
		string getNameAndColor();
		void onCaptured(const Piece piece);

		bool isComputer() const {
			return computer;
		}

		char* getPieces() const {
			return pieces;
		}

	private:
		string name;
		bool white;
		bool computer;
		IEngine* engine = nullptr;
		int gameTime = 0;
		int currentMoveTime;
		char* pieces = new char[7];

		void initPieces();
		
};
