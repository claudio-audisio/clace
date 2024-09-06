#pragma once

#include <list>

#include "../board/piece.h"

using namespace std;

class PieceHelper {
public:

	static bool isWhite(const Piece piece) {
		return piece < BPawn && piece != Empty;
	}

	static bool isBlack(const Piece piece) {
		return piece > WKing;
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

	static list<Piece>* getPromotionTypes(const bool white) {
		return white ?
			new list<Piece>{ WRook, WKnight, WBishop, WQueen } :
			new list<Piece>{ BRook, BKnight, BBishop, BQueen };
	}

	static list<Piece>* getXRayTypes(const bool white) {
		return white ?
			new list<Piece>{ WRook, WBishop, WQueen } :
			new list<Piece>{ BRook, BBishop, BQueen };
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

	/*	TODO
	static Piece valueOf(const string& stringPiece) {
		switch (stringPiece) {
			case "WPawn" -> 1;
			case "WKnight" -> 2;
			case "WBishop" -> 3;
			case "WRook" -> 4;
			case "WQueen" -> 5;
			case "WKing" -> 6;
			case "BPawn" -> 7;
			case "BKnight" -> 8;
			case "BBishop" -> 9;
			case "BRook" -> 10;
			case "BQueen" -> 11;
			case "BKing" -> 12;
			default -> 0;
		};
	}
	*/

};