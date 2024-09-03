#pragma once

#include "../common/types.h"
#include "piece.h"

class Board {
    public:
        Board();
        ~Board();

        void reset();
        Rawboard EMPTY();
        Rawboard BOARD();
        Rawboard WHITE();
        Rawboard BLACK();
        Rawboard getBoard(RawboardIndex boardIndex);
        void setBoard(RawboardIndex boardIndex, Rawboard pieceBoard);
        bool isEmpty(Position position);
        bool isWhite(Position position);
        bool isBlack(Position position);
        bool isPawn(Position position);
        bool isKnight(Position position);
        bool isBishop(Position position);
        bool isRook(Position position);
        bool isRook(Position position, bool white);
        bool isQueen(Position position);
        bool isKing(Position position);
        Piece getPiece(Position position);
        Piece setPiece(Position position, Piece piece);
        Piece move(Position source, Position destination, Piece piece);
        void set(Board& board);
        bool isUnderCheck(Position position, bool white);
        Rawboard getWhiteAttacks();

        // rays
        static Rawboard lineMask(Position position) {
            return 0xFFLL << (position & 56);
        }

        static Rawboard columnMask(Position position) {
            return 0x0101010101010101LL << (position & 7);
        }

        /*
        static Rawboard diagonalMask(Position position) {
            const Rawboard mainDiag = 0x8040201008040201LL;
            Position diag = (position & 7) - (position >> 3);
            return diag >= 0 ? (mainDiag >> diag * 8) & RIGHT_SHIFT_MSK(diag * 8) : mainDiag << -diag * 8;
        }
        */

        static Rawboard diagonalMask(int sq) {
            const Rawboard maindia = 0x8040201008040201LL;
            int diag = 8 * (sq & 7) - (sq & 56);
            int nort = -diag & (diag >> 31);
            int sout = diag & (-diag >> 31);
            return (maindia >> sout) << nort;
        }

        static Rawboard antiDiagonalMask(Position position) {
            const Rawboard maindia = 0x0102040810204080LL;
            const int diag = 7 - (position & 7) - (position >> 3);
            return diag >= 0 ? (maindia >> diag * 8) & RIGHT_SHIFT_MSK(diag * 8) : maindia << -diag * 8;
        }

        static Rawboard eastRay(Position position) {
            return lineMask(position) & (((Rawboard)-2) << position);
        }

        static Rawboard westRay(Position position) {
            return lineMask(position) & ((((Rawboard)1) << position) - 1);
        }

        static Rawboard southRay(Position position) {
            return columnMask(position) & (((Rawboard)-2) << position);
        }

        static Rawboard northRay(Position position) {
            return columnMask(position) & ((((Rawboard)1) << position) - 1);
        }

        static Rawboard soEastRay(Position position) {
            return diagonalMask(position) & (((Rawboard)-2) << position);
        }

        static Rawboard noWestRay(Position position) {
            return diagonalMask(position) & ((((Rawboard)1) << position) - 1);
        }

        static Rawboard soWestRay(Position position) {
            return antiDiagonalMask(position) & (((Rawboard)-2) << position);
        }

        static Rawboard noEastRay(Position position) {
            return antiDiagonalMask(position) & ((((Rawboard)1) << position) - 1);
        }

    private:
        static const RawboardIndex SIZE = 13;
        static const RawboardIndex EMPTY_IND = 0;   // This is related to Piece.Empty value
        static const Rawboard EMPTY_BOARD = 0xffffffffffffffffLL;
        static const Rawboard NOT_A_COL = 0xfefefefefefefefeLL;
        static const Rawboard NOT_AB_COL = 0xfcfcfcfcfcfcfcfcLL;
        static const Rawboard NOT_H_COL = 0x7f7f7f7f7f7f7f7fLL;
        static const Rawboard NOT_GH_COL = 0x3f3f3f3f3f3f3f3fLL;
        static const Rawboard SH_7DX_MSK = 0x1ffffffffffffffLL;
        static const Rawboard SH_1DX_MSK = 0x7fffffffffffffffLL;
        static const Rawboard SH_9DX_MSK = 0x7fffffffffffffLL;
        static const Rawboard SH_8DX_MSK = 0xffffffffffffffLL;
        static const Rawboard SH_15DX_MSK = 0x1ffffffffffffLL;
        static const Rawboard SH_6DX_MSK = 0x3ffffffffffffffLL;
        static const Rawboard SH_17DX_MSK = 0x7fffffffffffLL;
        static const Rawboard SH_10DX_MSK = 0x3fffffffffffffLL;
        static const Rawboard ROW_5 = 0x00000000ff000000LL;
        static const Rawboard ROW_4 = 0x000000ff00000000LL;

        Rawboard* pieceBoards;

       
        
        Rawboard getBlackAttacks();
        Rawboard getKingAttacks(bool white);
        Rawboard getKingMoves(bool white, CastlingInfo castlingInfo);
        Rawboard getQueenAttacks(bool white);
        Rawboard getRookAttacks(bool white);
        Rawboard getBishopAttacks(bool white);
        Rawboard getKnightAttacks(bool white);
        Rawboard getPawnMoves(bool white, Position enPassantPos);
        Rawboard getPawnAttacks(bool white);
        Rawboard getPawnMoves(Position position, bool white, Position enPassantPos);
        Rawboard getPawnAttacks(Position position, bool white);
        Rawboard getPawnEnPassant(Rawboard position, bool white, Position enPassantPos);
        Rawboard getKnightMoves(Position position, bool white);
        Rawboard getBishopMoves(Position position, bool white);
        Rawboard getRookMoves(Position position, bool white);
        Rawboard getQueenMoves(Position position, bool white);
        Rawboard getKingMoves(Position position, bool white, CastlingInfo castlingInfo);
        Rawboard getKingAttacks(Position position, bool white);
        Rawboard getKingCastling(bool white, CastlingInfo castlingInfo);
        Rawboard getKingCastling(Position position, bool white, CastlingInfo castlingInfo);
        Rawboard slidingAttack(Rawboard(*direction)(Rawboard), Rawboard position, Rawboard oppositeBoard);
        Rawboard rookAttack(Position position, bool white);
        Rawboard bishopAttack(Position position, bool white);
        Rawboard queenAttacks(Position position, bool white);

        // bitwise operations
        static Rawboard posInd(Position position) {
            return 1LL << position;
        }

        // Mask for right shift that add ones
        static Rawboard RIGHT_SHIFT_MSK(int len) {
            return len > 0 ? ~(0xffffffffffffffffLL << (64 - len)) : 0xffffffffffffffffLL;
        }

        // One step
        static Rawboard northOne(Rawboard start) {
            return (start >> 8) & SH_8DX_MSK;
        }

        static Rawboard noEastOne(Rawboard start) {
            return (start >> 7) & NOT_A_COL & SH_7DX_MSK;
        }

        static Rawboard eastOne(Rawboard start) {
            return (start << 1) & NOT_A_COL;
        }

        static Rawboard soEastOne(Rawboard start) {
            return (start << 9) & NOT_A_COL;
        }

        static Rawboard southOne(Rawboard start) {
            return start << 8;
        }

        static Rawboard soWestOne(Rawboard start) {
            return (start << 7) & NOT_H_COL;
        }

        static Rawboard westOne(Rawboard start) {
            return (start >> 1) & NOT_H_COL & SH_1DX_MSK;
        }

        static Rawboard noWestOne(Rawboard start) {
            return (start >> 9) & NOT_H_COL & SH_9DX_MSK;
        }

        // Knight step
        static Rawboard noNoEa(Rawboard start) {
            return (start >> 15) & NOT_A_COL & SH_15DX_MSK;
        }

        static Rawboard noEaEa(Rawboard start) {
            return (start >> 6) & NOT_AB_COL & SH_6DX_MSK;
        }

        static Rawboard soEaEa(Rawboard start) {
            return (start << 10) & NOT_AB_COL;
        }

        static Rawboard soSoEa(Rawboard start) {
            return (start << 17) & NOT_A_COL;
        }

        static Rawboard noNoWe(Rawboard start) {
            return (start >> 17) & NOT_H_COL & SH_17DX_MSK;
        }

        static Rawboard noWeWe(Rawboard start) {
            return (start >> 10) & NOT_GH_COL & SH_10DX_MSK;
        }

        static Rawboard soWeWe(Rawboard start) {
            return (start << 6) & NOT_GH_COL;
        }

        static Rawboard soSoWe(Rawboard start) {
            return (start << 15) & NOT_H_COL;
        }

        // ray attacks
        static Position numberOfTrailingZeros(Rawboard value) {
            if (value == 0) return 64; // Special case for 0, as __builtin_ctzll is undefined for 0
            //return __builtin_ctzll(value);
            unsigned long index;
            _BitScanForward64(&index, value);
            return (Position)index;
        }

        static Rawboard getNegativeRayAttacks(Rawboard occupied, Rawboard(*direction)(Position), int position) {
            Rawboard attacks = direction(position);
            const Rawboard blocker = attacks & occupied;
            const Position firstBlockPos = numberOfTrailingZeros(blocker);
            if (firstBlockPos < 64) {
                attacks ^= direction(firstBlockPos);
            }
            return attacks;
        }

        static Position numberOfLeadingZeros(Rawboard value) {
            if (value == 0) return 64; // Special case for 0, as __builtin_clzll is undefined for 0
            //return __builtin_clzll(value);
            unsigned long index;
            _BitScanReverse64(&index, value);
            return (Position)index;
        }

        static Rawboard getPositiveRayAttacks(Rawboard occupied, Rawboard(*direction)(Position), int position) {
            Rawboard attacks = direction(position);
            const Rawboard blocker = attacks & occupied;
            const Position firstBlockPos = 63 - numberOfLeadingZeros(blocker);
            if (firstBlockPos > -1) {
                attacks ^= direction(firstBlockPos);
            }
            return attacks;
        }

        static Rawboard noWestAttack(Rawboard occupied, int position) {
            return getPositiveRayAttacks(occupied, noWestRay, position);
        }

        static Rawboard northAttack(Rawboard occupied, int position) {
            return getPositiveRayAttacks(occupied, northRay, position);
        }

        static Rawboard noEastAttack(Rawboard occupied, int position) {
            return getPositiveRayAttacks(occupied, noEastRay, position);
        }

        static Rawboard eastAttack(Rawboard occupied, int position) {
            return getNegativeRayAttacks(occupied, eastRay, position);
        }

        static Rawboard soEastAttack(Rawboard occupied, int position) {
            return getNegativeRayAttacks(occupied, soEastRay, position);
        }

        static Rawboard southAttack(Rawboard occupied, int position) {
            return getNegativeRayAttacks(occupied, southRay, position);
        }

        static Rawboard soWestAttack(Rawboard occupied, int position) {
            return getNegativeRayAttacks(occupied, soWestRay, position);
        }

        static Rawboard westAttack(Rawboard occupied, int position) {
            return getPositiveRayAttacks(occupied, westRay, position);
        }

};
