// clace.cpp : Defines the entry point for the application.
//

#include "clace.h"
#include "ui/userInterface.h"
#include "utils/boardUtils.h"
#include "board/board.h"
#include "perft/perft.h"
#include "game/gameRunner.h"
#include "utils/logger.h"
#include "uci/uciProcessor.h"

using namespace std;

void manageGame();
void managePerft();
string getFenPerft(unsigned int index);
void printBoards();

int main(int argc, char* argv[]) {
	Logger& logger = Logger::getInstance();

	if (argc == 1) {
		// uci mode
		logger.log("started application in uci mode");
		UciProcessor processor;
		processor.run();
	} else if (strcmp(argv[1], "console") == 0) {
		// console mode
		logger.log("started application in console mode");
		bool exit = false;
        UI::clearScreen();
        UI::printLogo();

		while (!exit) {
			switch (UI::menu()) {
				case 1: manageGame(); break;
				case 2: managePerft(); break;
				case 3: printBoards(); break;
				default: exit = true; break;
			}
		}
	}
	
	return 0;
}

void manageGame() {
	string whiteName;
	string blackName;
	unsigned int gamesQuantity = 1;
	string fenBoard;

	switch (UI::menuGame()) {
		case 2: whiteName = UI::readName("white"); break;
		case 3: blackName = UI::readName("black"); break;
		case 4: whiteName = UI::readName("white");
			blackName = UI::readName("black"); break;
		case 5: gamesQuantity = UI::readGamesQuantity(); break;
		case 6: whiteName = UI::readName("");
			fenBoard = UI::readBoard(); break;
		default: break;
	}

	auto statistics = new Statistics(gamesQuantity);

	if (fenBoard.empty()) {
		for (int i = 0; i < gamesQuantity; i++) {
			auto runner = new GameRunner(whiteName, blackName, statistics, fenBoard);
			runner->run();
		}
	} else {
		if (!FEN::isWhiteToMove(fenBoard)) {
			blackName = whiteName;
			whiteName = "";
		}

		auto runner = new GameRunner(whiteName, blackName, statistics, fenBoard);
		runner->run();
	}

	statistics->print();
}

void printBoards() {
	string stringBoard;
	do {
		stringBoard = UI::readString();
		const Rawboard board = stoull(stringBoard);

		if (!board) {
			break;
		}

		BoardUtils::printBoard(board);
		cout << BoardUtils::positionsCount(board) << endl;
	} while (true);
}

void managePerft() {
	const unsigned int type = UI::readPerftType();
    const unsigned int index = UI::readPerftIndex();
	const unsigned int depth = UI::readDepth();
	unsigned int runs = UI::readPerftQuantity();

	while (runs != 0) {
        auto perft = new Perft(getFenPerft(index), depth);

		if (type == 1) {
            perft->run(true);
		}
		else {
			perft->runBulk();
		}

		--runs;
		delete perft;
	}
	
	UI::addLines(2);
}

string getFenPerft(const unsigned int index) {
    switch (index) {
        default: return INITIAL_FEN_POSITION;
        case 2: return PERFT_FEN_POSITION_2;
        case 3: return PERFT_FEN_POSITION_3;
        case 4: return PERFT_FEN_POSITION_4;
        case 5: return PERFT_FEN_POSITION_5;
        case 6: return PERFT_FEN_POSITION_6;
    }
}
