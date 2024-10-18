#include "boardUtils.h"

#include "../common/bitwise.h"

using namespace std;

extern Rawboard rayAttacks[8][64];
extern Rawboard knightAttacks[64];
extern Rawboard kingAttacks[64];

void BoardUtils::initAttacks() {
	for (Position position = 0; position < 64; position++) {
		rayAttacks[North][position] = northRay(position);
		rayAttacks[NoEast][position] = noEastRay(position);
		rayAttacks[East][position] = eastRay(position);
		rayAttacks[SoEast][position] = soEastRay(position);
		rayAttacks[South][position] = southRay(position);
		rayAttacks[SoWest][position] = soWestRay(position);
		rayAttacks[West][position] = westRay(position);
		rayAttacks[NoWest][position] = noWestRay(position);

		const Rawboard posIndex = posInd(position);

		knightAttacks[position] = noNoEa(posIndex) | noEaEa(posIndex) |
								  soEaEa(posIndex) | soSoEa(posIndex) |
								  soSoWe(posIndex) | soWeWe(posIndex) |
								  noWeWe(posIndex) | noNoWe(posIndex);

		kingAttacks[position] = northOne(posIndex) | noEastOne(posIndex) |
								eastOne(posIndex) | soEastOne(posIndex) |
								southOne(posIndex) | soWestOne(posIndex) |
								westOne(posIndex) | noWestOne(posIndex);
	}
}