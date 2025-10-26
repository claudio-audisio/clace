#include "board.h"
#include "../utils/boardUtils.h"
#include "../utils/positions.h"

Board::Board() {
    reset();
}

Board::~Board() = default;


void Board::reset() {
    for (RawboardIndex i = 1; i < SIZE; ++i) {
        pieceBoards[i] = 0;
    }

    pieceBoards[Empty] = EMPTY_BOARD;

    for (Piece& piece : piecePositions) {
        piece = Empty;
    }

	castlingInfo = 0;
	enPassantPosition = NO_POS;
#ifdef BOARD_USE_PRE_CALCULATED
	resetCalculated();
#endif
}

bool Board::equals(const Board& board) const {
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

void Board::set(const Board& board) {
    for (RawboardIndex i = 0; i < SIZE; ++i) {
        pieceBoards[i] = board.pieceBoards[i];
    }

    for (int i = 0; i < 64; i++) {
        piecePositions[i] = board.piecePositions[i];
    }

	castlingInfo = board.castlingInfo;
	enPassantPosition = board.enPassantPosition;

#ifdef BOARD_USE_PRE_CALCULATED
	resetCalculated();
#endif
}
