#pragma once

#include <list>
#include <unordered_set>
#include <string>
#include <cstring>

#include "../board/piece.h"

using namespace std;

class PieceHelper {
public:

	inline static const list<Piece> WHITE_PROMOTION_PIECES = list<Piece>{ WRook, WKnight, WBishop, WQueen };
	inline static const list<Piece> BLACK_PROMOTION_PIECES = list<Piece>{ BRook, BKnight, BBishop, BQueen };
	inline static const unordered_set<Piece> WHITE_XRAY_PIECES = unordered_set<Piece>{ WRook, WBishop, WQueen };
	inline static const unordered_set<Piece> BLACK_XRAY_PIECES = unordered_set<Piece>{ BRook, BBishop, BQueen };

	static bool isWhite(const Piece piece) {
		return piece % 2;
	}

	static bool isBlack(const Piece piece) {
		return piece != Empty && !isWhite(piece);
	}

	static bool isEmpty(const Piece piece) {
		return piece == Empty;
	}

	static bool isPawn(const Piece piece) {
		return piece == WPawn || piece == BPawn;
	}
	
	static bool isRook(const Piece piece) {
		return piece == WRook || piece == BRook;
	}

	static bool isKnight(const Piece piece) {
		return piece == WKnight || piece == BKnight;
	}

	static bool isBishop(const Piece piece) {
		return piece == WBishop || piece == BBishop;
	}

	static bool isQueen(const Piece piece) {
		return piece == WQueen || piece == BQueen;
	}
	
	static bool isKing(const Piece piece) {
		return piece == WKing || piece == BKing;
	}

	static bool isOpponent(const Piece piece, const bool white) {
		return !isEmpty(piece) && isWhite(piece) != white;
	}

	static bool isEmptyOrOpponent(const Piece piece, const bool white) {
		return isEmpty(piece) || isOpponent(piece, white);
	}

	static const list<Piece>& getPromotionTypes(const Side side) {
		return side ? BLACK_PROMOTION_PIECES : WHITE_PROMOTION_PIECES;
	}

	static const unordered_set<Piece>& getXRayTypes(const Side side) {
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

};