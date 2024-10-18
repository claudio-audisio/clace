#pragma once

#include "types.h"
#include "constants.h"

Rawboard rayAttacks[8][64];
Rawboard knightAttacks[64];
Rawboard kingAttacks[64];

inline static Rawboard lineMask(const Position position) {
	return 0xFFLL << (position & 56);
}

inline static Rawboard columnMask(const Position position) {
	return 0x0101010101010101LL << (position & 7);
}

// Mask for right shift that add ones
inline static Rawboard RIGHT_SHIFT_MSK(const unsigned int len) {
	return len > 0 ? ~(0xffffffffffffffffLL << (64 - len)) : 0xffffffffffffffffLL;
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
