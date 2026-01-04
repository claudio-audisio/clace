#pragma once

#include <climits>
#include <random>

#include "../common/types.h"
#include "../game/game.h"

using namespace std;


inline void initRandoms(Randoms& randoms) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<unsLL> distr(0, ULONG_LONG_MAX);

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 64; j++) {
            randoms.pieces[i][j] = distr(gen);
        }
    }

    randoms.sideToMove = distr(gen);

    for (int i = 0; i < 4; i++) {
        randoms.castling[i] = distr(gen);
    }

    for (int i = 0; i < 8; i++) {
        randoms.enPassant[i] = distr(gen);
    }
}

inline unsLL getZobristKey(const Game& game) {
    unsLL index = 0;
    const Board& board = *game.board;
    const Randoms& randoms = game.randoms;

    for (int i = 0; i < 64; i++) {
        if (board.piecePositions[i]) {
            index ^= randoms.pieces[board.piecePositions[i]][i];
        }
    }

    if (game.sideToMove) {
        index ^= randoms.sideToMove;
    }

    if (board.castlingInfo & 0b0001) {
        index ^= randoms.castling[0];
    }

    if (board.castlingInfo & 0b0010) {
        index ^= randoms.castling[1];
    }

    if (board.castlingInfo & 0b0100) {
        index ^= randoms.castling[2];
    }

    if (board.castlingInfo & 0b1000) {
        index ^= randoms.castling[3];
    }

    if (board.enPassantPosition != NO_POS) {
        index ^= randoms.enPassant[board.enPassantPosition % 8];
    }

    return index;
}