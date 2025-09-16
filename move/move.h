#pragma once

#include <string>

#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;


// Position sourcePosition;			0 - 7
// Position destinationPosition;	8 - 15
// Piece piece;						16 - 23
// Piece promotion = Empty;			24 - 31
// Piece captured;                  32 - 39
// Side side;						40
// bool castling = false;			41
// bool enPassant = false;			42
// bool pawnPromotion = false;		43

static void setSourcePosition(Move& move, const Position sourcePosition) {
	move &= ~SOURCE_POS_MASK;
	move |= sourcePosition;
}

static Position getSourcePosition(const Move move) {
	return move & SOURCE_POS_MASK;
}

static void setDestinationPosition(Move& move, const Position destinationPosition) {
	move &= ~DEST_POS_MASK;
	move |= (Move)destinationPosition << 8;
}

static Position getDestinationPosition(const Move move) {
	return static_cast<Position>((move & DEST_POS_MASK) >> 8);
}

static void setPiece(Move& move, const Piece piece) {
	move &= ~PIECE_MASK;
	move |= (Move)piece << 16;
}

static Piece getPiece(const Move move) {
	return static_cast<Piece>((move & PIECE_MASK) >> 16);
}

static void setPromotion(Move& move, const Piece promotion) {
	move &= ~PROMOTION_MASK;
	move |= (Move)promotion << 24;
}

static Piece getPromotion(const Move move) {
	return static_cast<Piece>((move & PROMOTION_MASK) >> 24);
}

static void setCaptured(Move& move, const Piece captured) {
    move &= ~CAPTURED_MASK;
    move |= (Move)captured << 32;
}

static Piece getCaptured(const Move move) {
    return static_cast<Piece>((move & CAPTURED_MASK) >> 32);
}

static bool isCaptured(const Move move) {
    return getCaptured(move) != Empty;
}

static void setMoveSide(Move& move, const Side side) {
	move &= ~SIDE_MASK;
	move |= (Move)side << 40;
}

static Side getMoveSide(const Move move) {
	return static_cast<Side>((move & SIDE_MASK) >> 40);
}

static bool isWhite(const Move move) {
	return getMoveSide(move) == WHITE;
}

static void setCastling(Move& move, const bool castling) {
	if (castling) {
		move |= CASTLING_MASK;
	}
	else {
		move &= ~CASTLING_MASK;
	}
}

static bool isCastling(const Move move) {
	return (move & CASTLING_MASK) >> 41;
}

static void setEnPassant(Move& move, const bool enPassant) {
	if (enPassant) {
		move |= EN_PASSANT_MASK;
	}
	else {
		move &= ~EN_PASSANT_MASK;
	}
}

static bool isEnPassant(const Move move) {
	return (move & EN_PASSANT_MASK) >> 42;
}

static void setPawnPromotion(Move& move, const bool pawnPromotion) {
	if (pawnPromotion) {
		move |= PAWN_PROM_MASK;
	}
	else {
		move &= ~PAWN_PROM_MASK;
	}
}

static bool isPawnPromotion(const Move move) {
	return (move & PAWN_PROM_MASK) >> 43;
}

static MoveType getType(const Move move) {
	return (move & MOVE_TYPE_MASK) >> 41;
}

static Move createMove(const Position sourcePosition, const Position destinationPosition, const Side side) {
	Move move = 0;
	setSourcePosition(move, sourcePosition);
	setDestinationPosition(move, destinationPosition);
	setMoveSide(move, side);
	return move;
}

static Move createMove(const string& move, const Side side) {
	return createMove(coordsToIndex(move.substr(0, 2)), coordsToIndex(move.substr(2, 4)), side);
}

static Move createMove(const Position sourcePosition, const Position destinationPosition, const Side side, const Piece piece, const Position enPassantPosition) {
	Move move = 0;
	setSourcePosition(move, sourcePosition);
	setDestinationPosition(move, destinationPosition);
	setMoveSide(move, side);
	setPiece(move, piece);

	if (_isKing(piece)) {
		// castling
		setCastling(move,
					(side == BLACK && sourcePosition == 4 && (destinationPosition == 2 || destinationPosition == 6)) ||
					(side == WHITE && sourcePosition == 60 && (destinationPosition == 58 || destinationPosition == 62)));
	} else if (_isPawn(piece)) {
		// En passant
		setEnPassant(move, enPassantPosition == destinationPosition);
		// Pawn promotion
		setPawnPromotion(move, isEighthRow(destinationPosition, side));
	}

	return move;
}

static void decorate(Move& move, const Piece piece, const Position enPassantPosition) {
	const Position sourcePosition = getSourcePosition(move);
	const Position destinationPosition = getDestinationPosition(move);
	const Side side = getMoveSide(move);

	setPiece(move, piece);

	if (_isKing(piece)) {
		// castling
		setCastling(move,
					(side == BLACK && sourcePosition == 4 && (destinationPosition == 2 || destinationPosition == 6)) ||
					(side == WHITE && sourcePosition == 60 && (destinationPosition == 58 || destinationPosition == 62)));
	} else if (_isPawn(piece)) {
		// En passant
		setEnPassant(move, enPassantPosition == destinationPosition);
		// Pawn promotion
		setPawnPromotion(move, isEighthRow(destinationPosition, side));
	}
}

static Move createMove(const Position sourcePosition, const Position destinationPosition, const Side side, const Piece piece) {
	return createMove(sourcePosition, destinationPosition, side, piece, NO_POS);
}

static string toString(const Move& move) {
	return indexToCoords(getSourcePosition(move)) + indexToCoords(getDestinationPosition(move));
}

static void setCapturedMR(MoveResult& moveResult, const bool captured) {
    if (captured) {
        moveResult |= MR_CAPTURED_MASK;
    }
    else {
        moveResult &= ~MR_CAPTURED_MASK;
    }
}

static bool isCapturedMR(const MoveResult moveResult) {
    return (moveResult & MR_CAPTURED_MASK);
}

static void setPromotedMR(MoveResult& moveResult, const bool promoted) {
    if (promoted) {
        moveResult |= MR_PROMOTED_MASK;
    }
    else {
        moveResult &= ~MR_PROMOTED_MASK;
    }
}

static bool isPromotedMR(const MoveResult moveResult) {
    return (moveResult & MR_PROMOTED_MASK);
}

static void setEnPassantMR(MoveResult& moveResult, const bool enPassant) {
    if (enPassant) {
        moveResult |= MR_PASSANT_MASK;
    }
    else {
        moveResult &= ~MR_PASSANT_MASK;
    }
}

static bool isEnPassantMR(const MoveResult moveResult) {
    return (moveResult & MR_PASSANT_MASK);
}

static void setCastlingMR(MoveResult& moveResult, const bool castling) {
    if (castling) {
        moveResult |= MR_CASTLING_MASK;
    }
    else {
        moveResult &= ~MR_CASTLING_MASK;
    }
}

static bool isCastlingMR(const MoveResult moveResult) {
    return (moveResult & MR_CASTLING_MASK);
}

static MoveResult getMoveResult(const bool captured, const MoveType moveType) {
	MoveResult moveResult = 0;
	setCapturedMR(moveResult, captured);
	// TODO posso buttare direttamente il valore di Game type se i bit fossero ordinati nello stesso modo
	setPromotedMR(moveResult, moveType == PROMOTION);
	setEnPassantMR(moveResult, moveType == EN_PASSANT);
	setCastlingMR(moveResult, moveType == CASTLING);
	return moveResult;
}

static bool isPresent(const Move move, const vector<Move>& moves) {
	const Position source = getSourcePosition(move);
	const Position destination  = getDestinationPosition(move);

	for (Move m : moves) {
		if (getSourcePosition(m) == source && getDestinationPosition(m) == destination) {
			return true;
		}
	}

	return false;
}

static bool isPresent(const Move move, const Move* moves, unsigned char amount) {
	const Position source = getSourcePosition(move);
	const Position destination  = getDestinationPosition(move);

	for (int i = 0; i < amount; i++) {
		if (moves[i] && getSourcePosition(moves[i]) == source && getDestinationPosition(moves[i]) == destination) {
			return true;
		}
	}

	return false;
}

static Move parseUciMove(string& uciMove) {
	if (uciMove.length() < 4 || uciMove.length() > 5) {
		throw runtime_error("uci move malformed");
	}

	Move move = createMove(coordsToIndex(uciMove.substr(0, 2)), coordsToIndex(uciMove.substr(2, 4)), WHITE);

	if (uciMove.length() == 5) {
		const Piece piece = FEN_TO_PIECE.at(uciMove.at(4)) - SIDE_GAP;
		setPromotion(move, piece);
	}

	return move;
}

static string toUciMove(Move move) {
	string uciMove = indexToCoords(getSourcePosition(move)) + indexToCoords(getDestinationPosition(move));

	if (isPawnPromotion(move)) {
		Piece piece = getPromotion(move);

		if (isWhite(piece)) {
			piece = piece + SIDE_GAP;
		}

		uciMove.append(PIECE_TO_FEN[piece]);
	}

	return uciMove;
}

