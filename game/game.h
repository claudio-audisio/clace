#pragma once

#include <list>
#include <deque>

#include "../clace.h"
#include "../board/board.h"
#include "../move/move.h"
#include "../move/rollback.h"
#include "../board/checkStatus.h"
#include "../game/player.h"
#include "statistics.h"

using namespace std;

class Game {
public:
    Game();
    ~Game();

	class MoveResult;

	void init();
	void initFromFEN(const string& fenPosition);
	void initPlayers();
	void initPlayers(Player* white, Player* black);
	Piece rawMove(const Position source, const Position destination, const Piece piece);
	MoveResult* finalizeMove(Move& move);
	const MoveResult* applyMove(Move& move);
	void applyMoves(list<Move>& moves);
	void simulateMove(const Move& move);
	void verifyChecks();
	EndGameType checkEndGame();
	EndGameType checkFiftyMoveRule();
	bool checkFiveFoldRepetitions();
	bool isUnderCheck(const Position position, const bool white);
	bool checkControl(const Move& move);
	void setKingPositions();
	void updateKingPosition(const Move& move);
	void updateEnPassantInfo(const Move& move);
	void updateCastlingInfo(const Move& move);
	void completeCastlingMove(const Move& move);
	Piece completeEnPassant(const Move& move);
	void completePawnPromotion(const Move& move);
	bool processCapture(const Piece piece, const bool white);
	void changeTurn();
	Piece getPiece(const Position position) const;
	Piece setPiece(const Position position, const Piece piece);
	Piece setEmptyPiece(const Position position);
	bool checkColor(const Position position) const;
	bool checkColor(const Position position, const bool white) const;
	bool checkColor(const Move& move) const;
	bool isWhite(const Position position) const;
	bool isEmpty(const Position position) const;
	bool isPawn(const Position position) const;
	bool isRook(const Position position) const;
	bool isRook(const Position position, bool white) const;
	bool isKing(const Position position) const;
	bool isWhiteKingCastling() const;
	bool isWhiteQueenCastling() const;
	bool isBlackKingCastling() const;
	bool isBlackQueenCastling() const;
	void save();
	void lightSave();
	void rollbackLastMove();
	void lightRollback();
	Player* getCurrentPlayer() const;
	bool isComputerToMove() const;
	void setLastMove(Move* move);
	void setPlayerPieces(const Game& game);
	void resetPlayersPieces();
	void incrementPlayerPieces(const Piece piece);
	Game* duplicate();

	void setBoard(const Board& board) {
		this->board.set(board);
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

	list<Move*>& getNextMoves() {
		return nextMoves;
	}

	Move* getLastMove() {
		return lastMove;
	}

	deque<Move*>& getMovesHistory() {
		return movesHistory;
	}

	class MoveResult {
	public:
		MoveResult(bool captured, bool promoted, bool enPassant, bool castling) {
			this->captured = captured;
			this->promoted = promoted;
			this->enPassant = enPassant;
			this->castling = castling;
		}
		bool captured;
		bool promoted;
		bool enPassant;
		bool castling;

		bool isCaptured() const {
			return captured;
		}

		bool isPromoted() const {
			return promoted;
		}

		bool isEnPassant() const {
			return enPassant;
		}

		bool isCastling() const {
			return castling;
		}
	};

private:
	Board board;
	list<Move*> nextMoves;
	deque<Move*> movesHistory;	// TODO renderla thread safe
	Rollback rollback;
	CastlingInfo castlingInfo;
	CheckStatus checkStatus;
	Move* lastMove = nullptr;
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
