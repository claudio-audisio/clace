#include "player.h"
#include "../board/piece.h"
#include "../utils/pieceHelper.h"
#include "../engine/r_engine.h"

Player::Player() {
}

Player::Player(const bool white) {
	init("H725", true, white, new R_Engine());
}

Player::Player(const bool white, IEngine* engine) {
	init("H725", true, white, engine);
}

Player::Player(const string& name, const bool white) {
	init(name, false, white, new R_Engine());
}

Player::~Player() {
	if (engine) {
		delete engine;
	}
}

void Player::init(const string& name, const bool computer, const bool white, IEngine* engine) {
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

void Player::setPieces(char* pieces) {
	for (unsigned int i = 0; i < 7; i++) {
		this->pieces[i] = pieces[i];
	}
}

void Player::resetPieces() {
	pieces[WKing] = 0;
	pieces[WQueen] = 0;
	pieces[WRook] = 0;
	pieces[WBishop] = 0;
	pieces[WKnight] = 0;
	pieces[WPawn] = 0;
}

void Player::incrementPieces(const Piece piece) {
	if (PieceHelper::isBlack(piece)) {
		pieces[piece - 6]++;
	}
	else {
		pieces[piece]++;
	}
}


string Player::getNameAndColor() {
	return name + (white ? " (white)" : " (black)");
}

void Player::onCaptured(const Piece piece) {
	if (PieceHelper::isBlack(piece)) {
		pieces[piece - 6]--;
	}
	else {
		pieces[piece]--;
	}
}

/*
public String getCapturedList() {
	String capturedList = "";

	if (pieces[WKing] != 1) {
		capturedList = addToCapturedList(capturedList, 1, white ? WKing : BKing);
	}

	if (pieces[WQueen] != 1) {
		capturedList = addToCapturedList(capturedList, 1, white ? WQueen : BQueen);
	}

	if (pieces[WRook] != 2) {
		capturedList = addToCapturedList(capturedList, 2 - pieces[WRook], white ? WRook : BRook);
	}

	if (pieces[WBishop] != 2) {
		capturedList = addToCapturedList(capturedList, 2 - pieces[WBishop], white ? WBishop : BBishop);
	}

	if (pieces[WKnight] != 2) {
		capturedList = addToCapturedList(capturedList, 2 - pieces[WKnight], white ? WKnight : BKnight);
	}

	if (pieces[WPawn] != 8) {
		capturedList = addToCapturedList(capturedList, 8 - pieces[WPawn], white ? WPawn : BPawn);
	}

	return capturedList;
}

private String addToCapturedList(String capturedList, int times, byte pieceType) {
	while (times > 0) {
		capturedList = capturedList.concat((char)PrintUtils.getPieceCode(pieceType) + " ");
		times--;
	}

	return capturedList;
}

public void startMoveTime() {
	currentMoveTime = System.currentTimeMillis();
}

public void stopMoveTime() {
	long timeMillis = System.currentTimeMillis();
	gameTime += timeMillis - currentMoveTime;
}

public String getMoveTime() {
	return String.format("%02d:%02d:%02d", gameTime / 3600000, (gameTime / 60000) % 60, (gameTime / 1000) % 60);
}
*/