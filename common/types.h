#pragma once

//#include <bitset>

using namespace std;

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
typedef unsigned char MoveResult;
typedef pair<Move, double> Evaluation;

enum class EndGameType
{
	FIFTY_MOVE_RULE,
	// NOTE While castling is irreversible with respect to the castling rights and resets the position index to determine repetition of positions,
    // castling doesn't reset, but increments the halfmove clock concerning the fifty-move rule [2] [3].
	FIVEFOLD_REPETITION,
	STALEMATE,
	CHECKMATE,
	NONE
};