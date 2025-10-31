#include "rollback.h"
#include "../game/game.h"

Rollback::Rollback(const unsigned int size) {
	this->size = size;
	this->boards = static_cast<MoveInfo**>(malloc(size * sizeof(MoveInfo*)));
	for (int i = 0; i < size; i++) {
		boards[i] = new MoveInfo();
	}
}

Rollback::~Rollback() {
	for (int i = 0; i < size; i++) {
		delete boards[i];
	}
	free(boards);
}

void Rollback::save(const Game& game) {
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

	const MoveInfo* moveInfo = boards[--index];
	copy(&moveInfo->board, &game.board);
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
	copy(&board, &this->board);
}
