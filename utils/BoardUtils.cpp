#include "boardUtils.h"

#include "../common/bitwise.h"

using namespace std;

extern Rawboard rayAttacks[8][64];

void BoardUtils::initRayAttacks() {
	for (int position = 0; position < 64; position++) {
		rayAttacks[North][position] = northRay(position);
		rayAttacks[NoEast][position] = noEastRay(position);
		rayAttacks[East][position] = eastRay(position);
		rayAttacks[SoEast][position] = soEastRay(position);
		rayAttacks[South][position] = southRay(position);
		rayAttacks[SoWest][position] = soWestRay(position);
		rayAttacks[West][position] = westRay(position);
		rayAttacks[NoWest][position] = noWestRay(position);
	}
}