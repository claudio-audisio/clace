#include "board.h"
#include "../utils/boardUtils.h"

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

    pieceBoards[Empty] = EMPTY_BOARD;

    for (Piece& piece : piecePositions) {
        piece = Empty;
    }

	castlingInfo = 0;

	resetCalculated();
}

bool Board::equals(const Board& board) {
	for (RawboardIndex i = 1; i < SIZE; ++i) {
		if (pieceBoards[i] != board.pieceBoards[i]) {
			return false;
		}
	}

	for (Position i = 0; i < 64; i++) {
		if (piecePositions[i] != board.piecePositions[i]) {
			return false;
		}
	}

	return true;
}

void Board::setBoard(const Piece boardIndex, const Rawboard pieceBoard) {
    // TODO gestire piecePositions
    pieceBoards[boardIndex] = pieceBoard;
}

void Board::set(const Board& board) {
    for (RawboardIndex i = 0; i < SIZE; ++i) {
        pieceBoards[i] = board.pieceBoards[i];
    }

    for (int i = 0; i < 64; i++) {
        piecePositions[i] = board.piecePositions[i];
    }

	resetCalculated();
}

bool Board::isUnderCheck(const Position position, const Side side) {
    return BoardUtils::isUnderCheck(getAttacks(BLACK - side), position);
}

Rawboard Board::getPawnMoves(const Side side) {
    Rawboard attacks = 0;
    Rawboard board = pieceBoards[WPawn + side];

    while (board) {
        const Position position = Utils::getFirstPos(board);
        attacks |= getPawnMoves(position, side);
        board &= (board - 1);
    }

    return attacks;
}

Rawboard Board::getPawnAttacks(const Side side) {
	Rawboard board = pieceBoards[WPawn + side];

	if (!side) {
		return (noWestOne(board) | noEastOne(board)) & (EMPTY | OPP_PIECES(side));
	}
	else {
		return (soWestOne(board) | soEastOne(board)) & (EMPTY | OPP_PIECES(side));
	}
}

Rawboard Board::getPawnAttacks(const Position position, const Side side) {
	const Rawboard posIndex = posInd(position);

	if (!side) {
		return (noWestOne(posIndex) | noEastOne(posIndex)) & (EMPTY | OPP_PIECES(side));
	}
	else {
		return (soWestOne(posIndex) | soEastOne(posIndex)) & (EMPTY | OPP_PIECES(side));
	}
}

Rawboard Board::getPawnMoves(const Position position, const Side side) {
    const Rawboard posIndex = posInd(position);

    if (!side) {
        const Rawboard onePush = northOne(posIndex) & EMPTY;
        return onePush | (northOne(onePush) & EMPTY & ROW_4) |
			   ((noWestOne(posIndex) | noEastOne(posIndex)) & OPP_PIECES(side)) |
			   getPawnEnPassant(posIndex, side);
    }
    else {
        const Rawboard onePush = southOne(posIndex) & EMPTY;
        return onePush | (southOne(onePush) & EMPTY & ROW_5) |
			   ((soWestOne(posIndex) | soEastOne(posIndex)) & OPP_PIECES(side)) |
			   getPawnEnPassant(posIndex, side);
    }
}

// TODO qui sto controllando per ogni pedone se posso fare en passant,
// quando data la posizione enpassant ci sono solo due posizioni che possono usufruire di quella mossa
Rawboard Board::getPawnEnPassant(const Rawboard position, const Side side) {
    if (enPassantPosition != NO_POS) {
        const Rawboard passantPos = posInd(enPassantPosition);
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

Rawboard Board::getKingMoves(const Side side) {
	return getKingAttacks(side) | getKingCastling(side);
}

Rawboard Board::getKingMoves(const Position position, const Side side) {
    return kingAttack(position, OPP_PIECES(side)) | getKingCastling(position, side);
}

Rawboard Board::getKingCastling(const Side side) const {
    const Rawboard board = pieceBoards[WKing + side];
    const Position position = Utils::getFirstPos(board);
    return getKingCastling(position, side);
}

Rawboard Board::getKingCastling(const Position position, const Side side) const {
    Rawboard positions = 0L;

    if (position == 4 && side) {
        if ((castlingInfo & 0b0010) && isEmpty(5)) {
            positions |= posInd(6) & EMPTY;
        }
        if ((castlingInfo & 0b0001) && isEmpty(3) && isEmpty(1)) {
            positions |= posInd(2) & EMPTY;
        }
    }
    else if (position == 60 && !side) {
        if ((castlingInfo & 0b1000) && isEmpty(61)) {
            positions |= posInd(62) & EMPTY;
        }
        if ((castlingInfo & 0b0100) && isEmpty(59) && isEmpty(57)) {
            positions |= posInd(58) & EMPTY;
        }
    }

    return positions;
}


/*Rawboard Board::slidingAttack(Rawboard(*direction)(Rawboard), const Rawboard position, const Rawboard oppositeBoard) const {
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
}*/
