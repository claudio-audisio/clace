#pragma once;

#include "../board/piece.h"

class Move {
public:
	Move();
	~Move();

private:
	const int sourcePosition;
	const int destinationPosition;
	const bool white;
	Piece pieceType;
	bool castling = false;
	bool enPassant = false;
	bool pawnPromotion = false;
	bool captured = false;
	Piece promotion = Empty;
	bool isComputer;
	double evaluationValue;
};
