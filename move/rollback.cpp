#include "rollback.h"
#include "../game/game.h"

Rollback::Rollback() {
	boards.reserve(size);
	for (int i = 0; i < size; i++) {
		boards.push_back(new MoveInfo());
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
	moveInfo->fullMoves = game.fullMoves;
	moveInfo->halfMoveClock = game.halfMoveClock;
}

void Rollback::rollback(Game& game) {
	if (index == 0) {
		throw runtime_error("asked rollback but here there is no data");
	}

	MoveInfo* moveInfo = boards[--index];
	game.board.set(moveInfo->board);
	game.sideToMove = moveInfo->sideToMove;
	game.fullMoves = moveInfo->fullMoves;
	game.halfMoveClock = moveInfo->halfMoveClock;
}

unsigned int Rollback::getRollbackSize() const {
	return index;
}

void Rollback::reset() {
	index = 0;
}

Rollback::MoveInfo::MoveInfo() {
}

Rollback::MoveInfo::~MoveInfo() {
}

void Rollback::MoveInfo::setBoard(const Board& board) {
	this->board.set(board);
}

