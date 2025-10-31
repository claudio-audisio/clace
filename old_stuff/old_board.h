#pragma once

#include <vector>
#include <bit>
#include <functional>

#include "../common/types.h"
#include "../common/constants.h"
#include "../common/bitwise.h"
#include "../utils/utils.h"
#include "../utils/pieceHelper.h"
#include "../movesCalculation/staticAttacks.h"
#include "../common/defines.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"
#include "../move/move.h"

using namespace std;


class old_Board {
public:
    old_Board() {
	    reset();
    }
    ~old_Board() = default;

	Rawboard pieceBoards[SIZE];
	Piece piecePositions[64];
	Rawboard& EMPTY = pieceBoards[Empty];
	CastlingInfo castlingInfo;
	Position enPassantPosition;
	Rawboard _OPP_PIECES[2];
	Rawboard _PIECES[2];


	Rawboard PIECES(const Side side) const {
		// TODO capire quanto questo venga effettivamente usate (per esempio con perft 6)
#ifdef BOARD_USE_PRE_CALCULATED
		if (!_PIECES[side]) {
			_PIECES[side] = pieceBoards[WPawn + side] |
				   pieceBoards[WBishop + side] |
				   pieceBoards[WQueen + side] |
				   pieceBoards[WKnight + side] |
				   pieceBoards[WKing + side] |
				   pieceBoards[WRook + side];
		}

		return _PIECES[side];
#else
		// Usare per test di performance sui metodi della board
		return pieceBoards[WPawn + side] |
			   pieceBoards[WBishop + side] |
			   pieceBoards[WQueen + side] |
			   pieceBoards[WKnight + side] |
			   pieceBoards[WKing + side] |
			   pieceBoards[WRook + side];
#endif
	}

	Rawboard OPP_PIECES(const Side side) const {
#ifdef BOARD_USE_PRE_CALCULATED
		if (!_OPP_PIECES[side]) {
			_OPP_PIECES[side] = pieceBoards[BPawn - side] |
					pieceBoards[BBishop - side] |
					pieceBoards[BQueen - side] |
					pieceBoards[BKnight - side] |
					pieceBoards[BKing - side] |
					pieceBoards[BRook - side];
		}

		return _OPP_PIECES[side];
#else
		// Usare per test di performance sui metodi della board
		return pieceBoards[BPawn - side] |
			   pieceBoards[BBishop - side] |
			   pieceBoards[BQueen - side] |
			   pieceBoards[BKnight - side] |
			   pieceBoards[BKing - side] |
			   pieceBoards[BRook - side];
#endif
	}

#ifdef BOARD_USE_PRE_CALCULATED
	void resetCalculated() {
		resetCalculated(_WHITE);
		resetCalculated(_BLACK);
	}

	void resetCalculated(Side side) {
		_PIECES[side] = 0;
		_OPP_PIECES[side] = 0;
	}
#endif

	void reset() {
		for (RawboardIndex i = 1; i < SIZE; ++i) {
			pieceBoards[i] = 0;
		}

		pieceBoards[Empty] = EMPTY_BOARD;

		for (Piece& piece : piecePositions) {
			piece = Empty;
		}

		castlingInfo = 0;
		enPassantPosition = NO_POS;
#ifdef BOARD_USE_PRE_CALCULATED
		resetCalculated();
#endif
	}

	bool equals(const old_Board& board) const {
		for (RawboardIndex i = 1; i < SIZE; ++i) {
			if (pieceBoards[i] != board.pieceBoards[i]) {
				return false;
			}
		}

		for (Position i = 0; i < 64; i++) {
			if (piecePositions[i] != board.piecePositions[i]) {
				return false;
			}
		}

		return true;
	}

	void set(const old_Board& board) {
		for (RawboardIndex i = 0; i < SIZE; ++i) {
			pieceBoards[i] = board.pieceBoards[i];
		}

		for (int i = 0; i < 64; i++) {
			piecePositions[i] = board.piecePositions[i];
		}

		castlingInfo = board.castlingInfo;
		enPassantPosition = board.enPassantPosition;

#ifdef BOARD_USE_PRE_CALCULATED
		resetCalculated();
#endif
	}

	unsigned int getPieceCount(const Piece piece) const {
		return popcount(pieceBoards[piece]);
	}

	Side getSide(const Position position) const {
		return _getSide(piecePositions[position]);
	}

	bool isEmpty(const Position position) const {
		return EMPTY & posInd(position);
	}

	bool isWhite(const Position position) const {
		return _isWhite(piecePositions[position]);
	}

	bool isBlack(const Position position) const {
		return _isBlack(piecePositions[position]);
	}

	bool isPawn(const Position position) const {
		return (pieceBoards[WPawn] | pieceBoards[BPawn]) & posInd(position);
	}

	bool isKnight(const Position position) const {
		return (pieceBoards[WKnight] | pieceBoards[BKnight]) & posInd(position);
	}

	bool isBishop(const Position position) const {
		return (pieceBoards[WBishop] | pieceBoards[BBishop]) & posInd(position);
	}

	bool isRook(const Position position) const {
		return (pieceBoards[WRook] | pieceBoards[BRook]) & posInd(position);
	}

	bool isRook(const Position position, const Side side) const {
		return pieceBoards[WRook + side] & posInd(position);
	}

	bool isQueen(const Position position) const {
		return (pieceBoards[WQueen] | pieceBoards[BQueen]) & posInd(position);
	}

	bool isKing(const Position position) const {
		return (pieceBoards[WKing] | pieceBoards[BKing]) & posInd(position);
	}

	Position getWhiteKingPosition() const {
		return getFirstPos(pieceBoards[WKing]);
	}

	Position getBlackKingPosition() const {
		return getFirstPos(pieceBoards[BKing]);
	}

	Piece getPiece(const Position position) const {
		return piecePositions[position];
	}

	Piece setPiece(const Position position, const Piece piece) {
		const Piece oldPiece = getPiece(position);
		const Rawboard posIndex = posInd(position);
		pieceBoards[oldPiece] &= ~posIndex;
		pieceBoards[piece] |= posIndex;
		piecePositions[position] = piece;
#ifdef BOARD_USE_PRE_CALCULATED
		resetCalculated();
#endif
		return oldPiece;
	}

	void setEmpty(const Position position, const Piece oldPiece) {
		const Rawboard posIndex = posInd(position);
		pieceBoards[oldPiece] &= ~posIndex;
		pieceBoards[Empty] |= posIndex;
		piecePositions[position] = Empty;
	}

	Piece setEmpty(const Position position) {
		const Piece oldPiece = getPiece(position);
		setEmpty(position, oldPiece);
		return oldPiece;
	}

	Piece simulateMove(const Position source, const Position destination, const Piece piece, const bool isCastling) {
		if (isCastling) {
			castlingMove(source, destination);
#ifdef BOARD_USE_PRE_CALCULATED
			resetCalculated();
#endif
			return Empty;
		}

		const Piece oldPiece = setPiece(destination, piece);
		setEmpty(source, piece);

		return oldPiece;
	}

	Piece move(const Position source, const Position destination, Piece piece, const MoveType type = NORMAL, const Piece promotionPiece = Empty) {
		switch (type) {
		case NORMAL: {
				const Piece oldPiece = setPiece(destination, piece);
				setEmpty(source, piece);
				updateEnPassantInfo(source, destination, piece);
				updateCastlingInfo(source, destination);
				return oldPiece;
				}
		case CASTLING: {
				enPassantPosition = NO_POS;
				castlingMove(source, destination);
				updateCastlingInfo(source, destination);
#ifdef BOARD_USE_PRE_CALCULATED
				resetCalculated();
#endif
				return Empty;
		}
		case EN_PASSANT: {
				enPassantPosition = NO_POS;
				setPiece(destination, piece);
				setEmpty(source, piece);
				return setEmpty(destination + (piece == WPawn ? 8 : -8));
		}
		case PROMOTION: {
				if (promotionPiece == Empty) { throw runtime_error("promotion piece not set"); }
				enPassantPosition = NO_POS;
				const Piece oldPiece = setPiece(destination, piece);
				setPiece(destination, promotionPiece);
				setEmpty(source, piece);
				updateCastlingInfo(source, destination);
				return oldPiece;
		}
		default: throw runtime_error("unexpected move type");
		}
	}

	void updateCastlingInfo(const Position source, const Position destination) {
		castlingInfo &= CASTLING_INFO_MASK[source];
		castlingInfo &= CASTLING_INFO_MASK[destination];
	}

	void updateEnPassantInfo(const Position source, const Position destination, const Piece piece) {
		const Side side = _getSide(piece);

		if (_isPawn(piece) &&
			isSecondRow(source, side) &&
			isFourthRow(destination, side)) {
			enPassantPosition = source - 8 + (16 * side);
			}
		else {
			enPassantPosition = NO_POS;
		}
	}

	Position getKingPosition(const Side side) const {
		return getFirstPos(pieceBoards[WKing + side]);
	}

	void castlingMove(const Position source, const Position destination) {
		switch (source + destination) {
			case BQCastling: {
				pieceBoards[BKing] = BQC_King;
				pieceBoards[BRook] &= BQC_RookRem;
				pieceBoards[BRook] |= BQC_RookAdd;
				pieceBoards[Empty] &= BQC_EmptyRem;
				pieceBoards[Empty] |= BQC_EmptyAdd;
				piecePositions[0] = Empty;
				piecePositions[2] = BKing;
				piecePositions[3] = BRook;
				piecePositions[4] = Empty;
				break;
			}
			case BKCastling: {
				pieceBoards[BKing] = BKC_King;
				pieceBoards[BRook] &= BKC_RookRem;
				pieceBoards[BRook] |= BKC_RookAdd;
				pieceBoards[Empty] &= BKC_EmptyRem;
				pieceBoards[Empty] |= BKC_EmptyAdd;
				piecePositions[4] = Empty;
				piecePositions[6] = BKing;
				piecePositions[5] = BRook;
				piecePositions[7] = Empty;
				break;
			}
			case WQCastling: {
				pieceBoards[WKing] = WQC_King;
				pieceBoards[WRook] &= WQC_RookRem;
				pieceBoards[WRook] |= WQC_RookAdd;
				pieceBoards[Empty] &= WQC_EmptyRem;
				pieceBoards[Empty] |= WQC_EmptyAdd;
				piecePositions[56] = Empty;
				piecePositions[58] = WKing;
				piecePositions[59] = WRook;
				piecePositions[60] = Empty;
				break;
			}
			case WKCastling: {
				pieceBoards[WKing] = WKC_King;
				pieceBoards[WRook] &= WKC_RookRem;
				pieceBoards[WRook] |= WKC_RookAdd;
				pieceBoards[Empty] &= WKC_EmptyRem;
				pieceBoards[Empty] |= WKC_EmptyAdd;
				piecePositions[60] = Empty;
				piecePositions[62] = WKing;
				piecePositions[61] = WRook;
				piecePositions[63] = Empty;
				break;
			}
			default: throw runtime_error("wrong castling move");
		}
	}

    void undoCastlingMove(const Position source, const Position destination) {
		switch (source + destination) {
			case BQCastling: {
				pieceBoards[BKing] = BC_King;
				pieceBoards[BRook] |= ~BQC_RookRem;
				pieceBoards[BRook] &= ~BQC_RookAdd;
				pieceBoards[Empty] |= ~BQC_EmptyRem;
				pieceBoards[Empty] &= ~BQC_EmptyAdd;
				piecePositions[2] = Empty;
				piecePositions[4] = BKing;
				piecePositions[0] = BRook;
				piecePositions[3] = Empty;
				break;
			}
			case BKCastling: {
				pieceBoards[BKing] = BC_King;
				pieceBoards[BRook] |= ~BKC_RookRem;
				pieceBoards[BRook] &= ~BKC_RookAdd;
				pieceBoards[Empty] |= ~BKC_EmptyRem;
				pieceBoards[Empty] &= ~BKC_EmptyAdd;
				piecePositions[5] = Empty;
				piecePositions[7] = BRook;
				piecePositions[4] = BKing;
				piecePositions[6] = Empty;
				break;
			}
			case WQCastling: {
				pieceBoards[WKing] = WC_King;
				pieceBoards[WRook] |= ~WQC_RookRem;
				pieceBoards[WRook] &= ~WQC_RookAdd;
				pieceBoards[Empty] |= ~WQC_EmptyRem;
				pieceBoards[Empty] &= ~WQC_EmptyAdd;
				piecePositions[58] = Empty;
				piecePositions[60] = WKing;
				piecePositions[56] = WRook;
				piecePositions[59] = Empty;
				break;
			}
			case WKCastling: {
				pieceBoards[WKing] = WC_King;
				pieceBoards[WRook] |= ~WKC_RookRem;
				pieceBoards[WRook] &= ~WKC_RookAdd;
				pieceBoards[Empty] |= ~WKC_EmptyRem;
				pieceBoards[Empty] &= ~WKC_EmptyAdd;
				piecePositions[61] = Empty;
				piecePositions[63] = WRook;
				piecePositions[60] = WKing;
				piecePositions[62] = Empty;
				break;
			}
		default: throw runtime_error("wrong undo castling move");
		}
	}

};

