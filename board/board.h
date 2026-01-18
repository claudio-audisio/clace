#pragma once

#include "../common/structures.h"
#include "../common/types.h"
#include "../common/constants.h"
#include "../utils/pieceHelper.h"
#include "../common/bitwise.h"
#include "../utils/utils.h"
#include "../utils/positions.h"


inline Rawboard PIECES(const Board *board, const Side side) {
    return board->pieceBoards[WPawn + side] |
           board->pieceBoards[WBishop + side] |
           board->pieceBoards[WQueen + side] |
           board->pieceBoards[WKnight + side] |
           board->pieceBoards[WKing + side] |
           board->pieceBoards[WRook + side];
}

inline Rawboard OPP_PIECES(const Board *board, const Side side) {
    return board->pieceBoards[BPawn - side] |
           board->pieceBoards[BBishop - side] |
           board->pieceBoards[BQueen - side] |
           board->pieceBoards[BKnight - side] |
           board->pieceBoards[BKing - side] |
           board->pieceBoards[BRook - side];
}

inline void reset(Board *board) {
    for (RawboardIndex i = 1; i < 13; ++i) {
        board->pieceBoards[i] = 0;
    }

    board->pieceBoards[Empty] = EMPTY_BOARD;

    for (Piece& piece : board->piecePositions) {
        piece = Empty;
    }

    board->castlingInfo = 0;
    board->enPassantPosition = NO_POS;
}

inline bool equals(const Board *board1, const Board *board2) {
    for (RawboardIndex i = 1; i < 13; ++i) {
        if (board1->pieceBoards[i] != board2->pieceBoards[i]) {
            return false;
        }
    }

    for (Position i = 0; i < 64; i++) {
        if (board1->piecePositions[i] != board2->piecePositions[i]) {
            return false;
        }
    }

    return true;
}

inline void copy(const Board* source, Board* destination) {
    for (RawboardIndex i = 0; i < 13; ++i) {
        destination->pieceBoards[i] = source->pieceBoards[i];
    }

    for (int i = 0; i < 64; i++) {
        destination->piecePositions[i] = source->piecePositions[i];
    }

    destination->castlingInfo = source->castlingInfo;
    destination->enPassantPosition = source->enPassantPosition;
}

inline unsigned int getPieceCount(const Board *board, const Piece piece) {
    return popcount(board->pieceBoards[piece]);
}

inline Side getSide(const Board *board, const Position position) {
    return _getSide(board->piecePositions[position]);
}

inline bool isEmpty(const Board *board, const Position position) {
    return board->pieceBoards[Empty] & posInd(position);
}

inline bool isWhite(const Board *board, const Position position) {
    return _isWhite(board->piecePositions[position]);
}

inline bool isBlack(const Board *board, const Position position) {
    return _isBlack(board->piecePositions[position]);
}

inline bool isPawn(const Board *board, const Position position) {
    return (board->pieceBoards[WPawn] | board->pieceBoards[BPawn]) & posInd(position);
}

inline bool isKnight(const Board *board, const Position position) {
    return (board->pieceBoards[WKnight] | board->pieceBoards[BKnight]) & posInd(position);
}

inline bool isBishop(const Board *board, const Position position) {
    return (board->pieceBoards[WBishop] | board->pieceBoards[BBishop]) & posInd(position);
}

inline bool isRook(const Board *board, const Position position) {
    return (board->pieceBoards[WRook] | board->pieceBoards[BRook]) & posInd(position);
}

inline bool isRook(const Board *board, const Position position, const Side side) {
    return board->pieceBoards[WRook + side] & posInd(position);
}

inline bool isQueen(const Board *board, const Position position) {
    return (board->pieceBoards[WQueen] | board->pieceBoards[BQueen]) & posInd(position);
}

inline bool isKing(const Board *board, const Position position) {
    return (board->pieceBoards[WKing] | board->pieceBoards[BKing]) & posInd(position);
}

inline Position getKingPosition(const Board *board, const Side side) {
    return getFirstPos(board->pieceBoards[WKing + side]);
}

inline Position getWhiteKingPosition(const Board *board) {
    return getFirstPos(board->pieceBoards[WKing]);
}

inline Position getBlackKingPosition(const Board *board) {
    return getFirstPos(board->pieceBoards[BKing]);
}

inline Piece getPiece(const Board *board, const Position position) {
    return board->piecePositions[position];
}

inline Piece setPiece(Board *board, const Position position, const Piece piece) {
    const Piece oldPiece = getPiece(board, position);
    const Rawboard posIndex = posInd(position);
    board->pieceBoards[oldPiece] &= ~posIndex;
    board->pieceBoards[piece] |= posIndex;
    board->piecePositions[position] = piece;
    return oldPiece;
}

inline void setEmpty(Board *board, const Position position, const Piece oldPiece) {
    const Rawboard posIndex = posInd(position);
    board->pieceBoards[oldPiece] &= ~posIndex;
    board->pieceBoards[Empty] |= posIndex;
    board->piecePositions[position] = Empty;
}

inline Piece setEmpty(Board *board, const Position position) {
    const Piece oldPiece = getPiece(board, position);
    setEmpty(board, position, oldPiece);
    return oldPiece;
}

inline void updateCastlingInfo(Board *board, const Position source, const Position destination) {
    board->castlingInfo &= CASTLING_INFO_MASK[source];
    board->castlingInfo &= CASTLING_INFO_MASK[destination];
}

inline void updateEnPassantInfo(Board *board, const Position source, const Position destination, const Piece piece) {
    const Side side = _getSide(piece);

    if (_isPawn(piece) && isSecondRow(source, side) && isFourthRow(destination, side)) {
        board->enPassantPosition = source + 8 - (16 * side);
    }
    else {
        board->enPassantPosition = NO_POS;
    }
}

inline void castlingMove(Board *board, const Position source, const Position destination) {
    switch (source + destination) {
    case BQCastling: {
            board->pieceBoards[BKing] = BQC_King;
            board->pieceBoards[BRook] &= BQC_RookRem;
            board->pieceBoards[BRook] |= BQC_RookAdd;
            board->pieceBoards[Empty] &= BQC_EmptyRem;
            board->pieceBoards[Empty] |= BQC_EmptyAdd;
            board->piecePositions[56] = Empty;
            board->piecePositions[58] = BKing;
            board->piecePositions[59] = BRook;
            board->piecePositions[60] = Empty;
            break;
    }
    case BKCastling: {
            board->pieceBoards[BKing] = BKC_King;
            board->pieceBoards[BRook] &= BKC_RookRem;
            board->pieceBoards[BRook] |= BKC_RookAdd;
            board->pieceBoards[Empty] &= BKC_EmptyRem;
            board->pieceBoards[Empty] |= BKC_EmptyAdd;
            board->piecePositions[60] = Empty;
            board->piecePositions[62] = BKing;
            board->piecePositions[61] = BRook;
            board->piecePositions[63] = Empty;
            break;
    }
    case WQCastling: {
            board->pieceBoards[WKing] = WQC_King;
            board->pieceBoards[WRook] &= WQC_RookRem;
            board->pieceBoards[WRook] |= WQC_RookAdd;
            board->pieceBoards[Empty] &= WQC_EmptyRem;
            board->pieceBoards[Empty] |= WQC_EmptyAdd;
            board->piecePositions[0] = Empty;
            board->piecePositions[2] = WKing;
            board->piecePositions[3] = WRook;
            board->piecePositions[4] = Empty;
            break;
    }
    case WKCastling: {
            board->pieceBoards[WKing] = WKC_King;
            board->pieceBoards[WRook] &= WKC_RookRem;
            board->pieceBoards[WRook] |= WKC_RookAdd;
            board->pieceBoards[Empty] &= WKC_EmptyRem;
            board->pieceBoards[Empty] |= WKC_EmptyAdd;
            board->piecePositions[4] = Empty;
            board->piecePositions[6] = WKing;
            board->piecePositions[5] = WRook;
            board->piecePositions[7] = Empty;
            break;
    }
    default: throw runtime_error("wrong castling move");
    }
}

inline Piece simulateMovePiece(Board *board, const Position source, const Position destination, const Piece piece, const bool isCastling) {
    if (isCastling) {
        castlingMove(board, source, destination);
        return Empty;
    }

    const Piece oldPiece = setPiece(board, destination, piece);
    setEmpty(board, source, piece);

    return oldPiece;
}

inline Piece movePiece(Board *board, const Position source, const Position destination, Piece piece, const MoveType type = NORMAL, const Piece promotionPiece = Empty) {
    switch (type) {
    case NORMAL: {
            const Piece oldPiece = setPiece(board, destination, piece);
            setEmpty(board, source, piece);
            updateEnPassantInfo(board, source, destination, piece);
            updateCastlingInfo(board, source, destination);
            return oldPiece;
    }
    case CASTLING: {
            board->enPassantPosition = NO_POS;
            castlingMove(board, source, destination);
            updateCastlingInfo(board, source, destination);
            return Empty;
    }
    case EN_PASSANT: {
            board->enPassantPosition = NO_POS;
            setPiece(board, destination, piece);
            setEmpty(board, source, piece);
            return setEmpty(board, destination - 24 + piece * 16);
    }
    case PROMOTION: {
            if (promotionPiece == Empty) { throw runtime_error("promotion piece not set"); }
            board->enPassantPosition = NO_POS;
            const Piece oldPiece = setPiece(board, destination, piece);
            setPiece(board, destination, promotionPiece);
            setEmpty(board, source, piece);
            updateCastlingInfo(board, source, destination);
            return oldPiece;
    }
    default: throw runtime_error("unexpected move type");
    }
}

inline void undoCastlingMove(Board *board, const Position source, const Position destination) {
    switch (source + destination) {
    case BQCastling: {
            board->pieceBoards[BKing] = BC_King;
            board->pieceBoards[BRook] |= ~BQC_RookRem;
            board->pieceBoards[BRook] &= ~BQC_RookAdd;
            board->pieceBoards[Empty] |= ~BQC_EmptyRem;
            board->pieceBoards[Empty] &= ~BQC_EmptyAdd;
            board->piecePositions[58] = Empty;
            board->piecePositions[60] = BKing;
            board->piecePositions[56] = BRook;
            board->piecePositions[59] = Empty;
            break;
    }
    case BKCastling: {
            board->pieceBoards[BKing] = BC_King;
            board->pieceBoards[BRook] |= ~BKC_RookRem;
            board->pieceBoards[BRook] &= ~BKC_RookAdd;
            board->pieceBoards[Empty] |= ~BKC_EmptyRem;
            board->pieceBoards[Empty] &= ~BKC_EmptyAdd;
            board->piecePositions[61] = Empty;
            board->piecePositions[63] = BRook;
            board->piecePositions[60] = BKing;
            board->piecePositions[62] = Empty;
            break;
    }
    case WQCastling: {
            board->pieceBoards[WKing] = WC_King;
            board->pieceBoards[WRook] |= ~WQC_RookRem;
            board->pieceBoards[WRook] &= ~WQC_RookAdd;
            board->pieceBoards[Empty] |= ~WQC_EmptyRem;
            board->pieceBoards[Empty] &= ~WQC_EmptyAdd;
            board->piecePositions[2] = Empty;
            board->piecePositions[4] = WKing;
            board->piecePositions[0] = WRook;
            board->piecePositions[3] = Empty;
            break;
    }
    case WKCastling: {
            board->pieceBoards[WKing] = WC_King;
            board->pieceBoards[WRook] |= ~WKC_RookRem;
            board->pieceBoards[WRook] &= ~WKC_RookAdd;
            board->pieceBoards[Empty] |= ~WKC_EmptyRem;
            board->pieceBoards[Empty] &= ~WKC_EmptyAdd;
            board->piecePositions[5] = Empty;
            board->piecePositions[7] = WRook;
            board->piecePositions[4] = WKing;
            board->piecePositions[6] = Empty;
            break;
    }
    default: throw runtime_error("wrong undo castling move");
    }
}
