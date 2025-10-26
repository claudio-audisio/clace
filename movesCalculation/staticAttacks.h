#pragma once

 #include "../common/types.h"
#include "../common/bitwise.h"

inline Rawboard staticRayAttacks[8][64];
inline Rawboard staticKnightAttacks[64];
inline Rawboard staticKingAttacks[64];

inline void initAttacks() {
    for (Position position = 0; position < 64; position++) {
        staticRayAttacks[North][position] = northRay(position);
        staticRayAttacks[NoEast][position] = noEastRay(position);
        staticRayAttacks[East][position] = eastRay(position);
        staticRayAttacks[SoEast][position] = soEastRay(position);
        staticRayAttacks[South][position] = southRay(position);
        staticRayAttacks[SoWest][position] = soWestRay(position);
        staticRayAttacks[West][position] = westRay(position);
        staticRayAttacks[NoWest][position] = noWestRay(position);

        const Rawboard posIndex = posInd(position);

        staticKnightAttacks[position] = noNoEa(posIndex) | noEaEa(posIndex) |
                                  soEaEa(posIndex) | soSoEa(posIndex) |
                                  soSoWe(posIndex) | soWeWe(posIndex) |
                                  noWeWe(posIndex) | noNoWe(posIndex);

        staticKingAttacks[position] = northOne(posIndex) | noEastOne(posIndex) |
                                eastOne(posIndex) | soEastOne(posIndex) |
                                southOne(posIndex) | soWestOne(posIndex) |
                                westOne(posIndex) | noWestOne(posIndex);
    }
}