#pragma once

#include "../common/types.h"
#include "../utils/boardUtils.h"

template <typename... SetOfPosition>
static bool checkBoard(Rawboard board, SetOfPosition... expectedPositions) {
	Rawboard bitPositions = 0;

	for (Position position : {expectedPositions...}) {
		if (position != NO_POS) {
			bitPositions |= BoardUtils::posInd(position);
		}
	}

	return board == bitPositions;
}

static bool checkBoard(Rawboard board, list<Position> expectedPositions) {
	Rawboard bitPositions = 0;

	for (Position position : expectedPositions) {
		if (position != NO_POS) {
			bitPositions |= BoardUtils::posInd(position);
		}
	}

	return board == bitPositions;
}

static bool checkBoardNoPos(Rawboard board) {
	return checkBoard(board, NO_POS);
}