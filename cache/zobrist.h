#pragma once

#include <climits>
#include <random>

#include "../common/types.h"
#include "../game/game.h"
#include "../move/move.h"

using namespace std;


inline void initRandoms(Randoms& randoms) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<unsLL> distr(0, ULONG_LONG_MAX);

    for (int i = 0; i < 13; i++) {
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

inline unsLL calculateKey(const Game& game) {
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

    if (board.castlingInfo & BQCastlingInfo) {
        index ^= randoms.castling[0];
    }

    if (board.castlingInfo & BKCastlingInfo) {
        index ^= randoms.castling[1];
    }

    if (board.castlingInfo & WQCastlingInfo) {
        index ^= randoms.castling[2];
    }

    if (board.castlingInfo & WKCastlingInfo) {
        index ^= randoms.castling[3];
    }

    if (board.enPassantPosition != NO_POS) {
        index ^= randoms.enPassant[board.enPassantPosition % 8];
    }

    return index;
}

inline void updateKey(const Game& game, const Move move, const CastlingInfo prevCastlingInfo, const Position prevEnPassant) {
    const Board& board = *game.board;
    const Randoms& randoms = game.randoms;
    const Piece piece = getPiece(move);
    const Position sourcePos = getSourcePosition(move);
    const Position destPos = getDestinationPosition(move);

    game.key ^= randoms.pieces[piece][sourcePos];

    if (!isPawnPromotion(move)) {
        game.key ^= randoms.pieces[piece][destPos];
    } else {
        game.key ^= randoms.pieces[getPromotion(move)][destPos];
    }

    game.key ^= randoms.sideToMove;

    const Piece captured = getCaptured(move);

    if (captured) {
        if (isEnPassant(move)) {
            game.key ^= randoms.pieces[captured][destPos - 24 + piece * 16];
        } else {
            game.key ^= randoms.pieces[captured][destPos];
        }
    } else if (isCastling(move)) {
        switch (sourcePos + destPos) {
            case BQCastling:
                game.key ^= randoms.pieces[BRook][56];
                game.key ^= randoms.pieces[BRook][59];
                break;
            case BKCastling:
                game.key ^= randoms.pieces[BRook][61];
                game.key ^= randoms.pieces[BRook][63];
                break;
            case WQCastling:
                game.key ^= randoms.pieces[WRook][0];
                game.key ^= randoms.pieces[WRook][3];
                break;
            case WKCastling:
                game.key ^= randoms.pieces[WRook][5];
                game.key ^= randoms.pieces[WRook][7];
                break;
            default: throw runtime_error("wrong castling move");
        }
    }

    const CastlingInfo castlingInfoChanges = board.castlingInfo ^ prevCastlingInfo;

    if (castlingInfoChanges) {
        if (castlingInfoChanges & BQCastlingInfo) {
            game.key ^= randoms.castling[0];
        }

        if (castlingInfoChanges & BKCastlingInfo) {
            game.key ^= randoms.castling[1];
        }

        if (castlingInfoChanges & WQCastlingInfo) {
            game.key ^= randoms.castling[2];
        }

        if (castlingInfoChanges & WKCastlingInfo) {
            game.key ^= randoms.castling[3];
        }
    }

    if (prevEnPassant != NO_POS) {
        game.key ^= randoms.enPassant[prevEnPassant % 8];
    }

    if (board.enPassantPosition != NO_POS) {
        game.key ^= randoms.enPassant[board.enPassantPosition % 8];
    }

}

