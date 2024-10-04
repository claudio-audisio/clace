#pragma once

#include "../common/types.h"


#define SIDE_GAP 1

const Piece Empty = 0;
const Piece WPawn = 1;
const Piece BPawn = WPawn + SIDE_GAP;
const Piece WKnight = 3;
const Piece BKnight = WKnight + SIDE_GAP;
const Piece WBishop = 5;
const Piece BBishop = WBishop + SIDE_GAP;
const Piece WRook = 7;
const Piece BRook = WRook + SIDE_GAP;
const Piece WQueen = 9;
const Piece BQueen = WQueen + SIDE_GAP;
const Piece WKing = 11;
const Piece BKing = WKing + SIDE_GAP;
