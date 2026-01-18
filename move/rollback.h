#pragma once


#include "../board/board.h"


inline GameSnapshot** allocateSnapshots(const unsigned int size) {
    const auto snapshots = static_cast<GameSnapshot**>(malloc(sizeof(GameSnapshot*) * size));

    if (snapshots == nullptr) {
        throw runtime_error("malloc failed");
    }

    for (int i = 0; i < size; i++) {
        snapshots[i] = static_cast<GameSnapshot*>(malloc(sizeof(GameSnapshot)));

        if (snapshots[i] == NULL) {
            throw runtime_error("malloc failed");
        }
    }

    return snapshots;
}

inline void deallocateSnapshots(GameSnapshot** snapshots, const unsigned int size) {
    for (int i = 0; i < size; i++) {
        free(snapshots[i]);
    }

    free(snapshots);
}

inline void saveSnapshot(const Board* board, const unsigned int sideToMove, const unsigned int fullMoves, const unsigned int halfMoveClock, const unsLL key, GameSnapshot** snapshots, const unsigned int index) {
    GameSnapshot *snapshot = snapshots[index];
    copy(board, &snapshot->board);
    snapshot->sideToMove = sideToMove;
    snapshot->fullMoves = fullMoves;
    snapshot->halfMoveClock = halfMoveClock;
    snapshot->key = key;
}

inline void loadSnapshot(Board *board, unsigned int &sideToMove, unsigned int &fullMoves, unsigned int &halfMoveClock, unsLL &key, GameSnapshot** snapshots, const unsigned int index) {
    const GameSnapshot *snapshot = snapshots[index];
    copy(&snapshot->board, board);
    sideToMove = snapshot->sideToMove;
    fullMoves = snapshot->fullMoves;
    halfMoveClock = snapshot->halfMoveClock;
    key = snapshot->key;
}
