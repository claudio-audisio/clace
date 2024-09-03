#include "move.h"
#include "../utils/positions.h"
#include "../common/constants.h"


Move::Move(string& move, bool white) {
	// TODO tirare un'eccezione
	/*
	if (move.length() != 5) {
		throw new RuntimeException("move malformed");
	}
	*/

	this->sourcePosition = Positions::coordsToIndex(move.substr(0, 2));
	this->destinationPosition = Positions::coordsToIndex(move.substr(3, 5));
	this->white = white;
}

Move::Move(Position sourcePosition, Position destinationPosition, bool white) {
	this->sourcePosition = sourcePosition;
	this->destinationPosition = destinationPosition;
	this->white = white;
	this->isComputer = false;
	this->evaluationValue = DRAW_VALUE;
}

Move::~Move() {
}
