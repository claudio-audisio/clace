#pragma once

#include <algorithm>
#include <cctype>
#include <locale>
#include <thread>
#include <future>

#include "../ui/userInterface.h"
#include "../utils/logger.h"
#include "movesProcessor.h"

using namespace std;

class UciProcessor {
public:
	Game game;
	bool waitingPosition = false;
	bool receivingMoves = false;
	bool calculatingMove = false;
	future<string> move;
	MovesProcessor movesProcessor;
	Logger& logger = Logger::getInstance();

	void run() {
		while (true) {
			string command = UI::readString();

			logger.log("received command: " + command);

			if (isQuit(command)) {
				break;
			}

			processCommand(command);
		}
	}

	void processCommand(string& command) {
		if (receivingMoves) {
			if (isGo(command)) {
				receivingMoves = false;
				string bestMove = movesProcessor.calculateMove(game);
				//move = async(launch::async, &MovesProcessor::calculateMove, &movesProcessor, game);
				//calculatingMove = true;
				sendCommand("bestmove " + bestMove);
				//calculatingMove = false;
			} else {
				movesProcessor.processMove(game, command);
			}
		} else if (isUci(command)) {
			sendCommand("id name Clace");
			sendCommand("id author Claudio Audisio");
			sendCommand("uciok");
		} else if (isIsReady(command)) {
			sendCommand("readyok");
		} else if (isUciNewGame(command)) {
			createNewGame();
		} else if (isPosition(command)) {
			waitingPosition = true;
		} else if (isStartPos(command)) {
			waitingPosition = false;
		} else if (isMoves(command)) {
			receivingMoves = true;
			movesProcessor.resetIndex();
		} else if (isGo(command)) {
			string bestMove = movesProcessor.calculateMove(game);
			//move = async(launch::async, &MovesProcessor::calculateMove, &movesProcessor, game);
			//calculatingMove = true;
			sendCommand("bestmove " + bestMove);
			//calculatingMove = false;
		}
	}

	void polish(string& command) {
		// TODO
	}

	bool isQuit(string& command) {
		return command == "quit";
	}

	bool isUci(string& command) {
		return command == "uci";
	}

	bool isIsReady(string& command) {
		return command == "isready";
	}

	bool isUciNewGame(string& command) {
		return command == "ucinewgame";
	}

	bool isPosition(string& command) {
		return command == "position";
	}

	bool isStartPos(string& command) {
		return command == "startpos";
	}

	bool isMoves(string& command) {
		return command == "moves";
	}

	bool isGo(string& command) {
		return command == "go";
	}

	void sendCommand(string command) {
		logger.log("sending command: " + command);
		cout << command << endl;
	}

	void createNewGame() {
		logger.log("creating new game");
		game.init();
	}

};