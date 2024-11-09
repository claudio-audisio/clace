#pragma once

#include <deque>

#include "../common/types.h"
#include "../board/board.h"

using namespace std;

class Game;

class Rollback {
public:
	Rollback();
	~Rollback();

	unsigned int index = 0;
	unsigned int size = 10;
	void save(Game& game);
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

	vector<MoveInfo*> boards;
};
