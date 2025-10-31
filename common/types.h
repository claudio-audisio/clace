#pragma once

//#include <bitset>

using namespace std;

typedef unsigned int EndGameType;
typedef unsigned int GameType;
typedef unsigned int Side;
typedef unsigned int Position;
typedef unsigned int RawboardIndex;
typedef unsigned long long Rawboard;
typedef unsigned int Piece;
typedef unsigned int CastlingInfo;
typedef unsigned long long unsLL;
//typedef bitset<72> PieceAndDest;
typedef unsigned long long Move;
typedef unsigned int MoveType;
typedef unsigned int MoveResult;

typedef struct {
	unsigned int total;
	unsigned int legal;
} MovesAmount;

typedef struct {
	Move move;
	double value;
	EndGameType endGameType;
} Evaluation;