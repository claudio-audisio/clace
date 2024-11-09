#pragma once

#include <vector>
#include <bit>

#include "../common/types.h"
#include "../common/constants.h"
//#include "../common/bitwise.h"
#include "piece.h"
#include "../utils/utils.h"
#include "../utils/pieceHelper.h"

using namespace std;

extern Rawboard rayAttacks[8][64];
extern Rawboard knightAttacks[64];
extern Rawboard kingAttacks[64];

class Board {
public:
    Board();
    ~Board();

	Rawboard pieceBoards[SIZE];
	Piece piecePositions[64];
	Rawboard& EMPTY = pieceBoards[Empty];
	CastlingInfo castlingInfo;
	Position enPassantPosition;
	Rawboard _OPP_PIECES[2];
	Rawboard _PIECES[2];

	inline Rawboard PIECES(const Side side) {
		if (!_PIECES[side]) {
			_PIECES[side] = pieceBoards[WPawn + side] |
				   pieceBoards[WBishop + side] |
				   pieceBoards[WQueen + side] |
				   pieceBoards[WKnight + side] |
				   pieceBoards[WKing + side] |
				   pieceBoards[WRook + side];
		}

		return _PIECES[side];

		// Usare per test di performance sui metodi della board
		/*return pieceBoards[WPawn + side] |
			   pieceBoards[WBishop + side] |
			   pieceBoards[WQueen + side] |
			   pieceBoards[WKnight + side] |
			   pieceBoards[WKing + side] |
			   pieceBoards[WRook + side];*/
	}

	inline Rawboard OPP_PIECES(const Side side) {
		if (!_OPP_PIECES[side]) {
			_OPP_PIECES[side] = pieceBoards[BPawn - side] |
					pieceBoards[BBishop - side] |
					pieceBoards[BQueen - side] |
					pieceBoards[BKnight - side] |
					pieceBoards[BKing - side] |
					pieceBoards[BRook - side];
		}

		return _OPP_PIECES[side];

		// Usare per test di performance sui metodi della board
		/*return pieceBoards[BPawn - side] |
			   pieceBoards[BBishop - side] |
			   pieceBoards[BQueen - side] |
			   pieceBoards[BKnight - side] |
			   pieceBoards[BKing - side] |
			   pieceBoards[BRook - side];*/
	}

	void resetCalculated() {
		resetCalculated(WHITE);
		resetCalculated(BLACK);
	}

	void resetCalculated(Side side) {
		_PIECES[side] = 0;
		_OPP_PIECES[side] = 0;
	}

    void reset();
	bool equals(const Board& board);
	void set(const Board& board);

	inline bool isEmpty(const Position position) const {
		return (EMPTY & posInd(position)) != 0L;
	}

	inline bool isWhite(const Position position) const {
		return PieceHelper::isWhite(piecePositions[position]);
	}

	inline bool isBlack(const Position position) const {
		return PieceHelper::isBlack(piecePositions[position]);
	}

	inline bool isPawn(const Position position) const {
		return ((pieceBoards[WPawn] | pieceBoards[BPawn]) & posInd(position)) != 0L;
	}

	inline bool isKnight(const Position position) const {
		return ((pieceBoards[WKnight] | pieceBoards[BKnight]) & posInd(position)) != 0L;
	}

	inline bool isBishop(const Position position) const {
		return ((pieceBoards[WBishop] | pieceBoards[BBishop]) & posInd(position)) != 0L;
	}

	inline bool isRook(const Position position) const {
		return ((pieceBoards[WRook] | pieceBoards[BRook]) & posInd(position)) != 0L;
	}

	inline bool isRook(const Position position, const Side side) const {
		return (pieceBoards[WRook + side] & posInd(position)) != 0;
	}

	inline bool isQueen(const Position position) const {
		return ((pieceBoards[WQueen] | pieceBoards[BQueen]) & posInd(position)) != 0L;
	}

	inline bool isKing(const Position position) const {
		return ((pieceBoards[WKing] | pieceBoards[BKing]) & posInd(position)) != 0L;
	}

	inline Position getWhiteKingPosition() const {
		return Utils::getFirstPos(pieceBoards[WKing]);
	}

	inline Position getBlackKingPosition() const {
		return Utils::getFirstPos(pieceBoards[BKing]);
	}

	inline Piece getPiece(const Position position) const {
		return piecePositions[position];
	}

	inline Piece setPiece(const Position position, const Piece piece) {
		const Piece oldPiece = getPiece(position);
		const Rawboard posIndex = posInd(position);
		pieceBoards[oldPiece] &= ~posIndex;
		pieceBoards[piece] |= posIndex;
		piecePositions[position] = piece;
		resetCalculated();
		return oldPiece;
	}

	inline void setEmpty(const Position position, const Piece oldPiece) {
		const Rawboard posIndex = posInd(position);
		pieceBoards[oldPiece] &= ~posIndex;
		pieceBoards[Empty] |= posIndex;
		piecePositions[position] = Empty;
	}

	inline Piece setEmpty(const Position position) {
		const Piece oldPiece = getPiece(position);
		setEmpty(position, oldPiece);
		return oldPiece;
	}

	inline Piece move(const Position source, const Position destination, Piece piece) {
		return move(source, destination, piece, false);
	}

	inline Piece move(const Position source, const Position destination, Piece piece, const bool isCastling) {
		if (isCastling) {
			castlingMove(source, destination);
			resetCalculated();
			return Empty;
		}

		const Piece oldPiece = setPiece(destination, piece);
		setEmpty(source, piece);
		return oldPiece;
	}

	void updateCastlingInfo(const Position source, const Position destination) {
		castlingInfo &= CASTLING_MASK[source];
		castlingInfo &= CASTLING_MASK[destination];
	}

	inline Position getKingPosition(const Side side) {
		return Utils::getFirstPos(pieceBoards[WKing + side]);
	}

	Rawboard getDestinationPositions(const Position position) {
		return getDestinationPositions(position, piecePositions[position]);
	}

	Rawboard getDestinationPositions(const Position position, const Piece piece) {
		switch (piece) {
			case WPawn: return getPawnMoves(position, WHITE);
			case BPawn: return getPawnMoves(position, BLACK);
			case WRook: return rookAttack(position, ~EMPTY, ~PIECES(WHITE));
			case BRook: return rookAttack(position, ~EMPTY, ~PIECES(BLACK));
			case WKnight: return knightAttack(position, OPP_PIECES(WHITE));
			case BKnight: return knightAttack(position, OPP_PIECES(BLACK));
			case WBishop: return bishopAttack(position, ~EMPTY, ~PIECES(WHITE));
			case BBishop: return bishopAttack(position, ~EMPTY, ~PIECES(BLACK));
			case WQueen: return queenAttacks(position, ~EMPTY, ~PIECES(WHITE));
			case BQueen: return queenAttacks(position, ~EMPTY, ~PIECES(BLACK));
			case WKing: return getKingMoves(position, WHITE);
			case BKing: return getKingMoves(position, BLACK);
			default: return 0;
		};
	}

	inline Rawboard getAttacks(const Side side) {
		return getPawnAttacks(side) | getKnightAttacks(side) | getBishopAttacks(side) | getRookAttacks(side) | getQueenAttacks(side) | getKingAttacks(side);
	}

	inline Rawboard getKingAttacks(const Side side) {
		const Position position = Utils::getFirstPos(pieceBoards[WKing + side]);
		return kingAttacks[position] & (EMPTY | OPP_PIECES(side));
	}

	inline Rawboard kingAttack(const Position position, const Rawboard opposite) {
		return kingAttacks[position] & (EMPTY | opposite);
	}

	inline Rawboard getKnightAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WKnight + side];

		while (board) {
			const Position position = Utils::getFirstPos(board);
			attacks |= knightAttacks[position];
			board &= (board - 1);
		}

		return attacks & (EMPTY | OPP_PIECES(side));
	}

	inline Rawboard knightAttack(const Position position, const Rawboard opposite) {
		return knightAttacks[position] & (EMPTY | opposite);
	}

	inline Rawboard getQueenAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WQueen + side];
		const Rawboard occupied = ~EMPTY;
		const Rawboard notSide = ~PIECES(side);

		while (board) {
			const Position position = Utils::getFirstPos(board);
			attacks |= queenAttacks(position, occupied, notSide);
			board &= (board - 1);
		}

		return attacks;
	}

	inline Rawboard queenAttacks(const Position position, const Rawboard occupied, const Rawboard notSide) {
		return (northAttack(occupied, position) | eastAttack(occupied, position) | southAttack(occupied, position) | westAttack(occupied, position) |
				noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notSide;
	}

	inline Rawboard getRookAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WRook + side];
		const Rawboard occupied = ~EMPTY;
		const Rawboard notSide = ~PIECES(side);

		while (board) {
			const Position position = Utils::getFirstPos(board);
			attacks |= rookAttack(position, occupied, notSide);
			board &= (board - 1);
		}

		return attacks;
	}

	inline Rawboard rookAttack(const Position position, const Rawboard occupied, const Rawboard notSide) {
		return (northAttack(occupied, position) | eastAttack(occupied, position) | southAttack(occupied, position) | westAttack(occupied, position)) & notSide;
	}

	inline Rawboard getBishopAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WBishop + side];
		const Rawboard occupied = ~EMPTY;
		const Rawboard notSide = ~PIECES(side);

		while (board) {
			const Position position = Utils::getFirstPos(board);
			attacks |= bishopAttack(position, occupied, notSide);
			board &= (board - 1);
		}

		return attacks;
	}

	inline Rawboard bishopAttack(const Position position, const Rawboard occupied, const Rawboard notSide) {
		return (noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notSide;
	}

	bool isUnderCheck(Position position, Side side);

	Rawboard getPawnMoves(Side side);
    Rawboard getPawnMoves(Position position, Side side);
    Rawboard getPawnAttacks(Side side);
    Rawboard getPawnAttacks(Position position, Side side);
	Rawboard getEnPassantMove(const Rawboard position, Side side) ;

	Rawboard getKingMoves(Side side);
    Rawboard getKingMoves(Position position, Side side);
	Rawboard getKingCastling(Side side) const;
	Rawboard getKingCastling(Position position, Side side) const;

    //Rawboard slidingAttack(Rawboard(*direction)(Rawboard), Rawboard position, Rawboard oppositeBoard) const;

	inline static Rawboard posInd(const Position position) {
		return 1LL << position;
	}

	// rays
	inline Rawboard noWestAttack(const Rawboard occupied, const Position position) {
		return getPositiveRayAttacks_NEW(occupied, NoWest, position);
	}

	inline Rawboard northAttack(const Rawboard occupied, const Position position) {
		return getPositiveRayAttacks_NEW(occupied, North, position);
	}

	inline Rawboard noEastAttack(const Rawboard occupied, const Position position) {
		return getPositiveRayAttacks_NEW(occupied, NoEast, position);
	}

	inline Rawboard eastAttack(const Rawboard occupied, const Position position) {
		return getNegativeRayAttacks_NEW(occupied, East, position);
	}

	inline Rawboard soEastAttack(const Rawboard occupied, const Position position) {
		return getNegativeRayAttacks_NEW(occupied, SoEast, position);
	}

	inline Rawboard southAttack(const Rawboard occupied, const Position position) {
		return getNegativeRayAttacks_NEW(occupied, South, position);
	}

	inline Rawboard soWestAttack(const Rawboard occupied, const Position position) {
		return getNegativeRayAttacks_NEW(occupied, SoWest, position);
	}

	inline Rawboard westAttack(const Rawboard occupied, const Position position) {
		return getPositiveRayAttacks_NEW(occupied, West, position);
	}

    // ray attacks
	inline static Rawboard getNegativeRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = Utils::getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ direction(firstBlockPos);
	}

	static inline Rawboard getNegativeRayAttacks_NEW(const Rawboard occupied, const unsigned char direction, const Position position) {
		const Rawboard attacks = rayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = Utils::getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ rayAttacks[direction][firstBlockPos];
	}

	inline static Rawboard getPositiveRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = Utils::getFirstPosReverse(blocker | 1);
		return attacks ^ direction(firstBlockPos);
	}

	static inline Rawboard getPositiveRayAttacks_NEW(const Rawboard occupied, const unsigned char direction, const Position position) {
		const Rawboard attacks = rayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = Utils::getFirstPosReverse(blocker | 1);
		return attacks ^ rayAttacks[direction][firstBlockPos];
	}

	// One step
	inline static Rawboard northOne(const Rawboard start) {
		return (start >> 8) & SH_8DX_MSK;
	}

	inline static Rawboard noEastOne(const Rawboard start) {
		return (start >> 7) & NOT_A_COL & SH_7DX_MSK;
	}

	inline static Rawboard eastOne(const Rawboard start) {
		return (start << 1) & NOT_A_COL;
	}

	inline static Rawboard soEastOne(const Rawboard start) {
		return (start << 9) & NOT_A_COL;
	}

	inline static Rawboard southOne(const Rawboard start) {
		return start << 8;
	}

	inline static Rawboard soWestOne(const Rawboard start) {
		return (start << 7) & NOT_H_COL;
	}

	inline static Rawboard westOne(const Rawboard start) {
		return (start >> 1) & NOT_H_COL & SH_1DX_MSK;
	}

	inline static Rawboard noWestOne(const Rawboard start) {
		return (start >> 9) & NOT_H_COL & SH_9DX_MSK;
	}

	inline void castlingMove(const Position source, const Position destination) {
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
			default: assert(false);
		}
	}

	inline void undoCastlingMove(const Position source, const Position destination) {
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
			default: assert(false);
		}
	}

};

