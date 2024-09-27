#pragma once

#include <list>
#include <deque>

#include "../clace.h"
#include "../board/board.h"
#include "../move/rollback.h"
#include "../board/checkStatus.h"
#include "../game/player.h"
#include "statistics.h"

using namespace std;


class Game {
public:
    Game();
    ~Game();

	void init();
	void initFromFEN(const string& fenPosition);
	void initPlayers();
	void initPlayers(Player* white, Player* black);
	Piece rawMove(Position source, Position destination, Piece piece);
	MoveResult finalizeMove(Move& move);
	MoveResult applyMove(Move& move);
	void applyMoves(list<Move>& moves);
	void simulateMove(Move& move);
    void undoSimulateMove(Move& move);
    void undoKingPosition(Move& move);
    void undoCastlingMove(Move& move);
    void undoEnPassant(Move& move);
	void verifyChecks();
	EndGameType checkEndGame(bool noMoves);
	EndGameType checkFiftyMoveRule();
	bool checkFiveFoldRepetitions();
	bool isUnderCheck(Position position, bool white);
	bool checkControl(const Move& move);
	void setKingPositions();
	void updateKingPosition(const Move& move);
	void updateEnPassantInfo(const Move& move);
	void completeCastlingMove(const Move& move);
	Piece completeEnPassant(const Move& move);
	void completePawnPromotion(const Move& move);
	bool processCapture(Piece piece, bool white);
	void changeTurn();
	Piece getPiece(Position position) const;
	Piece setPiece(Position position, Piece piece);
	Piece setEmptyPiece(Position position);
	bool checkColor(Position position) const;
	bool checkColor(Position position, bool white) const;
	bool checkColor(const Move& move) const;
	bool isWhite(Position position) const;
	bool isEmpty(Position position) const;
	bool isPawn(Position position) const;
	bool isRook(Position position) const;
	bool isRook(Position position, bool white) const;
	bool isKing(Position position) const;
	bool isWhiteKingCastling() const;
	bool isWhiteQueenCastling() const;
	bool isBlackKingCastling() const;
	bool isBlackQueenCastling() const;
	void save();
	//void lightSave();
	void rollbackLastMove();
	//void lightRollback();
	Player* getCurrentPlayer() const;
	bool isComputerToMove() const;
	void setLastMove(const Move& move);
	void setPlayerPieces(const Game& game);
	void resetPlayersPieces();
	void incrementPlayerPieces(Piece piece);
	Game* duplicate();
    Rawboard getRawBoard(bool white) const;
    string printMovesHistory();
    string printCastlingInfo() const;

	void setBoard(const Board& b) {
		this->board.set(b);
	}

	const Board& getBoard() const {
		return board;
	}

	void setWhiteKingPosition(Position position) {
		whiteKingPosition = position;
	}

	void setBlackKingPosition(Position position) {
		blackKingPosition = position;
	}

	Piece getWhiteKingPosition() const {
		return whiteKingPosition;
	}

	Piece getBlackKingPosition() const {
		return blackKingPosition;
	}

	bool isWhiteToMove() const {
		return whiteToMove;
	}

	CastlingInfo getCastlingInfo() const {
		return castlingInfo;
	}

	Position getEnPassantPosition() const {
		return enPassantPosition;
	}

	unsigned int getHalfMoveClock() const {
		return halfMoveClock;
	}

	unsigned int getFullMoves() const {
		return fullMoves;
	}

	void setWhiteToMove(const bool white) {
		whiteToMove = white;
	}

	void setCastlingInfo(const CastlingInfo info) {
		castlingInfo = info;
	}

	void setEnPassantPosition(const Position position) {
		enPassantPosition = position;
	}

	void setHalfMoveClock(const int halfMove) {
		halfMoveClock = halfMove;
	}

	void setFullMoves(const int moves) {
		fullMoves = moves;
	}
		
	CheckStatus& getCheckStatus() {
		return checkStatus;
	}

	Player* getWhitePlayer() const {
		return whitePlayer;
	}

	Player* getBlackPlayer() const {
		return blackPlayer;
	}

	Move getLastMove() const {
		return lastMove;
	}

	deque<Move>& getMovesHistory() {
		return movesHistory;
	}

    void addNextMove(const Move move) {
        nextMoves.push_back(move);
    }

    vector<Move>& getNextMoves() {
        return nextMoves;
    }

    void clearNextMoves() {
        nextMoves.clear();
    }

private:
	Board board;
    vector<Move> nextMoves;
	deque<Move> movesHistory;	// TODO renderla thread safe
	Rollback rollback;
	CastlingInfo castlingInfo;
	CheckStatus checkStatus;
	Move lastMove = 0;
	bool whiteToMove;
	Position whiteKingPosition;
	Position blackKingPosition;
	Position enPassantPosition = NO_POS;
	unsigned int fullMoves;
	unsigned int halfMoveClock;
	Player* whitePlayer = nullptr;
	Player* blackPlayer = nullptr;
	Statistics* statistics = nullptr;
		
};
