#pragma once

#include "../board/piece.h"

class PieceHelper {
public:

	static bool isWhitePiece(Piece piece) {
		return piece < BPawn && piece != Empty;
	}

	static bool isBlackPiece(Piece piece) {
		return piece > WKing;
	}

	static bool isEmpty(Piece piece) {
		return piece == Empty;
	}

};