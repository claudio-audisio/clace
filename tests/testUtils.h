#pragma once

#include "../common/types.h"
#include "../utils/boardUtils.h"

template <typename... SetOfPosition>
bool containsAll(list<Position> listOfPositions, SetOfPosition... positions) {
	for (Position positionToCheck : {positions...}) {
		bool found = false;
		for (Position position : listOfPositions) {
			if (position == positionToCheck) {
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}
	return true;
}

template <typename... SetOfPosition>
static bool checkBoard(Rawboard board, SetOfPosition... expectedPositions) {
	Rawboard bitPositions = 0;

	for (Position position : {expectedPositions...}) {
		if (position != NO_POS) {
			bitPositions |= posInd(position);
		}
	}

	return board == bitPositions;
}

static bool checkBoard(Rawboard board, list<Position> expectedPositions) {
	Rawboard bitPositions = 0;

	for (Position position : expectedPositions) {
		if (position != NO_POS) {
			bitPositions |= posInd(position);
		}
	}

	return board == bitPositions;
}

static bool checkBoardNoPos(Rawboard board) {
	return checkBoard(board, NO_POS);
}