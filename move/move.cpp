#include "move.h"
#include "../utils/positions.h"
#include "../common/constants.h"
#include "../game/game.h"
#include "../utils/pieceHelper.h"

Move::Move(const string& move, const bool white) {
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

Move::Move(const Position sourcePosition, const Position destinationPosition, const bool white) {
	this->sourcePosition = sourcePosition;
	this->destinationPosition = destinationPosition;
	this->white = white;
	this->computer = false;
	this->evaluationValue = DRAW_VALUE;
}

Move::~Move() {
}

void Move::decorate(const Piece piece, const Position enPassantPosition, const bool isComputerToMove) {
	this->piece = piece;
	const bool white = PieceHelper::isWhite(piece);

	// castling
	if (PieceHelper::isKing(this->piece)) {
		castling = (!white && sourcePosition == 4 && (destinationPosition == 2 || destinationPosition == 6)) ||
			(white && sourcePosition == 60 && (destinationPosition == 58 || destinationPosition == 62));
	}

	if (PieceHelper::isPawn(this->piece)) {
		// En passant
		enPassant = enPassantPosition == destinationPosition;
		// Pawn promotion
		pawnPromotion = Positions::isEighthRow(destinationPosition, white);
	}

	computer = isComputerToMove;
}

/*
public boolean isWinning() {
	return Optional.ofNullable(evaluationValue).map(v->v.equals(Constants.WIN_VALUE)).orElse(false);
}
*/

string Move::toString() const {
	return Positions::indexToCoords(sourcePosition) + "-" + Positions::indexToCoords(destinationPosition);
}

/*
public String toStringWithEvaluation() {
	return String.format(toString() + " --> " + new DecimalFormat("0.00").format(evaluationValue));
}

@Override
public boolean equals(Object obj) {
	if (obj instanceof Move) {
		return
			sourcePosition == ((Move)obj).sourcePosition &&
			destinationPosition == ((Move)obj).destinationPosition &&
			white == ((Move)obj).white &&
			pieceType == ((Move)obj).pieceType &&
			promotionType == ((Move)obj).promotionType;
	}

	return false;
}
*/