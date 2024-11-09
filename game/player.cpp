#include <chrono>

#include "player.h"
#include "../board/piece.h"
#include "../utils/pieceHelper.h"
#include "../engine/r_engine.h"
#include "../ui/userInterface.h"
#include "../utils/utils.h"

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
}

string Player::getNameAndColor() {
	return name + (white ? " (white)" : " (black)");
}

/*


string Player::getCapturedList() {
	string capturedList = "";

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

string& Player::addToCapturedList(string& capturedList, unsigned int times, Piece piece) {
	while (times > 0) {
		capturedList += string(1, UI::getPieceCode(piece)) + " ";
		times--;
	}

	return capturedList;
}*/

void Player::startMoveTime() {
	currentMoveTime = chrono::steady_clock::now();
}

void Player::stopMoveTime() {
	gameTime += Utils::getElapsedMillis(currentMoveTime);
}

string Player::getMoveTime() {
	ostringstream stringStream;
	stringStream << int(gameTime / 3600000) << ":" << int((gameTime / 60000) % 60) << ":" << int((gameTime / 1000) % 60);
	return stringStream.str();
}
