#pragma once

#include <deque>

#include "../common/types.h"
#include "../board/board.h"
#include "../utils/messenger.h"

using namespace std;

class Game;

class Rollback {
public:
	explicit Rollback(unsigned int size);
	~Rollback();

	unsigned int index = 0;
	unsigned int size;
	void save(const Game& game);
	void rollback(Game& game);
	unsigned int getRollbackSize() const;
	void reset();

private:
	class MoveInfo {
	public:
		MoveInfo();
		~MoveInfo();

		Board board;
		Side sideToMove;
		unsigned int fullMoves;
		unsigned int halfMoveClock;

		void setBoard(const Board& board);
	};

	MoveInfo** boards;
	Messenger& messenger = Messenger::getInstance();
};
