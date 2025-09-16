#pragma once

#include <vector>
#include <bit>

#include "../common/types.h"
#include "../common/constants.h"
#include "../common/bitwise.h"
#include "../utils/utils.h"
#include "../utils/pieceHelper.h"
#include "attackBoards.h"
#include "../common/defines.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"
#include "../move/move.h"

using namespace std;


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

	Rawboard PIECES(const Side side) {
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

	Rawboard OPP_PIECES(const Side side) {
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
		resetCalculated(WHITE);
		resetCalculated(BLACK);
	}

	void resetCalculated(Side side) {
		_PIECES[side] = 0;
		_OPP_PIECES[side] = 0;
	}
#endif

    void reset();
	bool equals(const Board& board) const;
	void set(const Board& board);

	unsigned int getPieceCount(const Piece piece) const {
		return popcount(pieceBoards[piece]);
	}

	Side getSide(const Position position) const {
		return _getSide(piecePositions[position]);
	}

	bool isEmpty(const Position position) const {
		return (EMPTY & posInd(position)) != 0L;
	}

	bool isWhite(const Position position) const {
		return _isWhite(piecePositions[position]);
	}

	bool isBlack(const Position position) const {
		return _isBlack(piecePositions[position]);
	}

	bool isPawn(const Position position) const {
		return ((pieceBoards[WPawn] | pieceBoards[BPawn]) & posInd(position)) != 0L;
	}

	bool isKnight(const Position position) const {
		return ((pieceBoards[WKnight] | pieceBoards[BKnight]) & posInd(position)) != 0L;
	}

	bool isBishop(const Position position) const {
		return ((pieceBoards[WBishop] | pieceBoards[BBishop]) & posInd(position)) != 0L;
	}

	bool isRook(const Position position) const {
		return ((pieceBoards[WRook] | pieceBoards[BRook]) & posInd(position)) != 0L;
	}

	bool isRook(const Position position, const Side side) const {
		return (pieceBoards[WRook + side] & posInd(position)) != 0;
	}

	bool isQueen(const Position position) const {
		return ((pieceBoards[WQueen] | pieceBoards[BQueen]) & posInd(position)) != 0L;
	}

	bool isKing(const Position position) const {
		return ((pieceBoards[WKing] | pieceBoards[BKing]) & posInd(position)) != 0L;
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

	Rawboard getAttacks(const Side side) {
		return getPawnAttacks(side) | getKnightAttacks(side) | getBishopAttacks(side) | getRookAttacks(side) | getQueenAttacks(side) | getKingAttacks(side);
	}

	Rawboard getKingAttacks(const Side side) {
		const Position position = getFirstPos(pieceBoards[WKing + side]);
		return kingAttacks[position] & (EMPTY | OPP_PIECES(side));
	}

	Rawboard kingAttack(const Position position, const Rawboard opposite) {
		return kingAttacks[position] & (EMPTY | opposite);
	}

	Rawboard getKnightAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WKnight + side];

		while (board) {
			const Position position = getFirstPos(board);
			attacks |= knightAttacks[position];
			board &= (board - 1);
		}

		return attacks & (EMPTY | OPP_PIECES(side));
	}

	Rawboard knightAttack(const Position position, const Rawboard opposite) const {
		return knightAttacks[position] & (EMPTY | opposite);
	}

	Rawboard getQueenAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WQueen + side];
		const Rawboard occupied = ~EMPTY;
		const Rawboard notSide = ~PIECES(side);

		while (board) {
			const Position position = getFirstPos(board);
			attacks |= queenAttacks(position, occupied, notSide);
			board &= (board - 1);
		}

		return attacks;
	}

	Rawboard queenAttacks(const Position position, const Rawboard occupied, const Rawboard notSide) {
		return (northAttack(occupied, position) | eastAttack(occupied, position) | southAttack(occupied, position) | westAttack(occupied, position) |
				noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notSide;
	}

	Rawboard getRookAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WRook + side];
		const Rawboard occupied = ~EMPTY;
		const Rawboard notSide = ~PIECES(side);

		while (board) {
			const Position position = getFirstPos(board);
			attacks |= rookAttack(position, occupied, notSide);
			board &= (board - 1);
		}

		return attacks;
	}

	Rawboard rookAttack(const Position position, const Rawboard occupied, const Rawboard notSide) {
		return (northAttack(occupied, position) | eastAttack(occupied, position) | southAttack(occupied, position) | westAttack(occupied, position)) & notSide;
	}

	Rawboard getBishopAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WBishop + side];
		const Rawboard occupied = ~EMPTY;
		const Rawboard notSide = ~PIECES(side);

		while (board) {
			const Position position = getFirstPos(board);
			attacks |= bishopAttack(position, occupied, notSide);
			board &= (board - 1);
		}

		return attacks;
	}

	Rawboard bishopAttack(const Position position, const Rawboard occupied, const Rawboard notSide) {
		return (noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notSide;
	}

	Rawboard getPawnMoves(Side side);
    Rawboard getPawnMoves(Position position, Side side);
    Rawboard getPawnAttacks(Side side);
    Rawboard getPawnAttacks(Position position, Side side);
	Rawboard getEnPassantMove(const Rawboard position, Side side) ;

	Rawboard getKingMoves(Side side);
    Rawboard getKingMoves(Position position, Side side);
	Rawboard getKingCastling(Side side) const;
	Rawboard getKingCastling(Position position, Side side) const;

    // rays
	Rawboard noWestAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
		return getPositiveRayAttacks(occupied, noWestRay, position);
#else
		return getPositiveRayAttacks(occupied, NoWest, position);
#endif
	}

	Rawboard northAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
		return getPositiveRayAttacks(occupied, northRay, position);
#else
		return getPositiveRayAttacks(occupied, North, position);
#endif
	}

	Rawboard noEastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
		return getPositiveRayAttacks(occupied, noEastRay, position);
#else
		return getPositiveRayAttacks(occupied, NoEast, position);
#endif
	}

	Rawboard eastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
		return getNegativeRayAttacks(occupied, eastRay, position);
#else
		return getNegativeRayAttacks(occupied, East, position);
#endif
	}

	Rawboard soEastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
		return getNegativeRayAttacks(occupied, soEastRay, position);
#else
		return getNegativeRayAttacks(occupied, SoEast, position);
#endif
	}

	Rawboard southAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
		return getNegativeRayAttacks(occupied, southRay, position);
#else
		return getNegativeRayAttacks(occupied, South, position);
#endif
	}

	Rawboard soWestAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
		return getNegativeRayAttacks(occupied, soWestRay, position);
#else
		return getNegativeRayAttacks(occupied, SoWest, position);
#endif
	}

	Rawboard westAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
		return getPositiveRayAttacks(occupied, westRay, position);
#else
		return getPositiveRayAttacks(occupied, West, position);
#endif
	}

    // ray attacks
#ifdef BOARD_STANDARD_RAY_ATTACKS
	static inline Rawboard getPositiveRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		Rawboard attacks = rayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		if (blocker) {
			const Position firstBlockPos = getFirstPosReverse(blocker);
			attacks ^= rayAttacks[direction][firstBlockPos];
		}
		return attacks;
	}

	static inline Rawboard getNegativeRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		Rawboard attacks = rayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		if (blocker) {
			const Position firstBlockPos = getFirstPos(blocker);
			attacks ^= rayAttacks[direction][firstBlockPos];
		}
		return attacks;
	}
#endif

#ifdef BOARD_BRANCHLESS_RAY_ATTACKS
	static inline Rawboard getPositiveRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		const Rawboard attacks = rayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPosReverse(blocker | 1);
		return attacks ^ rayAttacks[direction][firstBlockPos];
	}

	static inline Rawboard getNegativeRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		const Rawboard attacks = rayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ rayAttacks[direction][firstBlockPos];
	}
#endif

#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	static Rawboard getPositiveRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPosReverse(blocker | 1);
		return attacks ^ direction(firstBlockPos);
	}

	static Rawboard getNegativeRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ direction(firstBlockPos);
	}
#endif

	// One step
	static Rawboard northOne(const Rawboard start) {
		return (start >> 8) & SH_8DX_MSK;
	}

	static Rawboard noEastOne(const Rawboard start) {
		return (start >> 7) & NOT_A_COL & SH_7DX_MSK;
	}

	static Rawboard eastOne(const Rawboard start) {
		return (start << 1) & NOT_A_COL;
	}

	static Rawboard soEastOne(const Rawboard start) {
		return (start << 9) & NOT_A_COL;
	}

	static Rawboard southOne(const Rawboard start) {
		return start << 8;
	}

	static Rawboard soWestOne(const Rawboard start) {
		return (start << 7) & NOT_H_COL;
	}

	static Rawboard westOne(const Rawboard start) {
		return (start >> 1) & NOT_H_COL & SH_1DX_MSK;
	}

	static Rawboard noWestOne(const Rawboard start) {
		return (start >> 9) & NOT_H_COL & SH_9DX_MSK;
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
			default: assert(false);
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
			default: assert(false);
		}
	}

	unordered_set<Position>* getPiecePositions(const unordered_set<Piece>& pieces) const {
		unordered_set<Position>* positions = new unordered_set<Position>();

		for (Position i = 0; i < 64; i++) {
			if (pieces.find(getPiece(i)) != pieces.end()) {
				positions->insert(i);
			}
		}

		return positions;
	}

	bool isOnXRay(const Position sourcePosition, const Position excludePosition) {
		unordered_set<Position>* positions = getPiecePositions(XRAY_PIECES[OPPOSITE(getSide(sourcePosition))]);
		positions->erase(excludePosition);
		Rawboard xRayPositions = 0;

		for (Position position : *positions) {
			xRayPositions |= getDestinationPositions(position);
		}

		delete positions;

		return isUnderCheck(xRayPositions, sourcePosition);
	}

};

