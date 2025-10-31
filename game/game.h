#pragma once

#include <list>
#include <deque>

#include "../board/board.h"
#include "../move/rollback.h"
#include "../board/checkStatus.h"
#include "statistics.h"
#include "../utils/messenger.h"

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
	Evaluation calculateMove();
	MoveResult finalizeMove(Move& move);
	MoveResult applyMove(Move& move);
	void applyMoves(list<Move>& moves);
	void simulateMove(Move& move);
	Piece completeEnPassant(Move move);
	void completePawnPromotion(Move move);
    void undoSimulateMove(Move& move);
    void undoEnPassant(Move& move);
	void verifyChecks();
	EndGameType checkEndGame(unsigned char legalMoves);
	bool checkFiftyMoveRule() const;
	bool checkFiveFoldRepetitions() const;
	bool checkControl(Move move);
	void changeTurn();
	Side getSide(Position position) const;
	void save();
	void rollbackLastMove();
	Player* getCurrentPlayer() const;
    Player* getOtherPlayer() const;
    bool isComputerToMove() const;
	void setLastMove(Move move);
	Game* duplicate();
    string printMovesHistory(int depth = 0) const;
    string printCastlingInfo() const;
	string getCapturedList(Side side);
	void calculateCheckPositions(Side side);
	int getAllDestinationQty(Side side);

	// Only for testing
	Board getCopyBoard() {
		return board;
	}

    bool isWhiteToMove() const {
		return sideToMove == _WHITE;
	}

    Side getOppositeSide() const {
		return OPPOSITE(sideToMove);
	}

    string getDescription() const;

    Board board;
	deque<Move> movesHistory;	// TODO renderla thread safe (davvero necessario?)
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
	Messenger& messenger = Messenger::getInstance();
};
