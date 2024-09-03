#pragma once

#include <list>
#include <deque>

#include "../clace.h"
#include "../board/board.h"
#include "../move/move.h"
#include "../move/rollback.h"
#include "../board/checkStatus.h"
#include "../game/player.h"

using namespace std;


class Game {
    public:
        Game();
        ~Game();

		Board& getBoard();
		Piece getPiece(Position position);
		Piece getWhiteKingPosition();
		Piece getBlackKingPosition();
		bool isWhiteToMove();
		bool isBlackKingCastling();
		bool isBlackQueenCastling();
		bool isWhiteKingCastling();
		bool isWhiteQueenCastling();
		CastlingInfo getCastlingInfo();
		Position getEnPassantPosition();
		unsigned int getHalfMoveClock();
		unsigned int getFullMoves();
		void setCastlingInfo(CastlingInfo info);
		void setEnPassantPosition(Position position);
		void setHalfMoveClock(int halfMove);
		void setFullMoves(int moves);
		void setWhiteToMove(bool white);
		void resetPlayersPieces();
		void incrementPlayerPieces(Piece piece);
		Piece setPiece(Position position, Piece piece);
		Piece setEmptyPiece(Position position);
		void setKingPositions();
		bool isWhite(Position position);
		bool isKing(Position position);

    private:
		Board board;
		list<Move> nextMoves;
		deque<Move> movesHistory;	// TODO renderla thread safe
		Rollback rollback;
		CastlingInfo castlingInfo;
		CheckStatus checkStatus;
		Move* lastMove = 0;
		bool whiteToMove;
		Position whiteKingPosition;
		Position blackKingPosition;
		Position enPassantPosition = NO_POS;
		unsigned int fullMoves;
		unsigned int halfMoveClock;
		//IEvaluator evaluator = null;		// TODO da capire
		Player whitePlayer;
		Player blackPlayer;
		//GamesStatistics statistics;		// TODO da fare
};
