#include "game.h"
#include "../utils/pieceHelper.h"
#include "../utils/castlingInfoHelper.h"

Game::Game() {
	castlingInfo = 0b1111;
}

Game::~Game() {

}

Board& Game::getBoard() {
	return board;
}

Piece Game::getPiece(Position position) {
	return board.getPiece(position);
}

Piece Game::getWhiteKingPosition() {
	return whiteKingPosition;
}

Piece Game::getBlackKingPosition() {
	return blackKingPosition;
}

bool Game::isWhiteToMove() {
	return whiteToMove;
}

bool Game::isBlackKingCastling() {
	return CastlingInfoHelper::isBlackKingCastling(castlingInfo);
}

bool Game::isBlackQueenCastling() {
	return CastlingInfoHelper::isBlackQueenCastling(castlingInfo);
}

bool Game::isWhiteKingCastling() {
	return CastlingInfoHelper::isWhiteKingCastling(castlingInfo);
}

bool Game::isWhiteQueenCastling() {
	return CastlingInfoHelper::isWhiteQueenCastling(castlingInfo);
}

CastlingInfo Game::getCastlingInfo() {
	return castlingInfo;
}

Position Game::getEnPassantPosition() {
	return enPassantPosition;
}

unsigned int Game::getHalfMoveClock() {
	return halfMoveClock;
}

unsigned int Game::getFullMoves() {
	return fullMoves;
}

void Game::setWhiteToMove(bool white) {
	whiteToMove = white;
}

void Game::setCastlingInfo(CastlingInfo info) {
	castlingInfo = info;
}

void Game::setEnPassantPosition(Position position) {
	enPassantPosition = position;
}

void Game::setHalfMoveClock(int halfMove) {
	halfMoveClock = halfMove;
}

void Game::setFullMoves(int moves) {
	fullMoves = moves;
}

void Game::resetPlayersPieces() {
	whitePlayer.resetPieces();
	blackPlayer.resetPieces();
}

void Game::incrementPlayerPieces(Piece piece) {
	if (PieceHelper::isWhitePiece(piece)) {
		whitePlayer.incrementPieces(piece);
	}
	else {
		blackPlayer.incrementPieces(piece);
	}
}

void Game::setKingPositions() {
	for (Position i = 0; i < 64; i++) {
		if (isKing(i)) {
			if (isWhite(i)) {
				whiteKingPosition = i;
			}
			else {
				blackKingPosition = i;
			}
		}
	}
}

Piece Game::setPiece(Position position, Piece piece) {
	return board.setPiece(position, piece);
}

Piece Game::setEmptyPiece(Position position) {
	return setPiece(position, Empty);
}

bool Game::isWhite(Position position) {
	return board.isWhite(position);
}

bool Game::isKing(Position position) {
	return board.isKing(position);
}