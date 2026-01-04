#pragma once

#include "../common/types.h"
#include "../common/constants.h"
#include "../common/bitwise.h"
#include "../common/defines.h"
#include "staticAttacks.h"
#include "../utils/utils.h"

// ray attacks
#ifdef BOARD_STANDARD_RAY_ATTACKS
	inline Rawboard getPositiveRayAttacks(const Rawboard occupied, const unsigned int direction, const Position position) {
		Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		if (blocker) {
			const Position firstBlockPos = getFirstPos(blocker);
			attacks ^= staticRayAttacks[direction][firstBlockPos];
		}
		return attacks;
	}

	inline Rawboard getNegativeRayAttacks(const Rawboard occupied, const unsigned int direction, const Position position) {
		Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		if (blocker) {
			const Position firstBlockPos = getFirstPosReverse(blocker);
			attacks ^= staticRayAttacks[direction][firstBlockPos];
		}
		return attacks;
	}
#endif

#ifdef BOARD_BRANCHLESS_RAY_ATTACKS
	inline Rawboard getPositiveRayAttacks(const Rawboard occupied, const unsigned int direction, const Position position) {
		const Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ staticRayAttacks[direction][firstBlockPos];
	}

	inline Rawboard getNegativeRayAttacks(const Rawboard occupied, const unsigned int direction, const Position position) {
		const Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPosReverse(blocker | 1);
		return attacks ^ staticRayAttacks[direction][firstBlockPos];
	}
#endif

inline Rawboard noWestAttack(const Rawboard occupied, const Position position) {
	return getPositiveRayAttacks(occupied, NoWest, position);
}

inline Rawboard northAttack(const Rawboard occupied, const Position position) {
	return getPositiveRayAttacks(occupied, North, position);
}

inline Rawboard noEastAttack(const Rawboard occupied, const Position position) {
	return getPositiveRayAttacks(occupied, NoEast, position);
}

inline Rawboard eastAttack(const Rawboard occupied, const Position position) {
	return getPositiveRayAttacks(occupied, East, position);
}

inline Rawboard soEastAttack(const Rawboard occupied, const Position position) {
	return getNegativeRayAttacks(occupied, SoEast, position);
}

inline Rawboard southAttack(const Rawboard occupied, const Position position) {
	return getNegativeRayAttacks(occupied, South, position);
}

inline Rawboard soWestAttack(const Rawboard occupied, const Position position) {
	return getNegativeRayAttacks(occupied, SoWest, position);
}

inline Rawboard westAttack(const Rawboard occupied, const Position position) {
	return getNegativeRayAttacks(occupied, West, position);
}