#pragma once

#include <chrono>

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
		string getCapturedList();
		string& addToCapturedList(string& capturedList, unsigned int times, Piece piece);
		void startMoveTime();
		void stopMoveTime();
		string getMoveTime();

		bool isComputer() const {
			return computer;
		}

		char* getPieces() const {
			return pieces;
		}

		unsigned long long getGameTime() const {
			return gameTime;
		}

	private:
		string name;
		bool white;
		bool computer;
		IEngine* engine = nullptr;
		unsigned long long gameTime = 0;
		chrono::time_point<chrono::steady_clock> currentMoveTime;
		char* pieces = new char[7];

		void initPieces();
		
};
