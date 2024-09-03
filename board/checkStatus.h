#pragma once

#include "../common/types.h"

#include <unordered_map>

using namespace std;


class CheckStatus {
	public:
		CheckStatus();
		~CheckStatus();

	private:
        Rawboard allCheckPositions;                          // all positions under check
        unordered_map<Position, Rawboard> checkPositions;    // positions under check by piece position
        unordered_map<Position, Rawboard> xRayPositions;     // xray positions under check by piece position
        bool check;
        bool discoveryCheck;
        bool doubleCheck;
        bool checkmate;
};
