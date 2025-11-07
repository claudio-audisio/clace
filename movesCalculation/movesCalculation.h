#pragma once

#include "../board/board.h"
#include "../common/types.h"
#include "../common/constants.h"
#include "../common/bitwise.h"
#include "rayAttacks.h"
#include "../utils/boardUtils.h"


inline Rawboard whiteEnPassantMove(const Board *board, const Rawboard position) {
    if (board->enPassantPosition != NO_POS && (position & ROW_5) != 0) {
        const Rawboard passantPos = posInd(board->enPassantPosition);

        if (passantPos == noEastOne(position) || passantPos == noWestOne(position)) {
            return passantPos;
        }
    }

    return 0;
}

inline Rawboard blackEnPassantMove(const Board *board, const Rawboard position) {
    if (board->enPassantPosition != NO_POS && (position & ROW_4) != 0) {
        const Rawboard passantPos = posInd(board->enPassantPosition);

        if (passantPos == soEastOne(position) || passantPos == soWestOne(position)) {
            return passantPos;
        }
    }

    return 0;
}

inline Rawboard whitePawnMoves(const Board *board, const Position position, const Side side) {
    const Rawboard posIndex = posInd(position);
    const Rawboard onePush = northOne(posIndex) & board->pieceBoards[Empty];

    return onePush | (northOne(onePush) & board->pieceBoards[Empty] & ROW_4) |
           ((noWestOne(posIndex) | noEastOne(posIndex)) & OPP_PIECES(board, _WHITE)) |
                whiteEnPassantMove(board, posIndex);
}

inline Rawboard blackPawnMoves(const Board *board, const Position position, const Side side) {
    const Rawboard posIndex = posInd(position);
    const Rawboard onePush = southOne(posIndex) & board->pieceBoards[Empty];

    return onePush | (southOne(onePush) & board->pieceBoards[Empty] & ROW_5) |
           ((soWestOne(posIndex) | soEastOne(posIndex)) & OPP_PIECES(board, _BLACK)) |
               blackEnPassantMove(board, posIndex);
}

inline Rawboard allPawnMoves(const Board *board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board->pieceBoards[WPawn + side];

    if (side) {
        while (pieces) {
            const Position position = getFirstPos(pieces);
            attacks |= blackPawnMoves(board, position, side);
            pieces &= (pieces - 1);
        }
    } else {
        while (pieces) {
            const Position position = getFirstPos(pieces);
            attacks |= whitePawnMoves(board, position, side);
            pieces &= (pieces - 1);
        }
    }

    return attacks;
}

inline Rawboard whitePawnAttacks(const Rawboard pawns) {
    return noWestOne(pawns) | noEastOne(pawns);
}

inline Rawboard blackPawnAttacks(const Rawboard pawns) {
    return soWestOne(pawns) | soEastOne(pawns);
}

inline Rawboard(*pawnAttacksProviders[2])(Rawboard);

inline void initPawnAttacksProviders() {
    pawnAttacksProviders[_WHITE] = whitePawnAttacks;
    pawnAttacksProviders[_BLACK] = blackPawnAttacks;
}

inline Rawboard allPawnAttacks(const Board *board, const Side side) {
    return pawnAttacksProviders[side](board->pieceBoards[WPawn + side]) & (board->pieceBoards[Empty] | OPP_PIECES(board, side));
}

inline Rawboard pawnAttacks(const Board *board, const Position position, const Side side) {
    return pawnAttacksProviders[side](posInd(position)) & (board->pieceBoards[Empty] | OPP_PIECES(board, side));
}

inline Rawboard _rookAttacks(const Position position, const Rawboard occupied, const Rawboard notPieces) {
	return (northAttack(occupied, position) | eastAttack(occupied, position) | southAttack(occupied, position) | westAttack(occupied, position)) & notPieces;
}

inline Rawboard rookAttacks(const Board *board, const Position position, const Side side) {
    return _rookAttacks(position, ~board->pieceBoards[Empty], ~PIECES(board, side));
}

inline Rawboard allRookAttacks(const Board *board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board->pieceBoards[WRook + side];
    const Rawboard occupied = ~board->pieceBoards[Empty];
    const Rawboard notPieces = ~PIECES(board, side);

    while (pieces) {
        const Position position = getFirstPos(pieces);
        attacks |= _rookAttacks(position, occupied, notPieces);
        pieces &= (pieces - 1);
    }

    return attacks;
}

inline Rawboard _knightAttacks(const Board *board, const Position position, const Rawboard opposite) {
    return staticKnightAttacks[position] & (board->pieceBoards[Empty] | opposite);
}

inline Rawboard knightAttacks(const Board *board, const Position position, const Side side) {
	return _knightAttacks(board, position, OPP_PIECES(board, side));
}

inline Rawboard allKnightAttacks(const Board *board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board->pieceBoards[WKnight + side];

    while (pieces) {
        const Position position = getFirstPos(pieces);
        attacks |= staticKnightAttacks[position];
        pieces &= (pieces - 1);
    }

    return attacks & (board->pieceBoards[Empty] | OPP_PIECES(board, side));
}

inline Rawboard _bishopAttacks(const Position position, const Rawboard occupied, const Rawboard notSide) {
    return (noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notSide;
}

inline Rawboard bishopAttacks(const Board *board, const Position position, const Side side) {
    return _bishopAttacks(position, ~board->pieceBoards[Empty], ~PIECES(board, side));
}

inline Rawboard allBishopAttacks(const Board *board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board->pieceBoards[WBishop + side];
    const Rawboard occupied = ~board->pieceBoards[Empty];
    const Rawboard notPieces = ~PIECES(board, side);

    while (pieces) {
        const Position position = getFirstPos(pieces);
        attacks |= _bishopAttacks(position, occupied, notPieces);
        pieces &= (pieces - 1);
    }

    return attacks;
}

inline Rawboard _queenAttacks(const Position position, const Rawboard occupied, const Rawboard notPieces) {
    return (northAttack(occupied, position) | eastAttack(occupied, position) | southAttack(occupied, position) | westAttack(occupied, position) |
            noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notPieces;
}

inline Rawboard queenAttacks(const Board *board, const Position position, const Side side) {
    return _queenAttacks(position, ~board->pieceBoards[Empty], ~PIECES(board, side));
}

inline Rawboard allQueenAttacks(const Board *board, const Side side) {
    Rawboard attacks = 0;
    Rawboard pieces = board->pieceBoards[WQueen + side];
    const Rawboard occupied = ~board->pieceBoards[Empty];
    const Rawboard notPieces = ~PIECES(board, side);

    while (pieces) {
        const Position position = getFirstPos(pieces);
        attacks |= _queenAttacks(position, occupied, notPieces);
        pieces &= (pieces - 1);
    }

    return attacks;
}

inline Rawboard whiteKingCastling(const Board *board, const Position position) {
    Rawboard positions = 0L;

    if (position == 60) {
        if ((board->castlingInfo & 0b1000) && isEmpty(board, 61)) {
            positions |= posInd(62) & board->pieceBoards[Empty];
        }
        if ((board->castlingInfo & 0b0100) && isEmpty(board, 59) && isEmpty(board, 57)) {
            positions |= posInd(58) & board->pieceBoards[Empty];
        }
    }

    return positions;
}

inline Rawboard blackKingCastling(const Board *board, const Position position) {
    Rawboard positions = 0L;

    if (position == 4) {
        if ((board->castlingInfo & 0b0010) && isEmpty(board, 5)) {
            positions |= posInd(6) & board->pieceBoards[Empty];
        }
        if ((board->castlingInfo & 0b0001) && isEmpty(board, 3) && isEmpty(board, 1)) {
            positions |= posInd(2) & board->pieceBoards[Empty];
        }
    }

    return positions;
}

inline Rawboard allKingAttacks(const Board *board, const Side side) {
    const Position position = getFirstPos(board->pieceBoards[WKing + side]);
    return staticKingAttacks[position] & (board->pieceBoards[Empty] | OPP_PIECES(board, side));
}

inline Rawboard kingAttacks(const Board *board, const Position position, const Rawboard opposite) {
    return staticKingAttacks[position] & (board->pieceBoards[Empty] | opposite);
}

inline Rawboard whiteKingMoves(const Board *board, const Position position, const Side side) {
    return kingAttacks(board, position, OPP_PIECES(board, side)) | whiteKingCastling(board, position);
}

inline Rawboard blackKingMoves(const Board *board, const Position position, const Side side) {
    return kingAttacks(board, position, OPP_PIECES(board, side)) | blackKingCastling(board, position);
}

inline Rawboard kingMoves(const Board *board, const Position position, const Side side) {
    return side ? blackKingMoves(board, position, side) : whiteKingMoves(board, position, side);
}

inline Rawboard allKingMoves(const Board *board, const Side side) {
    const Position position = getFirstPos(board->pieceBoards[WKing + side]);
    return allKingAttacks(board, side) | (side ? blackKingCastling(board, position) : whiteKingCastling(board, position));
}

inline Rawboard allAttacks(const Board *board, const Side side) {
    return allPawnAttacks(board, side) | allKnightAttacks(board, side) |
        allBishopAttacks(board, side) | allRookAttacks(board, side) |
            allQueenAttacks(board, side) | allKingAttacks(board, side);
}

inline Rawboard(*destPosProviders[13])(const Board*, Position, Side);

inline void initDestPosProviders() {
    destPosProviders[WPawn] = whitePawnMoves;
    destPosProviders[BPawn] = blackPawnMoves;
    destPosProviders[WRook] = rookAttacks;
    destPosProviders[BRook] = rookAttacks;
    destPosProviders[WKnight] = knightAttacks;
    destPosProviders[BKnight] = knightAttacks;
    destPosProviders[WBishop] = bishopAttacks;
    destPosProviders[BBishop] = bishopAttacks;
    destPosProviders[WQueen] = queenAttacks;
    destPosProviders[BQueen] = queenAttacks;
    destPosProviders[WKing] = whiteKingMoves;
    destPosProviders[BKing] = blackKingMoves;
}

inline Rawboard getDestinationPositions(const Board *board, const Position position, const Piece piece, const Side side) {
    return (destPosProviders[piece])(board, position, side);
}

inline Rawboard getDestinationPositions(const Board *board, const Position position) {
    const Piece piece = board->piecePositions[position];
    return getDestinationPositions(board, position, piece, _getSide(piece));
}

inline unordered_set<Position>* getPiecePositions(const Board *board, const unordered_set<Piece>& pieces) {
    unordered_set<Position>* positions = new unordered_set<Position>();

    for (Position i = 0; i < 64; i++) {
        if (pieces.contains(getPiece(board, i))) {
            positions->insert(i);
        }
    }

    return positions;
}

inline bool isOnXRay(const Board *board, const Position sourcePosition, const Position excludePosition) {
    unordered_set<Position>* positions = getPiecePositions(board, XRAY_PIECES[OPPOSITE(getSide(board, sourcePosition))]);
    positions->erase(excludePosition);
    Rawboard xRayPositions = 0;

    for (Position position : *positions) {
        xRayPositions |= getDestinationPositions(board, position);
    }

    delete positions;

    return isUnderCheck(xRayPositions, sourcePosition);
}
