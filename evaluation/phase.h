#pragma once

#include "../board/board.h"

#define PAWN_PHASE 0
#define KNIGHT_PHASE 1
#define BISHOP_PHASE 1
#define ROOK_PHASE 2
#define QUEEN_PHASE 4
#define TOTAL_PHASE (PAWN_PHASE * 16 + KNIGHT_PHASE * 4 + BISHOP_PHASE * 4 + ROOK_PHASE * 4 + QUEEN_PHASE * 2)

class Phase {
public:
    double static calculate(const Board *board) {
        double phase = TOTAL_PHASE;

        phase -= PAWN_PHASE * getPieceCount(board, WPawn);
        phase -= KNIGHT_PHASE * getPieceCount(board, WKnight);
        phase -= BISHOP_PHASE * getPieceCount(board, WBishop);
        phase -= ROOK_PHASE * getPieceCount(board, WRook);
        phase -= QUEEN_PHASE * getPieceCount(board, WQueen);
        phase -= PAWN_PHASE * getPieceCount(board, BPawn);
        phase -= KNIGHT_PHASE * getPieceCount(board, BKnight);
        phase -= BISHOP_PHASE * getPieceCount(board, BBishop);
        phase -= ROOK_PHASE * getPieceCount(board, BRook);
        phase -= QUEEN_PHASE * getPieceCount(board, BQueen);

        return (phase * 256 + (TOTAL_PHASE / 2)) / TOTAL_PHASE;
    }

    double static taperedEval(const Board *board, const double mgEvaluation, const double egEvaluation) {
        const double phase = calculate(board);
        return ((mgEvaluation * (256 - phase)) + (egEvaluation * phase)) / 256;
    }
};
