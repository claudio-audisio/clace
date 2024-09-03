#pragma once

#include <string>

#include "../board/piece.h"

using namespace std;

class Move {
public:
	Move(string& move, bool white);
	Move(Position sourcePosition, Position destinationPosition, bool white);
	~Move();

private:
	int sourcePosition;
	int destinationPosition;
	bool white;
	Piece pieceType;
	bool castling = false;
	bool enPassant = false;
	bool pawnPromotion = false;
	bool captured = false;
	Piece promotion = Empty;
	bool isComputer;
	double evaluationValue;
};
