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

inline void setSourcePosition(Move& move, const Position sourcePosition) {
	move = (move & ~SOURCE_POS_MASK) | sourcePosition;
}

inline Position getSourcePosition(const Move move) {
	return move & SOURCE_POS_MASK;
}

inline void setDestinationPosition(Move& move, const Position destinationPosition) {
	move = (move & ~DEST_POS_MASK) | (static_cast<Move>(destinationPosition) << 8);
}

inline Position getDestinationPosition(const Move move) {
	return static_cast<Position>((move & DEST_POS_MASK) >> 8);
}

inline void setPiece(Move& move, const Piece piece) {
	move = (move & ~PIECE_MASK) | (static_cast<Move>(piece) << 16);
}

inline Piece getPiece(const Move move) {
	return static_cast<Piece>((move & PIECE_MASK) >> 16);
}

inline void setPromotion(Move& move, const Piece promotion) {
	move = (move & ~PROMOTION_MASK) | (static_cast<Move>(promotion) << 24);
}

inline Piece getPromotion(const Move move) {
	return static_cast<Piece>((move & PROMOTION_MASK) >> 24);
}

inline void setCaptured(Move& move, const Piece captured) {
	move = (move & ~CAPTURED_MASK) | (static_cast<Move>(captured) << 32);
}

inline Piece getCaptured(const Move move) {
    return static_cast<Piece>((move & CAPTURED_MASK) >> 32);
}

inline bool isCaptured(const Move move) {
    return getCaptured(move) != Empty;
}

inline void setMoveSide(Move& move, const Side side) {
	move = (move & ~SIDE_MASK) | (static_cast<Move>(side) << 40);
}

inline Side getMoveSide(const Move move) {
	return static_cast<Side>((move & SIDE_MASK) >> 40);
}

inline bool isWhite(const Move move) {
	return getMoveSide(move) == _WHITE;
}

inline void setCastling(Move& move, const bool castling) {
	move = (move & ~CASTLING_MASK) | (-castling & CASTLING_MASK);
}

inline bool isCastling(const Move move) {
	return (move & CASTLING_MASK) >> 41;
}

inline void setEnPassant(Move& move, const bool enPassant) {
	move = (move & ~EN_PASSANT_MASK) | (-enPassant & EN_PASSANT_MASK);
}

inline bool isEnPassant(const Move move) {
	return (move & EN_PASSANT_MASK) >> 42;
}

inline void setPawnPromotion(Move& move, const bool pawnPromotion) {
	move = (move & ~PAWN_PROM_MASK) | (-pawnPromotion & PAWN_PROM_MASK);
}

inline bool isPawnPromotion(const Move move) {
	return (move & PAWN_PROM_MASK) >> 43;
}

inline MoveType getType(const Move move) {
	return (move & MOVE_TYPE_MASK) >> 41;
}

inline Move createMove(const Position sourcePosition, const Position destinationPosition, const Side side) {
	Move move = 0;
	setSourcePosition(move, sourcePosition);
	setDestinationPosition(move, destinationPosition);
	setMoveSide(move, side);
	return move;
}

inline Move createMove(const string& move, const Side side) {
	return createMove(coordsToIndex(move.substr(0, 2)), coordsToIndex(move.substr(2, 4)), side);
}

inline Move createMove(const Position sourcePosition, const Position destinationPosition, const Side side, const Piece piece, const Position enPassantPosition) {
	Move move = 0;
	setSourcePosition(move, sourcePosition);
	setDestinationPosition(move, destinationPosition);
	setMoveSide(move, side);
	setPiece(move, piece);

	if (_isPawn(piece)) {
		// En passant
		setEnPassant(move, enPassantPosition == destinationPosition);
		// Pawn promotion
		setPawnPromotion(move, isEighthRow(destinationPosition, side));
	}
	else if (_isKing(piece)) {
		// castling
		setCastling(move,
					(side == _BLACK && sourcePosition == 4 && (destinationPosition == 2 || destinationPosition == 6)) ||
					(side == _WHITE && sourcePosition == 60 && (destinationPosition == 58 || destinationPosition == 62)));
	}

	return move;
}

inline void decorate(Move& move, const Piece piece, const Position enPassantPosition) {
	const Position sourcePosition = getSourcePosition(move);
	const Position destinationPosition = getDestinationPosition(move);
	const Side side = getMoveSide(move);

	setPiece(move, piece);

	if (_isPawn(piece)) {
		// En passant
		setEnPassant(move, enPassantPosition == destinationPosition);
		// Pawn promotion
		setPawnPromotion(move, isEighthRow(destinationPosition, side));
	} else if (_isKing(piece)) {
		// castling
		setCastling(move,
					(side == _BLACK && sourcePosition == 4 && (destinationPosition == 2 || destinationPosition == 6)) ||
					(side == _WHITE && sourcePosition == 60 && (destinationPosition == 58 || destinationPosition == 62)));
	}
}

inline Move createMove(const Position sourcePosition, const Position destinationPosition, const Side side, const Piece piece) {
	return createMove(sourcePosition, destinationPosition, side, piece, NO_POS);
}

inline void setCapturedMR(MoveResult& moveResult, const bool captured) {
    if (captured) {
        moveResult |= MR_CAPTURED_MASK;
    }
    else {
        moveResult &= ~MR_CAPTURED_MASK;
    }
}

inline bool isCapturedMR(const MoveResult moveResult) {
    return (moveResult & MR_CAPTURED_MASK);
}

inline void setPromotedMR(MoveResult& moveResult, const bool promoted) {
    if (promoted) {
        moveResult |= MR_PROMOTED_MASK;
    }
    else {
        moveResult &= ~MR_PROMOTED_MASK;
    }
}

inline bool isPromotedMR(const MoveResult moveResult) {
    return (moveResult & MR_PROMOTED_MASK);
}

inline void setEnPassantMR(MoveResult& moveResult, const bool enPassant) {
    if (enPassant) {
        moveResult |= MR_PASSANT_MASK;
    }
    else {
        moveResult &= ~MR_PASSANT_MASK;
    }
}

inline bool isEnPassantMR(const MoveResult moveResult) {
    return (moveResult & MR_PASSANT_MASK);
}

inline void setCastlingMR(MoveResult& moveResult, const bool castling) {
    if (castling) {
        moveResult |= MR_CASTLING_MASK;
    }
    else {
        moveResult &= ~MR_CASTLING_MASK;
    }
}

inline bool isCastlingMR(const MoveResult moveResult) {
    return (moveResult & MR_CASTLING_MASK);
}

inline MoveResult getMoveResult(const bool captured, const MoveType moveType) {
	MoveResult moveResult = 0;
	setCapturedMR(moveResult, captured);
	// TODO posso buttare direttamente il valore di Game type se i bit fossero ordinati nello stesso modo
	setPromotedMR(moveResult, moveType == PROMOTION);
	setEnPassantMR(moveResult, moveType == EN_PASSANT);
	setCastlingMR(moveResult, moveType == CASTLING);
	return moveResult;
}

inline bool isPresent(const Move move, const vector<Move>& moves) {
	const Position source = getSourcePosition(move);
	const Position destination  = getDestinationPosition(move);

	for (Move m : moves) {
		if (getSourcePosition(m) == source && getDestinationPosition(m) == destination) {
			return true;
		}
	}

	return false;
}

inline bool isPresent(const Move move, const Move* moves, unsigned char amount) {
	const Position source = getSourcePosition(move);
	const Position destination  = getDestinationPosition(move);

	for (int i = 0; i < amount; i++) {
		if (moves[i] && getSourcePosition(moves[i]) == source && getDestinationPosition(moves[i]) == destination) {
			return true;
		}
	}

	return false;
}

inline Move parseUciMove(string& uciMove) {
	if (uciMove.length() < 4 || uciMove.length() > 5) {
		throw runtime_error("uci move malformed");
	}

	Move move = createMove(coordsToIndex(uciMove.substr(0, 2)), coordsToIndex(uciMove.substr(2, 4)), _WHITE);

	if (uciMove.length() == 5) {
		const Piece piece = FEN_TO_PIECE.at(uciMove.at(4)) - 1;
		setPromotion(move, piece);
	}

	return move;
}

inline string toUciMove(Move move) {
	string uciMove = indexToCoords(getSourcePosition(move)) + indexToCoords(getDestinationPosition(move));

	if (isPawnPromotion(move)) {
		Piece piece = getPromotion(move);

		if (isWhite(piece)) {
			piece = piece + 1;
		}

		uciMove.append(PIECE_TO_FEN[piece]);
	}

	return uciMove;
}

