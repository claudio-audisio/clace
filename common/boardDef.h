#pragma once

#include "types.h"

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

/*#define posInd(position) (1LL << (position))
#define RIGHT_SHIFT_MSK(len) (len > 0 ? ~(0xffffffffffffffffLL << (64 - len)) : 0xffffffffffffffffLL)
#define noNoEa(start) ((start >> 15) & NOT_A_COL & SH_15DX_MSK)
#define noEaEa(start) ((start >> 6) & NOT_AB_COL & SH_6DX_MSK)
#define soEaEa(start) ((start << 10) & NOT_AB_COL)
#define soSoEa(start) ((start << 17) & NOT_A_COL)
#define noNoWe(start) ((start >> 17) & NOT_H_COL & SH_17DX_MSK)
#define noWeWe(start) ((start >> 10) & NOT_GH_COL & SH_10DX_MSK)
#define soWeWe(start) ((start << 6) & NOT_GH_COL)
#define soSoWe(start) ((start << 15) & NOT_H_COL)*/

/*#define northOne [](Rawboard start) { return (start >> 8) & SH_8DX_MSK; }
#define noEastOne [](Rawboard start) { return (start >> 7) & NOT_A_COL & SH_7DX_MSK; }
#define eastOne [](Rawboard start) { return (start << 1) & NOT_A_COL; }
#define soEastOne [](Rawboard start) { return (start << 9) & NOT_A_COL; }
#define southOne [](Rawboard start) { return start << 8; }
#define soWestOne [](Rawboard start) { return (start << 7) & NOT_H_COL; }
#define westOne [](Rawboard start) { return (start >> 1) & NOT_H_COL & SH_1DX_MSK; }
#define noWestOne [](Rawboard start) { return (start >> 9) & NOT_H_COL & SH_9DX_MSK; }*/
/*#define lineMask(position) (0xFFLL << (position & 56))
#define columnMask(position) (0x0101010101010101LL << (position & 7))*/
