#include "board.h"
#include "../utils/boardUtils.h"
#include "../utils/castlingHelper.h"

Board::Board() {
	pieceBoards = new Rawboard[SIZE];
    reset();
}

Board::~Board() {
    // TODO delete boards
}

void Board::reset() {
    for (RawboardIndex i = 1; i < SIZE; ++i) {
        pieceBoards[i] = 0;
    }

    pieceBoards[EMPTY_IND] = EMPTY_BOARD;
}

Rawboard Board::EMPTY() const {
    return pieceBoards[EMPTY_IND]; 
}

Rawboard Board::BOARD() const {
    return ~EMPTY();
}

Rawboard Board::WHITE() const {
    return pieceBoards[WPawn] |
        pieceBoards[WBishop] |
        pieceBoards[WQueen] |
        pieceBoards[WKnight] |
        pieceBoards[WKing] |
        pieceBoards[WRook];
}

Rawboard Board::BLACK() const {
    return pieceBoards[BPawn] |
        pieceBoards[BBishop] |
        pieceBoards[BQueen] |
        pieceBoards[BKnight] |
        pieceBoards[BKing] |
        pieceBoards[BRook];
}

Rawboard Board::getBoard(const RawboardIndex boardIndex) const {
    return pieceBoards[boardIndex];
}

void Board::setBoard(const RawboardIndex boardIndex, const Rawboard pieceBoard) {
    pieceBoards[boardIndex] = pieceBoard;
}

bool Board::isEmpty(const Position position) const {
    return (EMPTY() & Board::posInd(position)) != 0L;
}

bool Board::isWhite(const Position position) const {
    return ((pieceBoards[WPawn] |
        pieceBoards[WBishop] |
        pieceBoards[WQueen] |
        pieceBoards[WKnight] |
        pieceBoards[WKing] |
        pieceBoards[WRook])
        & posInd(position)) != 0L;
}

bool Board::isBlack(const Position position) const {
    return ((pieceBoards[BPawn] |
        pieceBoards[BBishop] |
        pieceBoards[BQueen] |
        pieceBoards[BKnight] |
        pieceBoards[BKing] |
        pieceBoards[BRook])
        & posInd(position)) != 0L;
}

bool Board::isPawn(const Position position) const {
    return ((pieceBoards[WPawn] | pieceBoards[BPawn]) & posInd(position)) != 0L;
}

bool Board::isKnight(const Position position) const {
    return ((pieceBoards[WKnight] | pieceBoards[BKnight]) & posInd(position)) != 0L;
}

bool Board::isBishop(const Position position) const {
    return ((pieceBoards[WBishop] | pieceBoards[BBishop]) & posInd(position)) != 0L;
}

bool Board::isRook(const Position position) const {
    return ((pieceBoards[WRook] | pieceBoards[BRook]) & posInd(position)) != 0L;
}

bool Board::isRook(const Position position, bool white) const {
    return white ? (pieceBoards[WRook] & posInd(position)) != 0 : (pieceBoards[BRook] & posInd(position)) != 0L;
}

bool Board::isQueen(const Position position) const {
    return ((pieceBoards[WQueen] | pieceBoards[BQueen]) & posInd(position)) != 0L;
}

bool Board::isKing(const Position position) const {
    return ((pieceBoards[WKing] | pieceBoards[BKing]) & posInd(position)) != 0L;
}

Piece Board::getPiece(const Position position) const {
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

Piece Board::setPiece(const Position position, const Piece piece) {
    const Piece oldPiece = getPiece(position);
    const Rawboard posIndex = posInd(position);
    pieceBoards[oldPiece] &= ~posIndex;
    pieceBoards[piece] |= posIndex;
    return oldPiece;
}

Piece Board::move(const Position source, const Position destination, Piece piece) {
    if (piece == Empty) {
        piece = getPiece(source);
    }
    const Piece oldPiece = setPiece(destination, piece);
    const Rawboard posSource = posInd(source);
    pieceBoards[piece] &= ~posSource;
    pieceBoards[EMPTY_IND] |= posSource;
    return oldPiece;
}

void Board::set(const Board& board) {
    for (RawboardIndex i = 0; i < SIZE; ++i) {
        pieceBoards[i] = board.getBoard(i);
    }
}

bool Board::isUnderCheck(const Position position, const bool white) const {
    return BoardUtils::isUnderCheck(white ? getBlackAttacks() : getWhiteAttacks(), position);
}

Rawboard Board::getWhiteAttacks() const {
    return getPawnAttacks(true) | getKnightAttacks(true) | getBishopAttacks(true) | getRookAttacks(true) | getQueenAttacks(true) | getKingAttacks(true);
}

Rawboard Board::getBlackAttacks() const {
    return getPawnAttacks(false) | getKnightAttacks(false) | getBishopAttacks(false) | getRookAttacks(false) | getQueenAttacks(false) | getKingAttacks(false);
}

Rawboard Board::getKingAttacks(const bool white) const {
    Rawboard board = white ? pieceBoards[WKing] : pieceBoards[BKing];
    Rawboard attacks = eastOne(board) | westOne(board);
    board |= attacks;
    attacks |= northOne(board) | southOne(board);
    const Rawboard oppositeBoard = white ? BLACK() : WHITE();
    return attacks & (EMPTY() | oppositeBoard);
}

Rawboard Board::getKingMoves(const bool white, const CastlingInfo castlingInfo) const {
    return getKingAttacks(white) | getKingCastling(white, castlingInfo);
}

Rawboard Board::getQueenAttacks(const bool white) const {
    Rawboard attacks = 0;
    Rawboard board = white ? pieceBoards[WQueen] : pieceBoards[BQueen];

    for (int i = 0; i < 64 && (board != 0); ++i) {
        if ((board & 1L) != 0) {
            attacks |= getQueenMoves(i, white);      // OLD
            //attacks |= queenAttacks(i, white);
        }
        board = board >> 1;
    }

    return attacks;
}

Rawboard Board::getRookAttacks(const bool white) const {
    Rawboard attacks = 0;
    Rawboard board = white ? pieceBoards[WRook] : pieceBoards[BRook];

    for (int i = 0; i < 64 && (board != 0); ++i) {
        if ((board & 1L) != 0) {
            attacks |= getRookMoves(i, white);       // OLD
            //attacks |= rookAttack(i, white);
        }
        board = board >> 1;
    }

    return attacks;
}

Rawboard Board::getBishopAttacks(const bool white) const {
    Rawboard attacks = 0;
    Rawboard board = white ? pieceBoards[WBishop] : pieceBoards[BBishop];

    for (int i = 0; i < 64 && (board != 0); ++i) {
        if ((board & 1L) != 0) {
            attacks |= getBishopMoves(i, white);     // OLD
            //attacks |= bishopAttack(i, white);
        }
        board = board >> 1;
    }

    return attacks;
}

Rawboard Board::getKnightAttacks(const bool white) const {
    Rawboard board = white ? pieceBoards[WKnight] : pieceBoards[BKnight];
    return (noNoEa(board) | noEaEa(board) |
        soEaEa(board) | soSoEa(board) |
        soSoWe(board) | soWeWe(board) |
        noWeWe(board) | noNoWe(board)) &
        (EMPTY() | (white ? BLACK() : WHITE()));
}

Rawboard Board::getPawnMoves(const bool white, const Position enPassantPos) const {
    Rawboard attacks = 0;
    Rawboard board = white ? pieceBoards[WPawn] : pieceBoards[BPawn];

    for (int i = 0; i < 64 && (board != 0); ++i) {
        if ((board & 1L) != 0) {
            attacks |= getPawnMoves(i, white, enPassantPos);
        }
        board = board >> 1;
    }

    return attacks;
}

Rawboard Board::getPawnAttacks(const bool white) const {
    Rawboard attacks = 0;
    Rawboard board = white ? pieceBoards[WPawn] : pieceBoards[BPawn];
    board = board >> 8;

    for (int i = 8; i < 56 && (board != 0); ++i) {
        if ((board & 1L) != 0) {
            attacks |= getPawnAttacks(i, white);
        }
        board = board >> 1;
    }

    return attacks;
}

Rawboard Board::getPawnMoves(const Position position, const bool white, const Position enPassantPos) const {
    const Rawboard posIndex = posInd(position);

    if (white) {
        const Rawboard onePush = northOne(posIndex) & EMPTY();
        return onePush | (northOne(onePush) & EMPTY() & ROW_4) |
            ((noWestOne(posIndex) | noEastOne(posIndex)) & BLACK()) |
            getPawnEnPassant(posIndex, true, enPassantPos);
    }
    else {
        const Rawboard onePush = southOne(posIndex) & EMPTY();
        return onePush | (southOne(onePush) & EMPTY() & ROW_5) |
            ((soWestOne(posIndex) | soEastOne(posIndex)) & WHITE()) |
            getPawnEnPassant(posIndex, false, enPassantPos);
    }
}

Rawboard Board::getPawnAttacks(const Position position, const bool white) const {
    const Rawboard posIndex = posInd(position);

    if (white) {
        return (noWestOne(posIndex) | noEastOne(posIndex)) & (EMPTY() | BLACK());
    }
    else {
        return (soWestOne(posIndex) | soEastOne(posIndex)) & (EMPTY() | WHITE());
    }
}

Rawboard Board::getPawnEnPassant(const Rawboard position, const bool white, const Position enPassantPos) const {
    if (enPassantPos != NO_POS) {
        const Rawboard passantPos = posInd(enPassantPos);
        if (white && (position & ROW_5) != 0) {
            if (passantPos == noEastOne(position) || passantPos == noWestOne(position)) {
                return passantPos;
            }
        }
        else if (!white && (position & ROW_4) != 0) {
            if (passantPos == soEastOne(position) || passantPos == soWestOne(position)) {
                return passantPos;
            }
        }
    }

    return 0;
}

Rawboard Board::getKnightMoves(const Position position, const bool white) const {
    const Rawboard posIndex = posInd(position);
    return (noNoEa(posIndex) | noEaEa(posIndex) |
        soEaEa(posIndex) | soSoEa(posIndex) |
        soSoWe(posIndex) | soWeWe(posIndex) |
        noWeWe(posIndex) | noNoWe(posIndex)) &
        (EMPTY() | (white ? BLACK() : WHITE()));
}

Rawboard Board::getBishopMoves(const Position position, const bool white) const {
    const Rawboard posIndex = posInd(position);
    const Rawboard oppositeBoard = white ? BLACK() : WHITE();

    return slidingAttack(noEastOne, posIndex, oppositeBoard) |
        slidingAttack(soEastOne, posIndex, oppositeBoard) |
        slidingAttack(soWestOne, posIndex, oppositeBoard) |
        slidingAttack(noWestOne, posIndex, oppositeBoard);
}

Rawboard Board::getRookMoves(const Position position, const bool white) const {
    const Rawboard posIndex = posInd(position);
    const Rawboard oppositeBoard = white ? BLACK() : WHITE();

    return slidingAttack(northOne, posIndex, oppositeBoard) |
        slidingAttack(eastOne, posIndex, oppositeBoard) |
        slidingAttack(southOne, posIndex, oppositeBoard) |
        slidingAttack(westOne, posIndex, oppositeBoard);
}

Rawboard Board::getQueenMoves(const Position position, const bool white) const {
    return getBishopMoves(position, white) | getRookMoves(position, white);
}

Rawboard Board::getKingMoves(const Position position, const bool white, const CastlingInfo castlingInfo) const {
    return getKingAttacks(position, white) | getKingCastling(position, white, castlingInfo);
}

Rawboard Board::getKingAttacks(const Position position, const bool white) const {
    const Rawboard posIndex = posInd(position);
    return (northOne(posIndex) | noEastOne(posIndex) | eastOne(posIndex) | soEastOne(posIndex) |
        southOne(posIndex) | soWestOne(posIndex) | westOne(posIndex) | noWestOne(posIndex))
        & (EMPTY() | (white ? BLACK() : WHITE()));
}

Rawboard Board::getKingCastling(const bool white, const CastlingInfo castlingInfo) const {
    const Rawboard board = white ? pieceBoards[WKing] : pieceBoards[BKing];
    const Position position = BoardUtils::getFirstPos(board);
    return getKingCastling(position, white, castlingInfo);
}

Rawboard Board::getKingCastling(const Position position, const bool white, const CastlingInfo castlingInfo) const {
    Rawboard positions = 0L;

    if (position == 4 && !white) {
        if (CastlingHelper::isBlackKingCastling(castlingInfo)) {
            const Rawboard posIndex = posInd(position);
            if ((eastOne(posIndex) & EMPTY()) != 0) {
                positions |= eastOne(eastOne(posIndex)) & EMPTY();
            }
        }
        if (CastlingHelper::isBlackQueenCastling(castlingInfo)) {
            const Rawboard posIndex = posInd(position);
            if ((westOne(posIndex) & EMPTY()) != 0) {
                positions |= westOne(westOne(posIndex)) & EMPTY();
            }
        }
    }
    else if (position == 60 && white) {
        if (CastlingHelper::isWhiteKingCastling(castlingInfo)) {
            const Rawboard posIndex = posInd(position);
            if ((eastOne(posIndex) & EMPTY()) != 0) {
                positions |= eastOne(eastOne(posIndex)) & EMPTY();
            }
        }
        if (CastlingHelper::isWhiteQueenCastling(castlingInfo)) {
            const Rawboard posIndex = posInd(position);
            if ((westOne(posIndex) & EMPTY()) != 0) {
                positions |= westOne(westOne(posIndex)) & EMPTY();
            }
        }
    }

    return positions;
}

Rawboard Board::slidingAttack(Rawboard(*direction)(Rawboard), const Rawboard position, const Rawboard oppositeBoard) const {
    Rawboard positions = 0;
    Rawboard newPos = position;
    Rawboard emptyPos;

    while (newPos != 0) {
        emptyPos = direction(newPos) & EMPTY();

        if (emptyPos == 0) {
            positions |= (direction(newPos) & oppositeBoard);
            break;
        }

        positions |= emptyPos;
        newPos = emptyPos;
    }

    return positions;
}

// sliding piece
Rawboard Board::rookAttack(const Position position, const bool white) const {
    const Rawboard occupied = BOARD();
    const Rawboard notWhite = white ? ~WHITE() : ~BLACK();
    return (northAttack(occupied, position) | eastAttack(occupied, position) | southAttack(occupied, position) | westAttack(occupied, position)) & notWhite;
}

Rawboard Board::bishopAttack(const Position position, const bool white) const {
    const Rawboard occupied = BOARD();
    const Rawboard notWhite = white ? ~WHITE() : ~BLACK();
    return (noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notWhite;
}

Rawboard Board::queenAttacks(const Position position, const bool white) const {
    return rookAttack(position, white) | bishopAttack(position, white);
}
