#pragma once

#include "types.h"
#include "constants.h"

Rawboard rayAttacks[8][64];

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
