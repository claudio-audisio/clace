#pragma once

#include <climits>
#include <vector>
#include <string>
#include <unordered_map>

#include "../board/piece.h"

using namespace std;


#define WHITE 0
#define BLACK (WHITE + SIDE_GAP)

#define NO_POS 64
#define KING_WT 200.0
#define QUEEN_WT 9.0
#define ROOK_WT 5.0
#define BISHOP_WT 3.5
#define KNIGHT_WT 3.0
#define PAWN_WT 1.0
//#define PAWN_WT 1.0	TODO peso per doubled, blocked and isolated pawns
#define WIN_VALUE (SHRT_MAX / 2.0)
#define LOSS_VALUE (-WIN_VALUE)
#define DRAW_VALUE 0
#define EVAL_WIN_VALUE (9 * QUEEN_WT + 2 * ROOK_WT + 2 * BISHOP_WT + 2 * KNIGHT_WT)
#define EVAL_LOSS_VALUE (-EVAL_WIN_VALUE)

#define SIZE 13
#define EMPTY_IND 0    // This is related to Piece.Empty value
#define EMPTY_BOARD 0xffffffffffffffffLL
#define NOT_A_COL 0xfefefefefefefefeLL
#define NOT_AB_COL 0xfcfcfcfcfcfcfcfcLL
#define NOT_H_COL 0x7f7f7f7f7f7f7f7fLL
#define NOT_GH_COL 0x3f3f3f3f3f3f3f3fLL
#define SH_7DX_MSK 0x1ffffffffffffffLL
#define SH_1DX_MSK 0x7fffffffffffffffLL
#define SH_9DX_MSK 0x7fffffffffffffLL
#define SH_8DX_MSK 0xffffffffffffffLL
#define SH_15DX_MSK 0x1ffffffffffffLL
#define SH_6DX_MSK 0x3ffffffffffffffLL
#define SH_17DX_MSK 0x7fffffffffffLL
#define SH_10DX_MSK 0x3fffffffffffffLL
#define ROW_5 0x00000000ff000000LL
#define ROW_4 0x000000ff00000000LL
#define MAIN_DIAG 0x8040201008040201LL
#define MAIN_ANTI_DIAG 0x0102040810204080LL
#define MAX_MOVES 218
#define North 0
#define NoEast 1
#define East 2
#define SoEast 3
#define South 4
#define SoWest 5
#define West 6
#define NoWest 7

static const vector<string> PIECE_TO_FEN = {"", "P", "p", "N", "n", "B", "b", "R", "r", "Q", "q", "K", "k"};
static const unordered_map<char, Piece> FEN_TO_PIECE = {{'K', WKing}, {'Q', WQueen}, {'R', WRook}, {'N', WKnight}, {'B', WBishop}, {'P', WPawn}, {'k', BKing}, {'q', BQueen}, {'r', BRook}, {'n', BKnight}, {'b', BBishop}, {'p', BPawn}};
static const vector<string> EMPTY_FEN = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};


