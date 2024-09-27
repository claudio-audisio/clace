#include "rollback.h"
#include "../game/game.h"

Rollback::Rollback() {
}

Rollback::~Rollback() {
}

void Rollback::save(const Game& game) {
	MoveInfo* moveInfo = new MoveInfo();
	moveInfo->setBoard(game.getBoard());
	moveInfo->whiteToMove = game.isWhiteToMove();
	moveInfo->castlingInfo = game.getCastlingInfo();
	moveInfo->enPassantPosition = game.getEnPassantPosition();
	moveInfo->fullMoves = game.getFullMoves();
	moveInfo->halfMoveClock = game.getHalfMoveClock();
	moveInfo->whiteKingPosition = game.getWhiteKingPosition();
	moveInfo->blackKingPosition = game.getBlackKingPosition();
	moveInfo->setWhitePieces(game.getWhitePlayer()->getPieces());
	moveInfo->setBlackPieces(game.getBlackPlayer()->getPieces());
	boards.push_front(moveInfo);
}

void Rollback::rollback(Game& game) {
	if (boards.empty()) {
		// TODO tirare eccezione
	}

	MoveInfo* moveInfo = boards.front();
	game.setBoard(moveInfo->board);
	game.setWhiteToMove(moveInfo->whiteToMove);
	game.setCastlingInfo(moveInfo->castlingInfo);
	game.setEnPassantPosition(moveInfo->enPassantPosition);
	game.setFullMoves(moveInfo->fullMoves);
	game.setHalfMoveClock(moveInfo->halfMoveClock);
	game.setWhiteKingPosition(moveInfo->whiteKingPosition);
	game.setBlackKingPosition(moveInfo->blackKingPosition);
	game.getWhitePlayer()->setPieces(moveInfo->whitePieces);
	game.getBlackPlayer()->setPieces(moveInfo->blackPieces);
	boards.pop_front();
    delete moveInfo;
}

/*void Rollback::lightSave(const Game& game) {
	lightBoard.setBoard(game.getBoard());
	lightBoard.whiteKingPosition = game.getWhiteKingPosition();
	lightBoard.blackKingPosition = game.getBlackKingPosition();
}

void Rollback::lightRollback(Game& game) {
	game.setBoard(lightBoard.board);
	game.setWhiteKingPosition(lightBoard.whiteKingPosition);
	game.setBlackKingPosition(lightBoard.blackKingPosition);
}*/

unsigned int Rollback::getRollbackSize() const {
	return boards.size();
}

void Rollback::reset() {
	while (!boards.empty()) {
		MoveInfo* board = boards.front();
		delete board;
		boards.pop_front();
	}
}

Rollback::MoveInfo::MoveInfo() {
	whitePieces = new char[7];
	blackPieces = new char[7];
}

Rollback::MoveInfo::~MoveInfo() {
	// TODO un po' di delete di vattori
}

void Rollback::MoveInfo::setBoard(const Board& board) {
	this->board.set(board);
}

void Rollback::MoveInfo::setWhitePieces(char* whitePieces) {
	for (RawboardIndex i = 0; i < 7; i++) {
		this->whitePieces[i] = whitePieces[i];
	}
}

void Rollback::MoveInfo::setBlackPieces(char* blackPieces) {
	for (RawboardIndex i = 0; i < 7; i++) {
		this->blackPieces[i] = blackPieces[i];
	}
}

