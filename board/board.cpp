#include "board.h"
#include "../utils/boardUtils.h"
#include "../utils/castlingHelper.h"

Board::Board() {
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

    for (Piece& piece : piecePositions) {
        piece = Empty;
    }

	/*sideBoards[WHITE] = 0;
	sideBoards[BLACK] = 0;*/
}

Rawboard Board::BOARD(const Side side) const {
	return pieceBoards[WPawn + side] |
		   pieceBoards[WBishop + side] |
		   pieceBoards[WQueen + side] |
		   pieceBoards[WKnight + side] |
		   pieceBoards[WKing + side] |
		   pieceBoards[WRook + side];
}

Rawboard Board::OPPOSITE(const Side side) const {
	return pieceBoards[BPawn - side] |
		   pieceBoards[BBishop - side] |
		   pieceBoards[BQueen - side] |
		   pieceBoards[BKnight - side] |
		   pieceBoards[BKing - side] |
		   pieceBoards[BRook - side];
}

// TODO con l'uso delle side board si incremneta di un 5-10% (prob anche di piu' se fatto con piu' cura)
// Questo indica che l'aggiornamento incrementale delle boards e' la scelta prob da percorrere
/*Rawboard Board::OPPOSITE(const Side side) const {
	return sideBoards[BoardUtils::opposite(side)];
}*/

/*Rawboard Board::WHITE() const {
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
}*/

void Board::setBoard(const Piece boardIndex, const Rawboard pieceBoard) {
    // TODO gestire piecePositions
    pieceBoards[boardIndex] = pieceBoard;
}

void Board::update() {
	sideBoards[WHITE] = BOARD(WHITE);
	sideBoards[BLACK] = BOARD(BLACK);
}

bool Board::isEmpty(const Position position) const {
    return (EMPTY & posInd(position)) != 0L;
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

bool Board::isRook(const Position position, const Side side) const {
    return (pieceBoards[WRook + side] & posInd(position)) != 0;
}

bool Board::isQueen(const Position position) const {
    return ((pieceBoards[WQueen] | pieceBoards[BQueen]) & posInd(position)) != 0L;
}

bool Board::isKing(const Position position) const {
    return ((pieceBoards[WKing] | pieceBoards[BKing]) & posInd(position)) != 0L;
}

Position Board::getWhiteKingPosition() const {
	return Utils::getFirstPos(pieceBoards[WKing]);
}

Position Board::getBlackKingPosition() const {
	return Utils::getFirstPos(pieceBoards[BKing]);
}

Piece Board::getPiece(const Position position) const {
    return piecePositions[position];
}

Piece Board::setPiece(const Position position, const Piece piece) {
    const Piece oldPiece = getPiece(position);
    const Rawboard posIndex = posInd(position);
    pieceBoards[oldPiece] &= ~posIndex;
    pieceBoards[piece] |= posIndex;
    piecePositions[position] = piece;
    return oldPiece;
}

Piece Board::move(const Position source, const Position destination, Piece piece) {
    const Piece oldPiece = setPiece(destination, piece);
    const Rawboard posSource = posInd(source);
    pieceBoards[piece] &= ~posSource;
    pieceBoards[EMPTY_IND] |= posSource;
    piecePositions[source] = Empty;
    return oldPiece;
}

void Board::set(const Board& board) {
    for (RawboardIndex i = 0; i < SIZE; ++i) {
        pieceBoards[i] = board.pieceBoards[i];
    }

    for (int i = 0; i < 64; i++) {
        piecePositions[i] = board.piecePositions[i];
    }
}

bool Board::isUnderCheck(const Position position, const Side side) const {
    return BoardUtils::isUnderCheck(getAttacks(BLACK - side), position);
}

Rawboard Board::getAttacks(const Side side) const {
	return getPawnAttacks(side) | getKnightAttacks(side) | getBishopAttacks(side) | getRookAttacks(side) | getQueenAttacks(side) | getKingAttacks(side);
}

/*Rawboard Board::getWhiteAttacks() const {
    return getPawnAttacks(true) | getKnightAttacks(true) | getBishopAttacks(true) | getRookAttacks(true) | getQueenAttacks(true) | getKingAttacks(true);
}

Rawboard Board::getBlackAttacks() const {
    return getPawnAttacks(false) | getKnightAttacks(false) | getBishopAttacks(false) | getRookAttacks(false) | getQueenAttacks(false) | getKingAttacks(false);
}*/

Rawboard Board::getKingAttacks(const Side side) const {
    Rawboard board = pieceBoards[WKing + side];
    Rawboard attacks = eastOne(board) | westOne(board);
    board |= attacks;
    attacks |= northOne(board) | southOne(board);
    const Rawboard oppositeBoard = OPPOSITE(side);
    return attacks & (EMPTY | oppositeBoard);
}

Rawboard Board::getKingMoves(const Side side, const CastlingInfo castlingInfo) const {
    return getKingAttacks(side) | getKingCastling(side, castlingInfo);
}

Rawboard Board::getQueenAttacks(const Side side) const {
    Rawboard attacks = 0;
    Rawboard board = pieceBoards[WQueen + side];

    while (board) {
        const Position position = Utils::getFirstPos(board);
        attacks |= getQueenMoves(position, side);      // OLD
        //attacks |= queenAttacks(position, side);
        board &= (board - 1);
    }

    return attacks;
}

Rawboard Board::getRookAttacks(const Side side) const {
    Rawboard attacks = 0;
    Rawboard board = pieceBoards[WRook + side];

    while (board) {
        const Position position = Utils::getFirstPos(board);
        attacks |= getRookMoves(position, side);       // OLD
        //attacks |= rookAttack(position, side);
        board &= (board - 1);
    }

    return attacks;
}

Rawboard Board::getBishopAttacks(const Side side) const {
    Rawboard attacks = 0;
    Rawboard board = pieceBoards[WBishop + side];

    while (board) {
        const Position position = Utils::getFirstPos(board);
        attacks |= getBishopMoves(position, side);       // OLD
        //attacks |= bishopAttack(position, side);
        board &= (board - 1);
    }

    return attacks;
}

Rawboard Board::getKnightAttacks(const Side side) const {
    Rawboard board = pieceBoards[WKnight + side];
    return (noNoEa(board) | noEaEa(board) |
        soEaEa(board) | soSoEa(board) |
        soSoWe(board) | soWeWe(board) |
        noWeWe(board) | noNoWe(board)) &
        (EMPTY | OPPOSITE(side));
}

Rawboard Board::getPawnMoves(const Side side, const Position enPassantPos) const {
    Rawboard attacks = 0;
    Rawboard board = pieceBoards[WPawn + side];

    while (board) {
        const Position position = Utils::getFirstPos(board);
        attacks |= getPawnMoves(position, side, enPassantPos);
        board &= (board - 1);
    }

    return attacks;
}

Rawboard Board::getPawnAttacks(const Side side) const {
    Rawboard attacks = 0;
    Rawboard board = pieceBoards[WPawn + side];
    //board = board >> 8;

    while (board) {
        const Position position = Utils::getFirstPos(board);
        attacks |= getPawnAttacks(position, side);       // OLD
        board &= (board - 1);
    }

    return attacks;
}

Rawboard Board::getPawnMoves(const Position position, const Side side, const Position enPassantPos) const {
    const Rawboard posIndex = posInd(position);

    if (!side) {
        const Rawboard onePush = northOne(posIndex) & EMPTY;
        return onePush | (northOne(onePush) & EMPTY & ROW_4) |
            ((noWestOne(posIndex) | noEastOne(posIndex)) & OPPOSITE(side)) |
            getPawnEnPassant(posIndex, side, enPassantPos);
    }
    else {
        const Rawboard onePush = southOne(posIndex) & EMPTY;
        return onePush | (southOne(onePush) & EMPTY & ROW_5) |
            ((soWestOne(posIndex) | soEastOne(posIndex)) & OPPOSITE(side)) |
            getPawnEnPassant(posIndex, side, enPassantPos);
    }
}

Rawboard Board::getPawnAttacks(const Position position, const Side side) const {
    const Rawboard posIndex = posInd(position);

    if (!side) {
        return (noWestOne(posIndex) | noEastOne(posIndex)) & (EMPTY | OPPOSITE(side));
    }
    else {
        return (soWestOne(posIndex) | soEastOne(posIndex)) & (EMPTY | OPPOSITE(side));
    }
}

Rawboard Board::getPawnEnPassant(const Rawboard position, const Side side, const Position enPassantPos) {
    if (enPassantPos != NO_POS) {
        const Rawboard passantPos = posInd(enPassantPos);
        if (!side && (position & ROW_5) != 0) {
            if (passantPos == noEastOne(position) || passantPos == noWestOne(position)) {
                return passantPos;
            }
        }
        else if (side && (position & ROW_4) != 0) {
            if (passantPos == soEastOne(position) || passantPos == soWestOne(position)) {
                return passantPos;
            }
        }
    }

    return 0;
}

Rawboard Board::getKnightMoves(const Position position, const Side side) const {
    const Rawboard posIndex = posInd(position);
    return (noNoEa(posIndex) | noEaEa(posIndex) |
        soEaEa(posIndex) | soSoEa(posIndex) |
        soSoWe(posIndex) | soWeWe(posIndex) |
        noWeWe(posIndex) | noNoWe(posIndex)) &
        (EMPTY | OPPOSITE(side));
}

Rawboard Board::getBishopMoves(const Position position, const Side side) const {
    const Rawboard posIndex = posInd(position);
    const Rawboard oppositeBoard = OPPOSITE(side);

    return slidingAttack(noEastOne, posIndex, oppositeBoard) |
        slidingAttack(soEastOne, posIndex, oppositeBoard) |
        slidingAttack(soWestOne, posIndex, oppositeBoard) |
        slidingAttack(noWestOne, posIndex, oppositeBoard);
}

Rawboard Board::getRookMoves(const Position position, const Side side) const {
    const Rawboard posIndex = posInd(position);
    const Rawboard oppositeBoard = OPPOSITE(side);

    return slidingAttack(northOne, posIndex, oppositeBoard) |
        slidingAttack(eastOne, posIndex, oppositeBoard) |
        slidingAttack(southOne, posIndex, oppositeBoard) |
        slidingAttack(westOne, posIndex, oppositeBoard);
}

Rawboard Board::getQueenMoves(const Position position, const Side side) const {
    return getBishopMoves(position, side) | getRookMoves(position, side);
}

Rawboard Board::getKingMoves(const Position position, const Side side, const CastlingInfo castlingInfo) const {
    return getKingAttacks(position, side) | getKingCastling(position, side, castlingInfo);
}

Rawboard Board::getKingAttacks(const Position position, const Side side) const {
    const Rawboard posIndex = posInd(position);
    return (northOne(posIndex) | noEastOne(posIndex) | eastOne(posIndex) | soEastOne(posIndex) |
        southOne(posIndex) | soWestOne(posIndex) | westOne(posIndex) | noWestOne(posIndex))
        & (EMPTY | OPPOSITE(side));
}

Rawboard Board::getKingCastling(const Side side, const CastlingInfo castlingInfo) const {
    const Rawboard board = pieceBoards[WKing + side];
    const Position position = Utils::getFirstPos(board);
    return getKingCastling(position, side, castlingInfo);
}

Rawboard Board::getKingCastling(const Position position, const Side side, const CastlingInfo castlingInfo) const {
    Rawboard positions = 0L;

    if (position == 4 && side) {
        if (CastlingHelper::isBlackKingCastling(castlingInfo) && isEmpty(5)) {
            positions |= posInd(6) & EMPTY;
        }
        if (CastlingHelper::isBlackQueenCastling(castlingInfo) && isEmpty(3) && isEmpty(1)) {
            positions |= posInd(2) & EMPTY;
        }
    }
    else if (position == 60 && !side) {
        if (CastlingHelper::isWhiteKingCastling(castlingInfo) && isEmpty(61)) {
            positions |= posInd(62) & EMPTY;
        }
        if (CastlingHelper::isWhiteQueenCastling(castlingInfo) && isEmpty(59) && isEmpty(57)) {
            positions |= posInd(58) & EMPTY;
        }
    }

    return positions;
}

Rawboard Board::slidingAttack(Rawboard(*direction)(Rawboard), const Rawboard position, const Rawboard oppositeBoard) const {
    Rawboard positions = 0;
    Rawboard newPos = position;

    while (newPos) {
        const Rawboard attack = direction(newPos);
        const Rawboard emptyPos = attack & EMPTY;

        if (!emptyPos) {
            positions |= attack & oppositeBoard;
            break;
        }

        positions |= emptyPos;
        newPos = emptyPos;
    }

    return positions;
}

// sliding piece
Rawboard Board::rookAttack(const Position position, const Side side) const {
    const Rawboard occupied = ~EMPTY;
    const Rawboard notSide = ~BOARD(side);
    return (northAttack(occupied, position) | eastAttack(occupied, position) | southAttack(occupied, position) | westAttack(occupied, position)) & notSide;
}

Rawboard Board::bishopAttack(const Position position, const Side side) const {
    const Rawboard occupied = ~EMPTY;
    const Rawboard notSide = ~BOARD(side);;
    return (noEastAttack(occupied, position) | soEastAttack(occupied, position) | soWestAttack(occupied, position) | noWestAttack(occupied, position)) & notSide;
}

Rawboard Board::queenAttacks(const Position position, const Side side) const {
    return rookAttack(position, side) | bishopAttack(position, side);
}
