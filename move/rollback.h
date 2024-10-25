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
	unsigned int size = 5;
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
		CastlingInfo castlingInfo;
		Position enPassantPosition;
		unsigned int fullMoves;
		unsigned int halfMoveClock;
		Position whiteKingPosition;
		Position blackKingPosition;
		Piece* whitePieces;
		Piece* blackPieces;

		void setBoard(const Board& board);
		void setWhitePieces(Piece* whitePieces);
		void setBlackPieces(Piece* blackPieces);
	};

	vector<MoveInfo*> boards;
};
