#pragma once

#include <list>
#include <unordered_set>
#include <string>
#include <cstring>
#include <assert.h>

#include "../common/constants.h"

using namespace std;


static bool _isWhite(const Piece piece) {
	return piece > Empty && piece % 2;
}

static bool _isBlack(const Piece piece) {
	return piece > Empty && (piece % 2 == 0);
}

static Side _getSide(const Piece piece) {
	if (piece > 0) {
		return OPPOSITE(piece % 2);
	}

	assert(false);
}

static Side _getOppositeSide(const Piece piece) {
	if (piece > 0) {
		return piece % 2;
	}

	assert(false);
}

static bool _isEmpty(const Piece piece) {
	return piece == Empty;
}

static bool _isPawn(const Piece piece) {
	return piece == WPawn || piece == BPawn;
}

static bool _isRook(const Piece piece) {
	return piece == WRook || piece == BRook;
}

static bool _isKnight(const Piece piece) {
	return piece == WKnight || piece == BKnight;
}

static bool _isBishop(const Piece piece) {
	return piece == WBishop || piece == BBishop;
}

static bool _isQueen(const Piece piece) {
	return piece == WQueen || piece == BQueen;
}

static bool _isKing(const Piece piece) {
	return piece == WKing || piece == BKing;
}

static bool _isOpponent(const Piece piece, const bool white) {
	return !_isEmpty(piece) && _isWhite(piece) != white;
}

static bool _isEmptyOrOpponent(const Piece piece, const bool white) {
	return _isEmpty(piece) || _isOpponent(piece, white);
}

static const unordered_set<Piece>& getXRayTypes(const Side side) {
	return XRAY_PIECES[side];
}

/*
static bool contains(Piece[] pieces, const Piece piece) {
	for (Piece pt : pieces) {
		if (pt == piece) {
			return true;
		}
	}

	return false;
}
*/

static Piece valueOf(const string& stringPiece) {
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

static char getPieceCode(Piece piece) {
	// TODO use a map
	switch (piece) {
		case WPawn: return 0x0050;
		case BPawn: return 0x0070;
		case WRook: return 0x0052;
		case BRook: return 0x0072;
		case WKnight: return 0x004E;
		case BKnight: return 0x006E;
		case WBishop: return 0x0042;
		case BBishop: return 0x0062;
		case WQueen: return 0x0051;
		case BQueen: return 0x0071;
		case WKing: return 0x004B;
		case BKing: return 0x006B;
		default: return 0x002E;
	}
}
