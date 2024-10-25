#include "rollback.h"
#include "../game/game.h"

Rollback::Rollback() {
	boards.reserve(size);
	for (int i = 0; i < size; i++) {
		boards[i] = new MoveInfo();
	}
}

Rollback::~Rollback() {
	for (int i = 0; i < size; i++) {
		delete boards[i];
	}
}

void Rollback::save(Game& game) {
	if (index == size) {
		boards[index] = new MoveInfo();
		size++;
	}

	MoveInfo* moveInfo = boards[index++];
	moveInfo->setBoard(game.board);
	moveInfo->sideToMove = game.sideToMove;
	moveInfo->castlingInfo = game.board.castlingInfo;
	moveInfo->enPassantPosition = game.board.enPassantPosition;
	moveInfo->fullMoves = game.fullMoves;
	moveInfo->halfMoveClock = game.halfMoveClock;
	moveInfo->whiteKingPosition = game.whiteKingPosition;
	moveInfo->blackKingPosition = game.blackKingPosition;
	moveInfo->setWhitePieces(game.whitePlayer->pieces);
	moveInfo->setBlackPieces(game.blackPlayer->pieces);
}

void Rollback::rollback(Game& game) {
	if (index == 0) {
		// TODO tirare eccezione
	}

	MoveInfo* moveInfo = boards[--index];
	game.board.set(moveInfo->board);
	game.sideToMove = moveInfo->sideToMove;
	game.board.castlingInfo = moveInfo->castlingInfo;
	game.board.enPassantPosition = moveInfo->enPassantPosition;
	game.fullMoves = moveInfo->fullMoves;
	game.halfMoveClock = moveInfo->halfMoveClock;
	game.whiteKingPosition = moveInfo->whiteKingPosition;
	game.blackKingPosition = moveInfo->blackKingPosition;
	game.whitePlayer->setPieces(moveInfo->whitePieces);
	game.blackPlayer->setPieces(moveInfo->blackPieces);
}

unsigned int Rollback::getRollbackSize() const {
	return index;
}

void Rollback::reset() {
	index = 0;
}

Rollback::MoveInfo::MoveInfo() {
	whitePieces = new Piece[7];
	blackPieces = new Piece[7];
}

Rollback::MoveInfo::~MoveInfo() {
	// TODO un po' di delete di vattori
}

void Rollback::MoveInfo::setBoard(const Board& board) {
	this->board.set(board);
}

void Rollback::MoveInfo::setWhitePieces(Piece* whitePieces) {
	for (RawboardIndex i = 0; i < 7; i++) {
		this->whitePieces[i] = whitePieces[i];
	}
}

void Rollback::MoveInfo::setBlackPieces(Piece* blackPieces) {
	for (RawboardIndex i = 0; i < 7; i++) {
		this->blackPieces[i] = blackPieces[i];
	}
}

