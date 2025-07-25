#pragma once

#include <list>
#include <deque>

#include "../board/board.h"
#include "../move/rollback.h"
#include "../board/checkStatus.h"
#include "statistics.h"

using namespace std;

class Player;
class IEvaluator;

class Game {
public:
    Game();
    ~Game();

	void init();
	void initFromFEN(const string& fenPosition);
	void initPlayers(Player* white, Player* black);
	MoveResult finalizeMove(Move& move);
	MoveResult finalizeMoveNew(Move& move);
	MoveResult applyMove(Move& move);
	MoveResult applyMoveNew(Move& move);
	void applyMoves(list<Move>& moves);
	void simulateMove(Move& move);
    void undoSimulateMove(Move& move);
    void undoEnPassant(Move& move);
	void verifyChecks();
	EndGameType checkEndGame(bool noMoves);
	EndGameType checkFiftyMoveRule();
	bool checkFiveFoldRepetitions();
	bool checkControl(const Move& move);
	void updateEnPassantInfo(const Move& move);
	Piece completeEnPassant(const Move& move);
	void completePawnPromotion(const Move& move);
	void changeTurn();
	Side getSide(Position position) const;
	void save();
	void rollbackLastMove();
	Player* getCurrentPlayer() const;
	bool isComputerToMove() const;
	void setLastMove(const Move& move);
	Game* duplicate();
    string printMovesHistory(int depth) const;
    string printCastlingInfo() const;
	string getCapturedList(Side side);
	void calculateCheckPositions(Side side);
	int getAllDestinationQty(Side side);

	// Only for testing
	Board getCopyBoard() {
		return board;
	}

    bool isWhiteToMove() const {
		return sideToMove == WHITE;
	}

    Side getOppositeSide() const {
		return BLACK - sideToMove;
	}

	Board board;
	deque<Move> movesHistory;	// TODO renderla thread safe
	Rollback rollback;
	CheckStatus checkStatus;
	Move lastMove = 0;
	Side sideToMove;
	unsigned int fullMoves;
	unsigned int halfMoveClock;
	Player* whitePlayer = nullptr;
	Player* blackPlayer = nullptr;
	IEvaluator* evaluator = nullptr;

	double currentEvaluation = 0;
};
