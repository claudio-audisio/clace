#pragma once

#include <climits>
#include <vector>
#include <string>
#include <unordered_map>
#include <array>
#include <list>
#include <unordered_set>

#include "types.h"

using namespace std;


#define HvsC 0
#define CvsC 1

#define _WHITE 0
#define _BLACK 1
#define OPPOSITE(side) (_BLACK - side)
#define NO_POS 64
#define MAX_MOVES 218
#define HISTORY_MOVES 100

// pieces
constexpr Piece Empty = 0;
constexpr Piece WPawn = 1;
constexpr Piece BPawn = 2;
constexpr Piece WKnight = 3;
constexpr Piece BKnight = 4;
constexpr Piece WBishop = 5;
constexpr Piece BBishop = 6;
constexpr Piece WRook = 7;
constexpr Piece BRook = 8;
constexpr Piece WQueen = 9;
constexpr Piece BQueen = 10;
constexpr Piece WKing = 11;
constexpr Piece BKing = 12;
inline const unordered_map<Piece, string> PIECE_TO_STRING = {{WKing, "King"}, {WQueen, "Queen"}, {WRook, "Rook"}, {WKnight, "Knight"}, {WBishop, "Bishop"}, {WPawn, "Pawn"}, {BKing, "King"}, {BQueen, "Queen"}, {BRook, "Rook"}, {BKnight, "Knight"}, {BBishop, "Bishop"}, {BPawn, "Pawn"}};
inline const unordered_map<Piece, char> PIECE_TO_CODE = {{WKing, 0x004B}, {WQueen, 0x0051}, {WRook, 0x0052}, {WKnight, 0x004E}, {WBishop, 0x0062}, {WPawn, 0x0050}, {BKing, 0x006B}, {BQueen, 0x0071}, {BRook, 0x0072}, {BKnight, 0x006E}, {BBishop, 0x0062}, {BPawn, 0x0070}, {Empty, 0x002E}};

// board masks
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
#define ROW_4 0x00000000ff000000LL
#define ROW_5 0x000000ff00000000LL
#define MAIN_DIAG 0x8040201008040201LL
#define MAIN_ANTI_DIAG 0x0102040810204080LL

// rays
#define North   0
#define NoEast  1
#define East    2
#define SoEast  3
#define South   4
#define SoWest  5
#define West    6
#define NoWest  7

// move type
#define NORMAL      0
#define CASTLING    1
#define EN_PASSANT  2
#define PROMOTION   4

// move mask
#define SOURCE_POS_MASK	0x00000000000000ffLL
#define DEST_POS_MASK	0x000000000000ff00LL
#define PIECE_MASK		0x0000000000ff0000LL
#define PROMOTION_MASK	0x00000000ff000000LL
#define CAPTURED_MASK	0x000000ff00000000LL
#define SIDE_MASK		0x0000010000000000LL
#define CASTLING_MASK	0x0000020000000000LL
#define EN_PASSANT_MASK	0x0000040000000000LL
#define PAWN_PROM_MASK	0x0000080000000000LL
#define MOVE_TYPE_MASK	0x00000E0000000000LL

// castling
#define WQCastling	6
#define WKCastling	10
#define BQCastling	118
#define BKCastling	122
#define WQCastlingInfo	0b0100
#define WKCastlingInfo	0b1000
#define BQCastlingInfo	0b0001
#define BKCastlingInfo	0b0010
#define WC_King	0x10
#define BC_King	0x1000000000000000
#define WQC_King 0x4
#define WQC_RookRem 0xfffffffffffffffe
#define WQC_RookAdd 0x8
#define WQC_EmptyRem 0xfffffffffffffff3
#define WQC_EmptyAdd 0x11
#define WKC_King 0x40
#define WKC_RookRem 0xffffffffffffff7f
#define WKC_RookAdd 0x20
#define WKC_EmptyRem 0xffffffffffffff9f
#define WKC_EmptyAdd 0x90
#define BQC_King 0x400000000000000
#define BQC_RookRem 0xfeffffffffffffff
#define BQC_RookAdd 0x800000000000000
#define BQC_EmptyRem 0xf3ffffffffffffff
#define BQC_EmptyAdd 0x1100000000000000
#define BKC_King 0x4000000000000000
#define BKC_RookRem 0x7fffffffffffffff
#define BKC_RookAdd 0x2000000000000000
#define BKC_EmptyRem 0x9fffffffffffffff
#define BKC_EmptyAdd 0x9000000000000000
inline constexpr CastlingInfo CASTLING_INFO_MASK[64] = {0b1011, 0b1111, 0b1111, 0b1111, 0b0011, 0b1111, 0b1111, 0b0111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1111, 0b1110, 0b1111, 0b1111, 0b1111, 0b1100, 0b1111, 0b1111, 0b1101};

// xray pieces
inline unordered_map<Side, const unordered_set<Piece>> XRAY_PIECES = {{_WHITE, { WRook, WBishop, WQueen }}, {_BLACK, { BRook, BBishop, BQueen }}};

// fen
inline const vector<string> PIECE_TO_FEN = {"", "P", "p", "N", "n", "B", "b", "R", "r", "Q", "q", "K", "k"};
inline const unordered_map<char, Piece> FEN_TO_PIECE = {{'K', WKing}, {'Q', WQueen}, {'R', WRook}, {'N', WKnight}, {'B', WBishop}, {'P', WPawn}, {'k', BKing}, {'q', BQueen}, {'r', BRook}, {'n', BKnight}, {'b', BBishop}, {'p', BPawn}};
inline const vector<string> EMPTY_FEN = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};

// positions
inline const string INITIAL_FEN_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
inline const string CASTLING_FEN_POSITION = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";
inline const string PERFT_FEN_POSITION_2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
inline const string PERFT_FEN_POSITION_2_BLACK_TO_MOVE = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1";
inline const string PERFT_FEN_POSITION_3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
inline const string PERFT_FEN_POSITION_4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
inline const string PERFT_FEN_POSITION_4_MIRRORED = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";
inline const string PERFT_FEN_POSITION_5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
inline const string PERFT_FEN_POSITION_6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
inline const string END_FEN_POSITION = "4k3/8/8/8/8/8/8/4K3 w - - 0 1";
inline const string FEN_EN_PASSANT_LEGALITY_TEST = "8/6bb/8/8/R1pP2k1/4P3/P7/K7 b - d3 0 0";
inline const string FEN_EN_PASSANT_BUG_TEST = "2r3k1/1q1nbppp/r3p3/3pP3/pPpP4/P1Q2N2/2RN1PPP/2R4K b - b3 0 23";


// evaluation weights
#define KING_WT     200.0
#define QUEEN_WT    9.0
#define ROOK_WT     5.0
#define BISHOP_WT   3.5
#define KNIGHT_WT   3.0
#define PAWN_WT     1.0
//#define PAWN_WT 1.0	TODO peso per doubled, blocked and isolated pawns
#define WIN_VALUE (SHRT_MAX / 2.0)
#define LOSS_VALUE (-WIN_VALUE)
#define DRAW_VALUE 0
#define EVAL_WIN_VALUE (9 * QUEEN_WT + 2 * ROOK_WT + 2 * BISHOP_WT + 2 * KNIGHT_WT)
#define EVAL_LOSS_VALUE (-EVAL_WIN_VALUE)
#define MATERIAL_WT 0.9
#define MOBILITY_WT 0.1
#define WIN_EVALUATION Evaluation(0, WIN_VALUE)
#define LOSS_EVALUATION Evaluation(0, LOSS_VALUE)
#define DRAW_EVALUATION Evaluation(0, DRAW_VALUE)

// end game types
#define NONE 0
#define FIFTY_MOVE_RULE 1
// NOTE While castling is irreversible with respect to the castling rights and resets the position index to determine repetition of positions,
// castling doesn't reset, but increments the halfmove clock concerning the fifty-move rule [2] [3].
#define FIVEFOLD_REPETITION 2
#define STALEMATE 3
#define CHECKMATE 4

// messaging types
#define MSG_ALL 0
#define MSG_LOG 1
#define MSG_GUI 2
