#pragma once

#include "../common/types.h"
#include "../common/constants.h"
#include "../common/bitwise.h"
#include "../common/defines.h"
#include "staticAttacks.h"
#include "../utils/utils.h"

// ray attacks
#ifdef BOARD_STANDARD_RAY_ATTACKS
	inline Rawboard s_getPositiveRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		if (blocker) {
			const Position firstBlockPos = getFirstPosReverse(blocker);
			attacks ^= staticRayAttacks[direction][firstBlockPos];
		}
		return attacks;
	}

	inline Rawboard s_getNegativeRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		if (blocker) {
			const Position firstBlockPos = getFirstPos(blocker);
			attacks ^= staticRayAttacks[direction][firstBlockPos];
		}
		return attacks;
	}
#endif

#ifdef BOARD_BRANCHLESS_RAY_ATTACKS
	static inline Rawboard s_getPositiveRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		const Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPosReverse(blocker | 1);
		return attacks ^ staticRayAttacks[direction][firstBlockPos];
	}

	static inline Rawboard s_getNegativeRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		const Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ staticRayAttacks[direction][firstBlockPos];
	}
#endif

#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	static Rawboard s_getPositiveRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPosReverse(blocker | 1);
		return attacks ^ direction(firstBlockPos);
	}

	static Rawboard s_getNegativeRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ direction(firstBlockPos);
	}
#endif

inline Rawboard s_noWestAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getPositiveRayAttacks(occupied, noWestRay, position);
#else
	return s_getPositiveRayAttacks(occupied, NoWest, position);
#endif
}

inline Rawboard s_northAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getPositiveRayAttacks(occupied, northRay, position);
#else
	return s_getPositiveRayAttacks(occupied, North, position);
#endif
}

inline Rawboard s_noEastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getPositiveRayAttacks(occupied, noEastRay, position);
#else
	return s_getPositiveRayAttacks(occupied, NoEast, position);
#endif
}

inline Rawboard s_eastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getNegativeRayAttacks(occupied, eastRay, position);
#else
	return s_getNegativeRayAttacks(occupied, East, position);
#endif
}

inline Rawboard s_soEastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getNegativeRayAttacks(occupied, soEastRay, position);
#else
	return s_getNegativeRayAttacks(occupied, SoEast, position);
#endif
}

inline Rawboard s_southAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getNegativeRayAttacks(occupied, southRay, position);
#else
	return s_getNegativeRayAttacks(occupied, South, position);
#endif
}

inline Rawboard s_soWestAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getNegativeRayAttacks(occupied, soWestRay, position);
#else
	return s_getNegativeRayAttacks(occupied, SoWest, position);
#endif
}

inline Rawboard s_westAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getPositiveRayAttacks(occupied, westRay, position);
#else
	return s_getPositiveRayAttacks(occupied, West, position);
#endif
}