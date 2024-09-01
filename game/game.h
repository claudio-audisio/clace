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

    private:
		Board board;
		list<Move> nextMoves;
		deque<Move> movesHistory;	// TODO renderla thread safe
		Rollback rollback;
		CastlingInfo castlingInfo;
		CheckStatus checkStatus;
		Move lastMove;
		bool whiteToMove;
		Position whiteKingPosition;
		Position blackKingPosition;
		Position enPassantPosition = NO_POS;
		int fullMoves;
		int halfMoveClock;
		//IEvaluator evaluator = null;		// TODO da capire
		Player whitePlayer;
		Player blackPlayer;
		//GamesStatistics statistics;		// TODO da fare
};
