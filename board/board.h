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

class Board {
public:
    Board();
    ~Board();

	Rawboard pieceBoards[SIZE];
	Piece piecePositions[64];
	Rawboard& EMPTY = pieceBoards[EMPTY_IND];
	/*Rawboard opposite[2];
	bool oppositeReady[2];
	Rawboard notSide[2];
	bool notSideReady[2];*/

	inline Rawboard BOARD(const Side side) const {
		return pieceBoards[WPawn + side] |
			   pieceBoards[WBishop + side] |
			   pieceBoards[WQueen + side] |
			   pieceBoards[WKnight + side] |
			   pieceBoards[WKing + side] |
			   pieceBoards[WRook + side];
	}

	inline Rawboard OPPOSITE(const Side side) const {
		return pieceBoards[BPawn - side] |
			   pieceBoards[BBishop - side] |
			   pieceBoards[BQueen - side] |
			   pieceBoards[BKnight - side] |
			   pieceBoards[BKing - side] |
			   pieceBoards[BRook - side];
	}

    void reset();
	/*void resetCalculated();
	void resetCalculated(Side side);*/
	/*Rawboard BOARD(Side side) const;
	Rawboard OPPOSITE(Side side) const;*/
	/*Rawboard getOpposite(Side side);
	Rawboard getNotSide(Side side);*/
    void setBoard(Piece boardIndex, Rawboard pieceBoard);
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
		/*if (oldPiece > 0) { // not empty
			resetCalculated();
		} else {
			resetCalculated(PieceHelper::getOppositeSide(piece));
		}*/
		const Rawboard posIndex = posInd(position);
		pieceBoards[oldPiece] &= ~posIndex;
		pieceBoards[piece] |= posIndex;
		piecePositions[position] = piece;
		return oldPiece;
	}

	inline Piece setEmpty(const Position position) {
		// NOTE non devo resettare i flag perche' prima di un set empty, c'e' stato per forza un setPiece (non compaiono caselle vuote dal nulla)
		const Rawboard posIndex = posInd(position);
		const Piece oldPiece = getPiece(position);
		pieceBoards[oldPiece] &= ~posIndex;
		pieceBoards[EMPTY_IND] |= posIndex;
		piecePositions[position] = Empty;
		return oldPiece;
	}

	inline Piece move(const Position source, const Position destination, Piece piece) {
		const Piece oldPiece = setPiece(destination, piece);
		const Rawboard posSource = posInd(source);
		pieceBoards[piece] &= ~posSource;
		pieceBoards[EMPTY_IND] |= posSource;
		piecePositions[source] = Empty;
		return oldPiece;
	}

	Rawboard getDestinationPositions(const Position position, const Position enPassantPosition, const CastlingInfo castlingInfo) {
		return getDestinationPositions(position, piecePositions[position], enPassantPosition, castlingInfo);
	}

	Rawboard getDestinationPositions(const Position position, const Piece piece, const Position enPassantPosition, const CastlingInfo castlingInfo) {
		switch (piece) {
			case WPawn: return getPawnMoves(position, WHITE, enPassantPosition);
			case BPawn: return getPawnMoves(position, BLACK, enPassantPosition);
				/*case WRook: return game.getBoard().getRookMoves(position, WHITE);
				case BRook: return game.getBoard().getRookMoves(position, BLACK);*/
			case WRook: return rookAttack(position, ~EMPTY, ~BOARD(WHITE));
			case BRook: return rookAttack(position, ~EMPTY, ~BOARD(BLACK));
			case WKnight: return getKnightMoves(position, WHITE);
			case BKnight: return getKnightMoves(position, BLACK);
				/*case WBishop: return game.getBoard().getBishopMoves(position, WHITE);
				case BBishop: return game.getBoard().getBishopMoves(position, BLACK);*/
			case WBishop: return bishopAttack(position, ~EMPTY, ~BOARD(WHITE));
			case BBishop: return bishopAttack(position, ~EMPTY, ~BOARD(BLACK));
				/*case WQueen: return game.getBoard().getQueenMoves(position, WHITE);
				case BQueen: return game.getBoard().getQueenMoves(position, BLACK);*/
			case WQueen: return queenAttacks(position, ~EMPTY, ~BOARD(WHITE));
			case BQueen: return queenAttacks(position, ~EMPTY, ~BOARD(BLACK));
			case WKing: return getKingMoves(position, WHITE, castlingInfo);
			case BKing: return getKingMoves(position, BLACK, castlingInfo);
			default: return 0;
		};
	}

	inline Rawboard getAttacks(const Side side) {
		return getPawnAttacks(side) | getKnightAttacks(side) | getBishopAttacks(side) | getRookAttacks(side) | getQueenAttacks(side) | getKingAttacks(side);
	}

	inline Rawboard getQueenAttacks(const Side side) {
		Rawboard attacks = 0;
		Rawboard board = pieceBoards[WQueen + side];
		const Rawboard occupied = ~EMPTY;
		const Rawboard notSide = ~BOARD(side);

		while (board) {
			const Position position = Utils::getFirstPos(board);
			//attacks |= getQueenMoves(position, side);      // OLD
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
		const Rawboard notSide = ~BOARD(side);

		while (board) {
			const Position position = Utils::getFirstPos(board);
			//attacks |= getRookMoves(position, side);       // OLD
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
		const Rawboard notSide = ~BOARD(side);

		while (board) {
			const Position position = Utils::getFirstPos(board);
			//attacks |= getBishopMoves(position, side);       // OLD
			attacks |= bishopAttack(position, occupied, notSide);
			board &= (board - 1);
		}

		return attacks;
	}

	inline Rawboard bishopAttack(const Position position, const Rawboard occupied, const Rawboard notSide) {
		return (noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notSide;
	}

	bool isUnderCheck(Position position, Side side);
	Rawboard getQueenMoves(Position position, Side side);
	Rawboard getRookMoves(Position position, Side side);
	Rawboard getBishopMoves(Position position, Side side);

	Rawboard getKnightAttacks(Side side);
	Rawboard getKnightMoves(Position position, Side side);

	Rawboard getPawnMoves(Side side, Position enPassantPos);
    Rawboard getPawnMoves(Position position, Side side, Position enPassantPos);
    Rawboard getPawnAttacks(Side side);
    Rawboard getPawnAttacks(Position position, Side side);
	static Rawboard getPawnEnPassant(Rawboard position, Side side, Position enPassantPos) ;

	Rawboard getKingMoves(Side side, CastlingInfo castlingInfo);
    Rawboard getKingMoves(Position position, Side side, CastlingInfo castlingInfo);
	Rawboard getKingAttacks(Side side);
	Rawboard getKingAttacks(Position position, Side side);
	Rawboard getKingCastling(Side side, CastlingInfo castlingInfo) const;
	Rawboard getKingCastling(Position position, Side side, CastlingInfo castlingInfo) const;

    Rawboard slidingAttack(Rawboard(*direction)(Rawboard), Rawboard position, Rawboard oppositeBoard) const;

	inline static Rawboard posInd(const Position position) {
		return 1LL << position;
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

    // Knight step
    inline static Rawboard noNoEa(const Rawboard start) {
        return (start >> 15) & NOT_A_COL & SH_15DX_MSK;
    }

    inline static Rawboard noEaEa(const Rawboard start) {
        return (start >> 6) & NOT_AB_COL & SH_6DX_MSK;
    }

    inline static Rawboard soEaEa(const Rawboard start) {
        return (start << 10) & NOT_AB_COL;
    }

    inline static Rawboard soSoEa(const Rawboard start) {
        return (start << 17) & NOT_A_COL;
    }

    inline static Rawboard noNoWe(const Rawboard start) {
        return (start >> 17) & NOT_H_COL & SH_17DX_MSK;
    }

    inline static Rawboard noWeWe(const Rawboard start) {
        return (start >> 10) & NOT_GH_COL & SH_10DX_MSK;
    }

    inline static Rawboard soWeWe(const Rawboard start) {
        return (start << 6) & NOT_GH_COL;
    }

    inline static Rawboard soSoWe(const Rawboard start) {
        return (start << 15) & NOT_H_COL;
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

};

