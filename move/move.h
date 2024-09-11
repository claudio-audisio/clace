#pragma once

#include <string>

#include "../board/piece.h"

using namespace std;

// TODO tutta questa classe puo' essere inglobata in un long long (a parte l'evaluation value)

class Move {
public:
	Move(const string& move, const bool white);
	Move(const Position sourcePosition, const Position destinationPosition, const bool white);
	~Move();

	void decorate(const Piece piece, const Position enPassantPosition, const bool isComputerToMove);
	string toString() const;

	Position getSourcePosition() const {
		return sourcePosition;
	}
	
	Position getDestinationPosition() const {
		return destinationPosition;
	}

	bool isWhite() const {
		return white;
	}

	Piece getPiece() const {
		return piece;
	}

	bool isCastling() const {
		return castling;
	}

	bool isEnPassant() const {
		return enPassant;
	}

	bool isPawnPromotion() const {
		return pawnPromotion;
	}

	void setCaptured(const bool captured) {
		this->captured = captured;
	}

	bool isCaptured() const {
		return captured;
	}

	Piece getPromotion() const {
		return promotion;
	}

	void setPromotion(const Piece promotion) {
		this->promotion = promotion;
	}

	bool isComputer() const {
		return computer;
	}

private:
	Position sourcePosition;
	Position destinationPosition;
	bool white;
	Piece piece;
	bool castling = false;
	bool enPassant = false;
	bool pawnPromotion = false;
	bool captured = false;
	Piece promotion = Empty;
	bool computer;
	double evaluationValue;

};
