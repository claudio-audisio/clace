#pragma once

#include <vector>
#include <bit>

#include "../common/types.h"
#include "../common/constants.h"
#include "piece.h"
#include "../utils/utils.h"

using namespace std;


class Board {
public:
    Board();
    ~Board();

    void reset();
	void resetOpposite();
	void resetOpposite(Side side);
	Rawboard BOARD(Side side) const;
	Rawboard OPPOSITE(Side side) const;
	Rawboard getOpposite(Side side);
    void setBoard(Piece boardIndex, Rawboard pieceBoard);
    bool isEmpty(Position position) const;
    bool isWhite(Position position) const;
    bool isBlack(Position position) const;
    bool isPawn(Position position) const;
    bool isKnight(Position position) const;
    bool isBishop(Position position) const;
    bool isRook(Position position) const;
    bool isRook(Position position, Side side) const;
    bool isQueen(Position position) const;
    bool isKing(Position position) const;
	Position getWhiteKingPosition() const;
	Position getBlackKingPosition() const;
    Piece getPiece(Position position) const;
    Piece setPiece(Position position, Piece piece);
	Piece setEmpty(Position position);
    Piece move(Position source, Position destination, Piece piece);
    void set(const Board& board);
    bool isUnderCheck(Position position, Side side);

	Rawboard getAttacks(Side side);

	Rawboard getQueenAttacks(Side side);
	Rawboard getQueenMoves(Position position, Side side);
	static Rawboard queenAttacks(Position position, Rawboard occupied, Rawboard notSide);

	Rawboard getRookAttacks(Side side);
	Rawboard getRookMoves(Position position, Side side);
	static Rawboard rookAttack(Position position, Rawboard occupied, Rawboard notSide) ;

	Rawboard getBishopAttacks(Side side);
	Rawboard getBishopMoves(Position position, Side side);
	static Rawboard bishopAttack(Position position, Rawboard occupied, Rawboard notSide) ;

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
    inline static Rawboard lineMask(const Position position) {
        return 0xFFLL << (position & 56);
    }

    inline static Rawboard columnMask(const Position position) {
        return 0x0101010101010101LL << (position & 7);
    }

    inline static Rawboard diagonalMask(const Position sq) {
        const int diag = 8 * (sq & 7) - (sq & 56);
        const int north = -diag & (diag >> 31);
        const int south = diag & (-diag >> 31);
        return (MAIN_DIAG >> south) << north;
    }

    inline static Rawboard antiDiagonalMask(const Position position) {
        const int diag = 7 - (position & 7) - (position >> 3);
        return diag >= 0 ? (MAIN_ANTI_DIAG >> diag * 8) & RIGHT_SHIFT_MSK(diag * 8) : MAIN_ANTI_DIAG << -diag * 8;
    }

    inline static Rawboard eastRay(const Position position) {
        return lineMask(position) & (((Rawboard)-2) << position);
    }

    inline static Rawboard westRay(const Position position) {
        return lineMask(position) & ((((Rawboard)1) << position) - 1);
    }

    inline static Rawboard southRay(const Position position) {
        return columnMask(position) & (((Rawboard)-2) << position);
    }

    inline static Rawboard northRay(const Position position) {
        return columnMask(position) & ((((Rawboard)1) << position) - 1);
    }

    inline static Rawboard soEastRay(const Position position) {
        return diagonalMask(position) & (((Rawboard)-2) << position);
    }

    inline static Rawboard noWestRay(const Position position) {
        return diagonalMask(position) & ((((Rawboard)1) << position) - 1);
    }

    inline static Rawboard soWestRay(const Position position) {
        return antiDiagonalMask(position) & (((Rawboard)-2) << position);
    }

    inline static Rawboard noEastRay(const Position position) {
        return antiDiagonalMask(position) & ((((Rawboard)1) << position) - 1);
    }

    inline static Rawboard noWestAttack(const Rawboard occupied, const Position position) {
        return getPositiveRayAttacks(occupied, noWestRay, position);
    }

    inline static Rawboard northAttack(const Rawboard occupied, const Position position) {
        return getPositiveRayAttacks(occupied, northRay, position);
    }

    inline static Rawboard noEastAttack(const Rawboard occupied, const Position position) {
        return getPositiveRayAttacks(occupied, noEastRay, position);
    }

    inline static Rawboard eastAttack(const Rawboard occupied, const Position position) {
        return getNegativeRayAttacks(occupied, eastRay, position);
    }

    inline static Rawboard soEastAttack(const Rawboard occupied, const Position position) {
        return getNegativeRayAttacks(occupied, soEastRay, position);
    }

    inline static Rawboard southAttack(const Rawboard occupied, const Position position) {
        return getNegativeRayAttacks(occupied, southRay, position);
    }

    inline static Rawboard soWestAttack(const Rawboard occupied, const Position position) {
        return getNegativeRayAttacks(occupied, soWestRay, position);
    }

    inline static Rawboard westAttack(const Rawboard occupied, const Position position) {
        return getPositiveRayAttacks(occupied, westRay, position);
    }

    Rawboard pieceBoards[SIZE];
    Piece piecePositions[64];
    Rawboard& EMPTY = pieceBoards[EMPTY_IND];
	Rawboard opposite[2];
	bool oppositeReady[2];

private:
    // Mask for right shift that add ones
    inline static Rawboard RIGHT_SHIFT_MSK(const unsigned int len) {
        return len > 0 ? ~(0xffffffffffffffffLL << (64 - len)) : 0xffffffffffffffffLL;
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

	// TODO per usare i ray attacks devo preclcolarmi le linee di attacco di ogni posizione per ogni pezzo
	// Vedi https://www.chessprogramming.org/On_an_empty_Board#RayAttacks

	// TODO Ragionando per ogni pezzo dovrei precalcolare le posizioni di attacco e di mossa per tutti i pezzi

    // ray attacks
    /*inline static Rawboard getNegativeRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
        Rawboard attacks = direction(position);
        const Rawboard blocker = attacks & occupied;
        if (blocker) {
			const Position firstBlockPos = Utils::getFirstPos(blocker);
            attacks ^= direction(firstBlockPos);
        }
        return attacks;
    }*/

	inline static Rawboard getNegativeRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = Utils::getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ direction(firstBlockPos);
	}

    /*inline static Rawboard getPositiveRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
        Rawboard attacks = direction(position);
        const Rawboard blocker = attacks & occupied;
        if (blocker) {
            const Position firstBlockPos = Utils::getFirstPosReverse(blocker);
            attacks ^= direction(firstBlockPos);
        }
        return attacks;
    }*/

	inline static Rawboard getPositiveRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = Utils::getFirstPosReverse(blocker | 1);
		return attacks ^ direction(firstBlockPos);
	}

};
