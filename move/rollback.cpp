#include "rollback.h"
#include "../game/game.h"

Rollback::Rollback() {
}

Rollback::~Rollback() {
}

void Rollback::save(Game& game) {
	MoveInfo* moveInfo = new MoveInfo();
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
	boards.push_front(moveInfo);
}

void Rollback::rollback(Game& game) {
	if (boards.empty()) {
		// TODO tirare eccezione
	}

	MoveInfo* moveInfo = boards.front();
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

