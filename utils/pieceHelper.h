#pragma once

#include <list>
#include <unordered_set>
#include <string>
#include <cstring>
#include <assert.h>
#include <stdexcept>

#include "../common/constants.h"

using namespace std;


inline bool _isWhite(const Piece piece) {
	return piece > Empty && piece % 2;
}

inline bool _isBlack(const Piece piece) {
	return piece > Empty && (piece % 2 == 0);
}

inline Side _getSide(const Piece piece) {
	if (piece > 0) {
		return OPPOSITE(piece % 2);
	}

	throw runtime_error("get side on empty cell");
}

inline Side _getOppositeSide(const Piece piece) {
	if (piece > 0) {
		return piece % 2;
	}

	throw runtime_error("get side on empty cell");
}

inline bool _isEmpty(const Piece piece) {
	return piece == Empty;
}

inline bool _isPawn(const Piece piece) {
	return piece == WPawn || piece == BPawn;
}

inline bool _isRook(const Piece piece) {
	return piece == WRook || piece == BRook;
}

inline bool _isKnight(const Piece piece) {
	return piece == WKnight || piece == BKnight;
}

inline bool _isBishop(const Piece piece) {
	return piece == WBishop || piece == BBishop;
}

inline bool _isQueen(const Piece piece) {
	return piece == WQueen || piece == BQueen;
}

inline bool _isKing(const Piece piece) {
	return piece == WKing || piece == BKing;
}

inline bool _isOpponent(const Piece piece, const bool white) {
	return !_isEmpty(piece) && _isWhite(piece) != white;
}

inline bool _isEmptyOrOpponent(const Piece piece, const bool white) {
	return _isEmpty(piece) || _isOpponent(piece, white);
}

inline const unordered_set<Piece>& getXRayTypes(const Side side) {
	return XRAY_PIECES[side];
}

/*
inline bool contains(Piece[] pieces, const Piece piece) {
	for (Piece pt : pieces) {
		if (pt == piece) {
			return true;
		}
	}

	return false;
}
*/

inline Piece valueOf(const string& stringPiece) {
	const string type = stringPiece.substr(1);
	const Side side = stringPiece.at(0) == 'W' ? 0 : 1;

	if (strcmp(type.c_str(), "Pawn") == 0) {
		return 1 + side;
	}

	if (strcmp(type.c_str(), "Knight") == 0) {
		return 3 + side;
	}

	if (strcmp(type.c_str(), "Bishop") == 0) {
		return 5 + side;
	}

	if (strcmp(type.c_str(), "Rook") == 0) {
		return 7 + side;
	}

	if (strcmp(type.c_str(), "Queen") == 0) {
		return 9 + side;
	}

	if (strcmp(type.c_str(), "King") == 0) {
		return 11 + side;
	}

	return 0;
}

inline char getPieceCode(const Piece piece) {
	return PIECE_TO_CODE.at(piece);
}
