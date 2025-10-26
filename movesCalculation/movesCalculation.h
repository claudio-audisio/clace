#pragma once

#include "../board/board.h"
#include "../common/types.h"
#include "../common/constants.h"
#include "../common/bitwise.h"
#include "rayAttacks.h"

inline Rawboard s_whiteEnPassantMove(const Board& board, const Rawboard position) {
    if (board.enPassantPosition != NO_POS && (position & ROW_5) != 0) {
        const Rawboard passantPos = posInd(board.enPassantPosition);

        if (passantPos == noEastOne(position) || passantPos == noWestOne(position)) {
            return passantPos;
        }
    }

    return 0;
}

inline Rawboard s_blackEnPassantMove(const Board& board, const Rawboard position) {
    if (board.enPassantPosition != NO_POS && (position & ROW_4) != 0) {
        const Rawboard passantPos = posInd(board.enPassantPosition);

        if (passantPos == soEastOne(position) || passantPos == soWestOne(position)) {
            return passantPos;
        }
    }

    return 0;
}

inline Rawboard s_whitePawnMoves(const Board& board, const Position position, const Side side) {
    const Rawboard posIndex = posInd(position);
    const Rawboard onePush = northOne(posIndex) & board.EMPTY;

    return onePush | (northOne(onePush) & board.EMPTY & ROW_4) |
           ((noWestOne(posIndex) | noEastOne(posIndex)) & board.OPP_PIECES(_WHITE)) |
                s_whiteEnPassantMove(board, posIndex);
}

inline Rawboard s_blackPawnMoves(const Board& board, const Position position, const Side side) {
    const Rawboard posIndex = posInd(position);
    const Rawboard onePush = southOne(posIndex) & board.EMPTY;

    return onePush | (southOne(onePush) & board.EMPTY & ROW_5) |
           ((soWestOne(posIndex) | soEastOne(posIndex)) & board.OPP_PIECES(_BLACK)) |
               s_blackEnPassantMove(board, posIndex);
}

inline Rawboard s_allPawnMoves(const Board& board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board.pieceBoards[WPawn + side];

    if (side) {
        while (pieces) {
            const Position position = getFirstPos(pieces);
            attacks |= s_blackPawnMoves(board, position, side);
            pieces &= (pieces - 1);
        }
    } else {
        while (pieces) {
            const Position position = getFirstPos(pieces);
            attacks |= s_whitePawnMoves(board, position, side);
            pieces &= (pieces - 1);
        }
    }

    return attacks;
}

inline Rawboard s_allPawnAttacks(const Board& board, const Side side) {
    const Rawboard pieces = board.pieceBoards[WPawn + side];

    if (!side) {
        return (noWestOne(pieces) | noEastOne(pieces)) & (board.EMPTY | board.OPP_PIECES(side));
    }
    else {
        return (soWestOne(pieces) | soEastOne(pieces)) & (board.EMPTY | board.OPP_PIECES(side));
    }
}

inline Rawboard s_pawnAttacks(const Board& board, const Position position, const Side side) {
    const Rawboard posIndex = posInd(position);

    if (!side) {
        return (noWestOne(posIndex) | noEastOne(posIndex)) & (board.EMPTY | board.OPP_PIECES(side));
    }
    else {
        return (soWestOne(posIndex) | soEastOne(posIndex)) & (board.EMPTY | board.OPP_PIECES(side));
    }
}

inline Rawboard s__rookAttacks(const Position position, const Rawboard occupied, const Rawboard notPieces) {
	return (s_northAttack(occupied, position) | s_eastAttack(occupied, position) | s_southAttack(occupied, position) | s_westAttack(occupied, position)) & notPieces;
}

inline Rawboard s_rookAttacks(const Board& board, const Position position, const Side side) {
    return s__rookAttacks(position, ~board.EMPTY, ~board.PIECES(side));
}

inline Rawboard s_allRookAttacks(const Board& board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board.pieceBoards[WRook + side];
    const Rawboard occupied = ~board.EMPTY;
    const Rawboard notPieces = ~board.PIECES(side);

    while (pieces) {
        const Position position = getFirstPos(pieces);
        attacks |= s__rookAttacks(position, occupied, notPieces);
        pieces &= (pieces - 1);
    }

    return attacks;
}

inline Rawboard s__knightAttacks(const Board& board, const Position position, const Rawboard opposite) {
    return staticKnightAttacks[position] & (board.EMPTY | opposite);
}

inline Rawboard s_knightAttacks(const Board& board, const Position position, const Side side) {
	return s__knightAttacks(board, position, board.OPP_PIECES(side));
}

inline Rawboard s_allKnightAttacks(const Board& board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board.pieceBoards[WKnight + side];

    while (pieces) {
        const Position position = getFirstPos(pieces);
        attacks |= staticKnightAttacks[position];
        pieces &= (pieces - 1);
    }

    return attacks & (board.EMPTY | board.OPP_PIECES(side));
}

inline Rawboard s__bishopAttacks(const Position position, const Rawboard occupied, const Rawboard notSide) {
    return (s_noEastAttack(occupied, position) | s_soEastAttack(occupied, position) | s_soWestAttack(occupied, position) | s_noWestAttack(occupied, position)) & notSide;
}

inline Rawboard s_bishopAttacks(const Board& board, const Position position, const Side side) {
    return s__bishopAttacks(position, ~board.EMPTY, ~board.PIECES(side));
}

inline Rawboard s_allBishopAttacks(const Board& board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board.pieceBoards[WBishop + side];
    const Rawboard occupied = ~board.EMPTY;
    const Rawboard notPieces = ~board.PIECES(side);

    while (pieces) {
        const Position position = getFirstPos(pieces);
        attacks |= s__bishopAttacks(position, occupied, notPieces);
        pieces &= (pieces - 1);
    }

    return attacks;
}

inline Rawboard s__queenAttacks(const Position position, const Rawboard occupied, const Rawboard notPieces) {
    return (s_northAttack(occupied, position) | s_eastAttack(occupied, position) | s_southAttack(occupied, position) | s_westAttack(occupied, position) |
            s_noEastAttack(occupied, position) | s_soEastAttack(occupied, position) | s_soWestAttack(occupied, position) | s_noWestAttack(occupied, position)) & notPieces;
}

inline Rawboard s_queenAttacks(const Board& board, const Position position, const Side side) {
    return s__queenAttacks(position, ~board.EMPTY, ~board.PIECES(side));
}

inline Rawboard s_allQueenAttacks(const Board& board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board.pieceBoards[WQueen + side];
    const Rawboard occupied = ~board.EMPTY;
    const Rawboard notPieces = ~board.PIECES(side);

    while (pieces) {
        const Position position = getFirstPos(pieces);
        attacks |= s__queenAttacks(position, occupied, notPieces);
        pieces &= (pieces - 1);
    }

    return attacks;
}

inline Rawboard s_whiteKingCastling(const Board& board, const Position position) {
    Rawboard positions = 0L;

    if (position == 60) {
        if ((board.castlingInfo & 0b1000) && board.isEmpty(61)) {
            positions |= posInd(62) & board.EMPTY;
        }
        if ((board.castlingInfo & 0b0100) && board.isEmpty(59) && board.isEmpty(57)) {
            positions |= posInd(58) & board.EMPTY;
        }
    }

    return positions;
}

inline Rawboard s_blackKingCastling(const Board& board, const Position position) {
    Rawboard positions = 0L;

    if (position == 4) {
        if ((board.castlingInfo & 0b0010) && board.isEmpty(5)) {
            positions |= posInd(6) & board.EMPTY;
        }
        if ((board.castlingInfo & 0b0001) && board.isEmpty(3) && board.isEmpty(1)) {
            positions |= posInd(2) & board.EMPTY;
        }
    }

    return positions;
}

inline Rawboard s_allKingAttacks(const Board& board, const Side side) {
    const Position position = getFirstPos(board.pieceBoards[WKing + side]);
    return staticKingAttacks[position] & (board.EMPTY | board.OPP_PIECES(side));
}

inline Rawboard s_kingAttacks(const Board& board, const Position position, const Rawboard opposite) {
    return staticKingAttacks[position] & (board.EMPTY | opposite);
}

inline Rawboard s_whiteKingMoves(const Board& board, const Position position, const Side side) {
    return s_kingAttacks(board, position, board.OPP_PIECES(side)) | s_whiteKingCastling(board, position);
}

inline Rawboard s_blackKingMoves(const Board& board, const Position position, const Side side) {
    return s_kingAttacks(board, position, board.OPP_PIECES(side)) | s_blackKingCastling(board, position);
}

inline Rawboard s_kingMoves(const Board& board, const Position position, const Side side) {
    return side ? s_blackKingMoves(board, position, side) : s_whiteKingMoves(board, position, side);
}

inline Rawboard s_allKingMoves(const Board& board, const Side side) {
    const Position position = getFirstPos(board.pieceBoards[WKing + side]);
    return s_allKingAttacks(board, side) | (side ? s_blackKingCastling(board, position) : s_whiteKingCastling(board, position));
}

inline Rawboard s_allAttacks(const Board& board, const Side side) {
    return s_allPawnAttacks(board, side) | s_allKnightAttacks(board, side) |
        s_allBishopAttacks(board, side) | s_allRookAttacks(board, side) |
            s_allQueenAttacks(board, side) | s_allKingAttacks(board, side);
}

inline Rawboard(*destPosProviders[13])(const Board&, Position, Side);

inline void initDestPosProviders() {
    destPosProviders[WPawn] = s_whitePawnMoves;
    destPosProviders[BPawn] = s_blackPawnMoves;
    destPosProviders[WRook] = s_rookAttacks;
    destPosProviders[BRook] = s_rookAttacks;
    destPosProviders[WKnight] = s_knightAttacks;
    destPosProviders[BKnight] = s_knightAttacks;
    destPosProviders[WBishop] = s_bishopAttacks;
    destPosProviders[BBishop] = s_bishopAttacks;
    destPosProviders[WQueen] = s_queenAttacks;
    destPosProviders[BQueen] = s_queenAttacks;
    destPosProviders[WKing] = s_whiteKingMoves;
    destPosProviders[BKing] = s_blackKingMoves;
}

inline Rawboard s_getDestinationPositions(const Board& board, const Position position, const Piece piece, const Side side) {
    return (destPosProviders[piece])(board, position, side);
}

inline Rawboard s_getDestinationPositions(const Board& board, const Position position) {
    const Piece piece = board.piecePositions[position];
    return s_getDestinationPositions(board, position, piece, _getSide(piece));
}

inline unordered_set<Position>* getPiecePositions(const unordered_set<Piece>& pieces) {
    unordered_set<Position>* positions = new unordered_set<Position>();

    for (Position i = 0; i < 64; i++) {
        if (pieces.find(getPiece(i)) != pieces.end()) {
            positions->insert(i);
        }
    }

    return positions;
}

inline bool isOnXRay(const Board& board, const Position sourcePosition, const Position excludePosition) {
    unordered_set<Position>* positions = getPiecePositions(XRAY_PIECES[OPPOSITE(board.getSide(sourcePosition))]);
    positions->erase(excludePosition);
    Rawboard xRayPositions = 0;

    for (Position position : *positions) {
        xRayPositions |= s_getDestinationPositions(board, position);
    }

    delete positions;

    return isUnderCheck(xRayPositions, sourcePosition);
}
