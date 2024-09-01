#include "board.h"

Board::Board() {
	pieceBoards = new Rawboard[SIZE];
    reset();
}

Board::~Board() {
    delete pieceBoards;
}

void Board::reset() {
    for (RawboardIndex i = 1; i < SIZE; ++i) {
        pieceBoards[i] = 0;
    }

    pieceBoards[EMPTY_IND] = EMPTY_BOARD;
}

Rawboard Board::EMPTY() {
    return pieceBoards[EMPTY_IND]; 
}

Rawboard Board::BOARD() {
    return ~EMPTY();
}

Rawboard Board::getBoard(RawboardIndex boardIndex) {
    return pieceBoards[boardIndex];
}

void Board::setBoard(RawboardIndex boardIndex, Rawboard pieceBoard) {
    pieceBoards[boardIndex] = pieceBoard;
}

bool Board::isEmpty(Position position) {
    return (EMPTY() & Board::posInd(position)) != 0L;
}

bool Board::isWhite(Position position) {
    return ((pieceBoards[WPawn] |
        pieceBoards[WBishop] |
        pieceBoards[WQueen] |
        pieceBoards[WKnight] |
        pieceBoards[WKing] |
        pieceBoards[WRook])
        & posInd(position)) != 0L;
}

bool Board::isBlack(Position position) {
    return ((pieceBoards[BPawn] |
        pieceBoards[BBishop] |
        pieceBoards[BQueen] |
        pieceBoards[BKnight] |
        pieceBoards[BKing] |
        pieceBoards[BRook])
        & posInd(position)) != 0L;
}

bool Board::isPawn(Position position) {
    return ((pieceBoards[WPawn] | pieceBoards[BPawn]) & posInd(position)) != 0L;
}

bool Board::isKnight(Position position) {
    return ((pieceBoards[WKnight] | pieceBoards[BKnight]) & posInd(position)) != 0L;
}

bool Board::isBishop(Position position) {
    return ((pieceBoards[WBishop] | pieceBoards[BBishop]) & posInd(position)) != 0L;
}

bool Board::isRook(Position position) {
    return ((pieceBoards[WRook] | pieceBoards[BRook]) & posInd(position)) != 0L;
}

bool Board::isRook(Position position, bool white) {
    return white ? (pieceBoards[WRook] & posInd(position)) != 0 : (pieceBoards[BRook] & posInd(position)) != 0L;
}

bool Board::isQueen(Position position) {
    return ((pieceBoards[WQueen] | pieceBoards[BQueen]) & posInd(position)) != 0L;
}

bool Board::isKing(Position position) {
    return ((pieceBoards[WKing] | pieceBoards[BKing]) & posInd(position)) != 0L;
}

Piece Board::getPiece(Position position) {
    const Rawboard posIndex = posInd(position);

    // TODO PERF Ricerco su tutte le boards se in quella posizione c'e' qualcosa: e se avessi una mappa posizione -> board 
    for (Piece piece = 0; piece < SIZE; ++piece) {
        if ((pieceBoards[piece] & posIndex) != 0L) {
            return piece;
        }
    }

    // TODO sollevare eccezione
    //throw new RuntimeException("chessboard internal error");
}

Piece Board::setPiece(Position position, Piece piece) {
    const Piece oldPiece = getPiece(position);
    const Rawboard posIndex = posInd(position);
    pieceBoards[oldPiece] &= ~posIndex;
    pieceBoards[piece] |= posIndex;
    return oldPiece;
}

Piece Board::move(Position source, Position destination, Piece piece) {
    if (piece == Empty) {
        piece = getPiece(source);
    }
    const Piece oldPiece = setPiece(destination, piece);
    const Rawboard posSource = posInd(source);
    pieceBoards[piece] &= ~posSource;
    pieceBoards[EMPTY_IND] |= posSource;
    return oldPiece;
}

void Board::set(Board& board) {
    for (RawboardIndex i = 0; i < SIZE; ++i) {
        pieceBoards[i] = board.getBoard(i);
    }
}
