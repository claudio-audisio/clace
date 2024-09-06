#pragma once

#include "../common/types.h"
#include "piece.h"

class Board {
    public:
        Board();
        ~Board();

        void reset();
        Rawboard EMPTY() const;
        Rawboard BOARD() const;
        Rawboard WHITE() const;
        Rawboard BLACK() const;
        Rawboard getBoard(const RawboardIndex boardIndex) const;
        void setBoard(const RawboardIndex boardIndex, const Rawboard pieceBoard);
        bool isEmpty(const Position position) const;
        bool isWhite(const Position position) const;
        bool isBlack(const Position position) const;
        bool isPawn(const Position position) const;
        bool isKnight(const Position position) const;
        bool isBishop(const Position position) const;
        bool isRook(const Position position) const;
        bool isRook(const Position position, const bool white) const;
        bool isQueen(const Position position) const;
        bool isKing(const Position position) const;
        Piece getPiece(const Position position) const;
        Piece setPiece(const Position position, const Piece piece);
        Piece move(const Position source, const Position destination, Piece piece);
        void set(const Board& board);
        bool isUnderCheck(const Position position, const bool white) const;
        Rawboard getWhiteAttacks() const;
        Rawboard getBlackAttacks() const;
        Rawboard getKingAttacks(const bool white) const;
        Rawboard getKingMoves(const bool white, const CastlingInfo castlingInfo) const;
        Rawboard getQueenAttacks(const bool white) const;
        Rawboard getRookAttacks(const bool white) const;
        Rawboard getBishopAttacks(const bool white) const;
        Rawboard getKnightAttacks(const bool white) const;
        Rawboard getPawnMoves(const bool white, const Position enPassantPos) const;
        Rawboard getPawnMoves(const Position position, const bool white, const Position enPassantPos) const;
        Rawboard getPawnAttacks(const bool white) const;
        Rawboard getPawnAttacks(const Position position, const bool white) const;
        Rawboard getKnightMoves(const Position position, const bool white) const;
        Rawboard getBishopMoves(const Position position, const bool white) const;
        Rawboard getRookMoves(const Position position, const bool white) const;
        Rawboard getQueenMoves(const Position position, const bool white) const;
        Rawboard getKingMoves(const Position position, const bool white, const CastlingInfo castlingInfo) const;
        Rawboard slidingAttack(Rawboard(*direction)(Rawboard), const Rawboard position, const Rawboard oppositeBoard) const;

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

        // rays
        static Rawboard lineMask(const Position position) {
            return 0xFFLL << (position & 56);
        }

        static Rawboard columnMask(const Position position) {
            return 0x0101010101010101LL << (position & 7);
        }

        /*
        static Rawboard diagonalMask(const Position position) {
            const Rawboard mainDiag = 0x8040201008040201LL;
            Position diag = (position & 7) - (position >> 3);
            return diag >= 0 ? (mainDiag >> diag * 8) & RIGHT_SHIFT_MSK(diag * 8) : mainDiag << -diag * 8;
        }
        */

        static Rawboard diagonalMask(int sq) {
            const Rawboard mainDiag = 0x8040201008040201LL;
            int diag = 8 * (sq & 7) - (sq & 56);
            int nort = -diag & (diag >> 31);
            int sout = diag & (-diag >> 31);
            return (mainDiag >> sout) << nort;
        }

        static Rawboard antiDiagonalMask(const Position position) {
            const Rawboard mainDiag = 0x0102040810204080LL;
            const int diag = 7 - (position & 7) - (position >> 3);
            return diag >= 0 ? (mainDiag >> diag * 8) & RIGHT_SHIFT_MSK(diag * 8) : mainDiag << -diag * 8;
        }

        static Rawboard eastRay(const Position position) {
            return lineMask(position) & (((Rawboard)-2) << position);
        }

        static Rawboard westRay(const Position position) {
            return lineMask(position) & ((((Rawboard)1) << position) - 1);
        }

        static Rawboard southRay(const Position position) {
            return columnMask(position) & (((Rawboard)-2) << position);
        }

        static Rawboard northRay(const Position position) {
            return columnMask(position) & ((((Rawboard)1) << position) - 1);
        }

        static Rawboard soEastRay(const Position position) {
            return diagonalMask(position) & (((Rawboard)-2) << position);
        }

        static Rawboard noWestRay(const Position position) {
            return diagonalMask(position) & ((((Rawboard)1) << position) - 1);
        }

        static Rawboard soWestRay(const Position position) {
            return antiDiagonalMask(position) & (((Rawboard)-2) << position);
        }

        static Rawboard noEastRay(const Position position) {
            return antiDiagonalMask(position) & ((((Rawboard)1) << position) - 1);
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

        static RawboardIndex getInternalSize() {
            return SIZE;
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

        
        Rawboard getPawnEnPassant(Rawboard position, const bool white, const Position enPassantPos) const;
        Rawboard getKingAttacks(const Position position, const bool white) const;
        Rawboard getKingCastling(const bool white, CastlingInfo castlingInfo) const;
        Rawboard getKingCastling(const Position position, const bool white, const CastlingInfo castlingInfo) const;
        Rawboard rookAttack(const Position position, const bool white) const;
        Rawboard bishopAttack(const Position position, const bool white) const;
        Rawboard queenAttacks(const Position position, const bool white) const;

        // bitwise operations
        static Rawboard posInd(const Position position) {
            return 1LL << position;
        }

        // Mask for right shift that add ones
        static Rawboard RIGHT_SHIFT_MSK(const unsigned int len) {
            return len > 0 ? ~(0xffffffffffffffffLL << (64 - len)) : 0xffffffffffffffffLL;
        }

        // Knight step
        static Rawboard noNoEa(const Rawboard start) {
            return (start >> 15) & NOT_A_COL & SH_15DX_MSK;
        }

        static Rawboard noEaEa(const Rawboard start) {
            return (start >> 6) & NOT_AB_COL & SH_6DX_MSK;
        }

        static Rawboard soEaEa(const Rawboard start) {
            return (start << 10) & NOT_AB_COL;
        }

        static Rawboard soSoEa(const Rawboard start) {
            return (start << 17) & NOT_A_COL;
        }

        static Rawboard noNoWe(const Rawboard start) {
            return (start >> 17) & NOT_H_COL & SH_17DX_MSK;
        }

        static Rawboard noWeWe(const Rawboard start) {
            return (start >> 10) & NOT_GH_COL & SH_10DX_MSK;
        }

        static Rawboard soWeWe(const Rawboard start) {
            return (start << 6) & NOT_GH_COL;
        }

        static Rawboard soSoWe(const Rawboard start) {
            return (start << 15) & NOT_H_COL;
        }

        // ray attacks
        static char numberOfTrailingZeros(const Rawboard value) {
            if (value == 0) return 64; // Special case for 0, as __builtin_ctzll is undefined for 0
            //return __builtin_ctzll(value);
            unsigned long index;
            _BitScanForward64(&index, value);
            return (char)index;
        }

        static Rawboard getNegativeRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
            Rawboard attacks = direction(position);
            const Rawboard blocker = attacks & occupied;
            const char firstBlockPos = numberOfTrailingZeros(blocker);
            if (firstBlockPos < 64) {
                attacks ^= direction(firstBlockPos);
            }
            return attacks;
        }

        static char numberOfLeadingZeros(const Rawboard value) {
            if (value == 0) return 64; // Special case for 0, as __builtin_clzll is undefined for 0
            //return __builtin_clzll(value);
            unsigned long index;
            _BitScanReverse64(&index, value);
            return (char)index;
        }

        static Rawboard getPositiveRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
            Rawboard attacks = direction(position);
            const Rawboard blocker = attacks & occupied;
            const char firstBlockPos = numberOfLeadingZeros(blocker);
            if (firstBlockPos > -1) {
                attacks ^= direction(firstBlockPos);
            }
            return attacks;
        }

};
