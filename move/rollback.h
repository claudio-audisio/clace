#pragma once

#include <deque>

#include "../common/types.h"
#include "../board/board.h"

using namespace std;


class Rollback {
	public:
		Rollback();
		~Rollback();

	private:
		class MoveInfo {
		public:
			MoveInfo();
			~MoveInfo();

		private:
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
		};

		deque<MoveInfo> boards;
		MoveInfo lightSave;

};
