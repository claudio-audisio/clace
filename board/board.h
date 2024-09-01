#pragma once

#include "../common/types.h"
#include "piece.h"

class Board {
    public:
        Board();
        ~Board();

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

    private:
        const RawboardIndex SIZE = 13;
        const RawboardIndex EMPTY_IND = 0;   // This is related to Piece.Empty value
        const Rawboard EMPTY_BOARD = 0xffffffffffffffff;
        const Rawboard NOT_A_COL = 0xfefefefefefefefe;
        const Rawboard NOT_AB_COL = 0xfcfcfcfcfcfcfcfc;
        const Rawboard NOT_H_COL = 0x7f7f7f7f7f7f7f7f;
        const Rawboard NOT_GH_COL = 0x3f3f3f3f3f3f3f3f;
        const Rawboard SH_7DX_MSK = 0x1ffffffffffffff;
        const Rawboard SH_1DX_MSK = 0x7fffffffffffffff;
        const Rawboard SH_9DX_MSK = 0x7fffffffffffff;
        const Rawboard SH_8DX_MSK = 0xffffffffffffff;
        const Rawboard SH_15DX_MSK = 0x1ffffffffffff;
        const Rawboard SH_6DX_MSK = 0x3ffffffffffffff;
        const Rawboard SH_17DX_MSK = 0x7fffffffffff;
        const Rawboard SH_10DX_MSK = 0x3fffffffffffff;
        const Rawboard ROW_5 = 0x00000000ff000000;
        const Rawboard ROW_4 = 0x000000ff00000000;

        Rawboard* pieceBoards;

        void reset();
        Rawboard EMPTY();
        Rawboard BOARD();

        static Rawboard posInd(Position position) {
            return 1LL << position;
        }
};
