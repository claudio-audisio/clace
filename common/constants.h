#pragma once

#include <climits>

const int NO_POS = 64;
const double KING_WT = 200.0;
const double QUEEN_WT = 9.0;
const double ROOK_WT = 5.0;
const double BISHOP_WT = 3.5;
const double KNIGHT_WT = 3.0;
const double PAWN_WT = 1.0;
//const double PAWN_WT = 1.0;	TODO peso per doubled, blocked and isolated pawns
const double WIN_VALUE = SHRT_MAX / 2.0;
const double LOSS_VALUE = -WIN_VALUE;
const double DRAW_VALUE = 0;
const double EVAL_WIN_VALUE = 9 * QUEEN_WT + 2 * ROOK_WT + 2 * BISHOP_WT + 2 * KNIGHT_WT;
const double EVAL_LOSS_VALUE = -EVAL_WIN_VALUE;