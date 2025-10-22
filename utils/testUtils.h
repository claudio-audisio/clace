#pragma once

#include <deque>
#include <chrono>
#include <list>
#include <string>

#include "../move/move.h"
#include "../utils/toString.h"

using namespace std;


class TestUtils {
public:
	template <typename... SetOfString>
	static bool containsExactlyInAnyOrder(vector<Move>& moves, SetOfString... stringMoves) {
		unsigned int counter = 0;
		for (string stringMove : { stringMoves... }) {
			++counter;
			bool found = false;
			for (Move move : moves) {
				if (strcmp(moveToString(move).c_str(), stringMove.c_str()) == 0) {
					found = true;
					break;
				}
			}
			if (!found) {
				return false;
			}
		}
		return moves.size() == counter;
	}

};

