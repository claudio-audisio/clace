#pragma once


#include "../board/board.h"

typedef struct {
    Board board;
    Side sideToMove;
    unsigned int fullMoves;
    unsigned int halfMoveClock;
} GameSnapshot;

inline GameSnapshot** allocateSnapshots(const unsigned int size) {
    const auto snapshots = static_cast<GameSnapshot**>(malloc(sizeof(GameSnapshot*) * size));

    for (int i = 0; i < size; i++) {
        snapshots[i] = static_cast<GameSnapshot*>(malloc(sizeof(GameSnapshot)));
    }

    return snapshots;
}

inline void deallocateSnapshots(GameSnapshot** snapshots, const unsigned int size) {
    for (int i = 0; i < size; i++) {
        free(snapshots[i]);
    }

    free(snapshots);
}

inline void saveSnapshot(const Board* board, const unsigned int sideToMove, const unsigned int fullMoves, const unsigned int halfMoveClock, GameSnapshot** snapshots, const unsigned int index) {
    GameSnapshot *snapshot = snapshots[index];
    copy(board, &snapshot->board);
    snapshot->sideToMove = sideToMove;
    snapshot->fullMoves = fullMoves;
    snapshot->halfMoveClock = halfMoveClock;
}

inline void loadSnapshot(Board *board, unsigned int &sideToMove, unsigned int &fullMoves, unsigned int &halfMoveClock, GameSnapshot** snapshots, const unsigned int index) {
    const GameSnapshot *snapshot = snapshots[index];
    copy(&snapshot->board, board);
    sideToMove = snapshot->sideToMove;
    fullMoves = snapshot->fullMoves;
    halfMoveClock = snapshot->halfMoveClock;
}
