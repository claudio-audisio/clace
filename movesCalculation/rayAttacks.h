#pragma once

#include "../common/types.h"
#include "../common/constants.h"
#include "../common/bitwise.h"
#include "../common/defines.h"
#include "staticAttacks.h"
#include "../utils/utils.h"

// ray attacks
#ifdef BOARD_STANDARD_RAY_ATTACKS
	inline Rawboard getPositiveRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		if (blocker) {
			const Position firstBlockPos = getFirstPosReverse(blocker);
			attacks ^= staticRayAttacks[direction][firstBlockPos];
		}
		return attacks;
	}

	inline Rawboard getNegativeRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
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
	static inline Rawboard getPositiveRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		const Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPosReverse(blocker | 1);
		return attacks ^ staticRayAttacks[direction][firstBlockPos];
	}

	static inline Rawboard getNegativeRayAttacks(const Rawboard occupied, const unsigned char direction, const Position position) {
		const Rawboard attacks = staticRayAttacks[direction][position];
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ staticRayAttacks[direction][firstBlockPos];
	}
#endif

#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	static Rawboard getPositiveRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPosReverse(blocker | 1);
		return attacks ^ direction(firstBlockPos);
	}

	static Rawboard getNegativeRayAttacks(const Rawboard occupied, Rawboard(*direction)(Position), const Position position) {
		const Rawboard attacks = direction(position);
		const Rawboard blocker = attacks & occupied;
		const Position firstBlockPos = getFirstPos(blocker | 0x8000000000000000LL);
		return attacks ^ direction(firstBlockPos);
	}
#endif

inline Rawboard noWestAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getPositiveRayAttacks(occupied, noWestRay, position);
#else
	return getPositiveRayAttacks(occupied, NoWest, position);
#endif
}

inline Rawboard northAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getPositiveRayAttacks(occupied, northRay, position);
#else
	return getPositiveRayAttacks(occupied, North, position);
#endif
}

inline Rawboard noEastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getPositiveRayAttacks(occupied, noEastRay, position);
#else
	return getPositiveRayAttacks(occupied, NoEast, position);
#endif
}

inline Rawboard eastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getNegativeRayAttacks(occupied, eastRay, position);
#else
	return getNegativeRayAttacks(occupied, East, position);
#endif
}

inline Rawboard soEastAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getNegativeRayAttacks(occupied, soEastRay, position);
#else
	return getNegativeRayAttacks(occupied, SoEast, position);
#endif
}

inline Rawboard southAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getNegativeRayAttacks(occupied, southRay, position);
#else
	return getNegativeRayAttacks(occupied, South, position);
#endif
}

inline Rawboard soWestAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getNegativeRayAttacks(occupied, soWestRay, position);
#else
	return getNegativeRayAttacks(occupied, SoWest, position);
#endif
}

inline Rawboard westAttack(const Rawboard occupied, const Position position) {
#ifdef BOARD_ONTHEFLY_RAY_ATTACKS
	return getPositiveRayAttacks(occupied, westRay, position);
#else
	return getPositiveRayAttacks(occupied, West, position);
#endif
}