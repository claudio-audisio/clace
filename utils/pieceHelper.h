#pragma once

#include <list>
#include <unordered_set>
#include <string>
#include <cstring>
#include <assert.h>

#include "../common/constants.h"
#include "../board/piece.h"

using namespace std;

class PieceHelper {
public:

	inline static const list<Piece> WHITE_PROMOTION_PIECES = list<Piece>{ WRook, WKnight, WBishop, WQueen };
	inline static const list<Piece> BLACK_PROMOTION_PIECES = list<Piece>{ BRook, BKnight, BBishop, BQueen };
	inline static const unordered_set<Piece> WHITE_XRAY_PIECES = unordered_set<Piece>{ WRook, WBishop, WQueen };
	inline static const unordered_set<Piece> BLACK_XRAY_PIECES = unordered_set<Piece>{ BRook, BBishop, BQueen };

	inline static bool isWhite(const Piece piece) {
		return piece > Empty && piece % 2;
	}

	inline static bool isBlack(const Piece piece) {
		return piece > Empty && (piece % 2 == 0);
	}

	inline static Side getSide(const Piece piece) {
		if (piece > 0) {
			return piece % 2 ? WHITE : BLACK;
		}

		assert(false);
	}

	inline static Side getOppositeSide(const Piece piece) {
		if (piece > 0) {
			return piece % 2 ? BLACK : WHITE;
		}

		assert(false);
	}

	inline static bool isEmpty(const Piece piece) {
		return piece == Empty;
	}

	inline static bool isPawn(const Piece piece) {
		return piece == WPawn || piece == BPawn;
	}

	inline static bool isRook(const Piece piece) {
		return piece == WRook || piece == BRook;
	}

	inline static bool isKnight(const Piece piece) {
		return piece == WKnight || piece == BKnight;
	}

	inline static bool isBishop(const Piece piece) {
		return piece == WBishop || piece == BBishop;
	}

	inline static bool isQueen(const Piece piece) {
		return piece == WQueen || piece == BQueen;
	}

	inline static bool isKing(const Piece piece) {
		return piece == WKing || piece == BKing;
	}

	inline static bool isOpponent(const Piece piece, const bool white) {
		return !isEmpty(piece) && isWhite(piece) != white;
	}

	inline static bool isEmptyOrOpponent(const Piece piece, const bool white) {
		return isEmpty(piece) || isOpponent(piece, white);
	}

	inline static const list<Piece>& getPromotionTypes(const Side side) {
		return side ? BLACK_PROMOTION_PIECES : WHITE_PROMOTION_PIECES;
	}

	inline static const unordered_set<Piece>& getXRayTypes(const Side side) {
		return side ? BLACK_XRAY_PIECES : WHITE_XRAY_PIECES;
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
		else if (strcmp(type.c_str(), "Knight") == 0) {
			return 3 + side;
		}
		else if (strcmp(type.c_str(), "Bishop") == 0) {
			return 5 + side;
		}
		else if (strcmp(type.c_str(), "Rook") == 0) {
			return 7 + side;
		}
		else if (strcmp(type.c_str(), "Queen") == 0) {
			return 9 + side;
		}
		else if (strcmp(type.c_str(), "King") == 0) {
			return 11 + side;
		}
		else return 0;
	}

	static char getPieceCode(Piece piece) {
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

};