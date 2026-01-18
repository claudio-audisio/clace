#pragma once

#include "types.h"


typedef struct {
    Rawboard pieceBoards[13];
    Piece piecePositions[64];
    CastlingInfo castlingInfo;
    Position enPassantPosition;
} Board;

typedef struct {
    unsigned int total;
    unsigned int legal;
} MovesAmount;

typedef struct {
    Board board;
    Side sideToMove;
    unsigned int fullMoves;
    unsigned int halfMoveClock;
    unsLL key;
} GameSnapshot;

typedef struct {
    Move move;
    double value;
    EndGameType endGameType;
} Evaluation;

typedef struct {
    unsLL pieces[13][64];
    unsLL sideToMove;
    unsLL castling[4];
    unsLL enPassant[8];
} Randoms;

typedef struct {
    Move *moves;
    MovesAmount amount;
} Transposition;

