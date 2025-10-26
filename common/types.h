#pragma once

//#include <bitset>

using namespace std;

typedef unsigned char EndGameType;
typedef unsigned char GameType;
typedef unsigned char Side;
typedef unsigned char Position;
typedef unsigned char RawboardIndex;
typedef unsigned long long Rawboard;
typedef unsigned char Piece;
typedef unsigned char CastlingInfo;
typedef unsigned long long unsLL;
//typedef bitset<72> PieceAndDest;
typedef unsigned long long Move;
typedef unsigned char MoveType;
typedef unsigned char MoveResult;

typedef struct {
	unsigned char total;
	unsigned char legal;
} MovesAmount;

typedef struct {
	Move move;
	double value;
	EndGameType endGameType;
} Evaluation;