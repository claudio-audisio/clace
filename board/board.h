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
	Rawboard BOARD(Side side) const;
	Rawboard OPPOSITE(Side side) const;
    /*Rawboard WHITE() const;
    Rawboard BLACK() const;*/
    void setBoard(Piece boardIndex, Rawboard pieceBoard);
	void update();
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
    Piece move(Position source, Position destination, Piece piece);
    void set(const Board& board);
    bool isUnderCheck(Position position, Side side) const;
	Rawboard getAttacks(Side side) const;
    /*Rawboard getWhiteAttacks() const;
    Rawboard getBlackAttacks() const;*/
    Rawboard getKingAttacks(Side side) const;
    Rawboard getKingMoves(Side side, CastlingInfo castlingInfo) const;
    Rawboard getQueenAttacks(Side side) const;
    Rawboard getRookAttacks(Side side) const;
    Rawboard getBishopAttacks(Side side) const;
    Rawboard getKnightAttacks(Side side) const;
    Rawboard getPawnMoves(Side side, Position enPassantPos) const;
    Rawboard getPawnMoves(Position position, Side side, Position enPassantPos) const;
    Rawboard getPawnAttacks(Side side) const;
    Rawboard getPawnAttacks(Position position, Side side) const;
    Rawboard getKnightMoves(Position position, Side side) const;
    Rawboard getBishopMoves(Position position, Side side) const;
    Rawboard getRookMoves(Position position, Side side) const;
    Rawboard getQueenMoves(Position position, Side side) const;
    Rawboard getKingMoves(Position position, Side side, CastlingInfo castlingInfo) const;
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
	Rawboard sideBoards[2];

private:
    static Rawboard getPawnEnPassant(Rawboard position, Side side, Position enPassantPos) ;
    Rawboard getKingAttacks(Position position, Side side) const;
    Rawboard getKingCastling(Side side, CastlingInfo castlingInfo) const;
    Rawboard getKingCastling(Position position, Side side, CastlingInfo castlingInfo) const;
    Rawboard rookAttack(Position position, Side side) const;
    Rawboard bishopAttack(Position position, Side side) const;
    Rawboard queenAttacks(Position position, Side side) const;

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

    // ray attacks
    inline static Rawboard getNegativeRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
        Rawboard attacks = direction(position);
        const Rawboard blocker = attacks & occupied;
        if (blocker) {
            const Position firstBlockPos = Utils::getFirstPos(blocker);
            attacks ^= direction(firstBlockPos);
        }
        return attacks;
    }

    inline static Rawboard getPositiveRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
        Rawboard attacks = direction(position);
        const Rawboard blocker = attacks & occupied;
        if (blocker) {
            const Position firstBlockPos = Utils::getFirstPosRevers(blocker);
            attacks ^= direction(firstBlockPos);
        }
        return attacks;
    }

};
