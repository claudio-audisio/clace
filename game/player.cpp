#include "player.h"
#include "../board/piece.h"
#include "../utils/pieceHelper.h"

Player::Player() {
}

Player::~Player() {
	if (engine != 0) {
		delete engine;
	}
}

void Player::init(string& name, bool computer, bool white, IEngine* engine) {
	this->name = name;
	this->computer = computer;
	this->white = white;
	this->engine = engine;
	initPieces();
}

void Player::initPieces() {
	pieces[WKing] = 1;
	pieces[WQueen] = 1;
	pieces[WRook] = 2;
	pieces[WBishop] = 2;
	pieces[WKnight] = 2;
	pieces[WPawn] = 8;
}

void Player::resetPieces() {
	pieces[WKing] = 0;
	pieces[WQueen] = 0;
	pieces[WRook] = 0;
	pieces[WBishop] = 0;
	pieces[WKnight] = 0;
	pieces[WPawn] = 0;
}

void Player::incrementPieces(Piece piece) {
	if (PieceHelper::isBlackPiece(piece)) {
		pieces[piece - 6]++;
	}
	else {
		pieces[piece]++;
	}
}