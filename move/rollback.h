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

	void save(const Game& game);
	void rollback(Game& game);
	void lightSave(const Game& game);
	void lightRollback(Game& game);
	unsigned int getRollbackSize() const;
	void reset();

private:
	class MoveInfo {
	public:
		MoveInfo();
		~MoveInfo();

		Board board;
		bool whiteToMove;
		CastlingInfo castlingInfo;
		Position enPassantPosition;
		int fullMoves;
		int halfMoveClock;
		Position whiteKingPosition;
		Position blackKingPosition;
		char* whitePieces;
		char* blackPieces;

		void setBoard(const Board& board);
		void setWhitePieces(char* whitePieces);
		void setBlackPieces(char* blackPieces);
	};

	deque<MoveInfo*> boards;
	MoveInfo lightBoard;

};
