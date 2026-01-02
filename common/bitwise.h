#pragma once

#include "types.h"
#include "constants.h"

inline Rawboard posInd(const Position position) {
	return 1LL << position;
}

inline Rawboard lineMask(const Position position) {
	return 0xFFLL << (position & 56);
}

inline Rawboard columnMask(const Position position) {
	return 0x0101010101010101LL << (position & 7);
}

inline Rawboard diagonalMask(const Position sq) {
	const int diag = 8 * (sq & 7) - (sq & 56);
	const int north = -diag & (diag >> 31);
	const int south = diag & (-diag >> 31);
	return (MAIN_DIAG >> south) << north;
}

inline Rawboard antiDiagonalMask(const Position sq) {
	const int diag = 56 - 8 * (sq & 7) - (sq & 56);
	const int north = -diag & (diag >> 31);
	const int south = diag & (-diag >> 31);
	return (MAIN_ANTI_DIAG >> south) << north;
}

inline Rawboard eastRay(const Position position) {
	return lineMask(position) & (((Rawboard)-2) << position);
}

inline Rawboard westRay(const Position position) {
	return lineMask(position) & ((((Rawboard)1) << position) - 1);
}

inline Rawboard northRay(const Position position) {
	return columnMask(position) & (((Rawboard)-2) << position);
}

inline Rawboard southRay(const Position position) {
	return columnMask(position) & ((((Rawboard)1) << position) - 1);
}

inline Rawboard soEastRay(const Position position) {
	return antiDiagonalMask(position) & (((Rawboard)1) << position) - 1;
}

inline Rawboard noWestRay(const Position position) {
	return antiDiagonalMask(position) & (((Rawboard)-2) << position);
}

inline Rawboard soWestRay(const Position position) {
	return diagonalMask(position) & (((Rawboard)1) << position) - 1;
}

inline Rawboard noEastRay(const Position position) {
	return diagonalMask(position) & (((Rawboard)-2) << position);
}

// Knight step
inline Rawboard noNoEa(const Rawboard start) {
	return (start << 17) & NOT_A_COL;
}

inline Rawboard noEaEa(const Rawboard start) {
	return (start << 10) & NOT_AB_COL;
}

inline Rawboard soEaEa(const Rawboard start) {
	return (start >> 6) & NOT_AB_COL;
}

inline Rawboard soSoEa(const Rawboard start) {
	return (start >> 15) & NOT_A_COL;
}

inline Rawboard noNoWe(const Rawboard start) {
	return (start << 15) & NOT_H_COL;
}

inline Rawboard noWeWe(const Rawboard start) {
	return (start << 6) & NOT_GH_COL;
}

inline Rawboard soWeWe(const Rawboard start) {
	return (start >> 10) & NOT_GH_COL;
}

inline Rawboard soSoWe(const Rawboard start) {
	return (start >> 17) & NOT_H_COL;
}

// One step
inline Rawboard northOne(const Rawboard start) {
	return start << 8;
}

inline Rawboard noEastOne(const Rawboard start) {
	return (start << 9) & NOT_A_COL;
}

inline Rawboard eastOne(const Rawboard start) {
	return (start << 1) & NOT_A_COL;
}

inline Rawboard soEastOne(const Rawboard start) {
	return (start >> 7) & NOT_A_COL;
}

inline Rawboard southOne(const Rawboard start) {
	return start >> 8;
}

inline Rawboard soWestOne(const Rawboard start) {
	return (start >> 9) & NOT_H_COL;
}

inline Rawboard westOne(const Rawboard start) {
	return (start >> 1) & NOT_H_COL;
}

inline Rawboard noWestOne(const Rawboard start) {
	return (start << 7) & NOT_H_COL;
}
