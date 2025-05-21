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
    double static calculate(const Board& board) {
        double phase = TOTAL_PHASE;

        phase -= PAWN_PHASE * board.getPieceCount(WPawn);
        phase -= KNIGHT_PHASE * board.getPieceCount(WKnight);
        phase -= BISHOP_PHASE * board.getPieceCount(WBishop);
        phase -= ROOK_PHASE * board.getPieceCount(WRook);
        phase -= QUEEN_PHASE * board.getPieceCount(WQueen);
        phase -= PAWN_PHASE * board.getPieceCount(BPawn);
        phase -= KNIGHT_PHASE * board.getPieceCount(BKnight);
        phase -= BISHOP_PHASE * board.getPieceCount(BBishop);
        phase -= ROOK_PHASE * board.getPieceCount(BRook);
        phase -= QUEEN_PHASE * board.getPieceCount(BQueen);

        return (phase * 256 + (TOTAL_PHASE / 2)) / TOTAL_PHASE;
    }

    double static taperedEval(const Board& board, const double mgEvaluation, const double egEvaluation) {
        const double phase = calculate(board);
        return ((mgEvaluation * (256 - phase)) + (egEvaluation * phase)) / 256;
    }
};
