// clace.cpp : Defines the entry point for the application.
//

#include "clace.h"
#include "ui/userInterface.h"
#include "utils/boardUtils.h"
#include "board/board.h"
#include "perft/perft.h"
#include "utils/positions.h"

using namespace std;

void manageGame();
void managePerft();

int main(int argc, char* argv[])
{
	if (argc == 0) {
		// uci mode
		// TODO
	} else if (strcmp(argv[1], "console") == 0) {
		// console mode
		bool exit = false;
        UI::clearScreen();
        UI::printLogo();

		while (!exit) {
			switch (UI::menu()) {
				case 1: manageGame(); break;
				case 2: managePerft(); break;
				default: exit = true; break;
			}
		}
	}
	
	return 0;
}

void manageGame() {
	// TODO
}

void managePerft() {
	const unsigned int type = UI::readPerftType();
	const unsigned int depth = UI::readDepth();
	unsigned int runs = UI::readPerftQuantity();

	while (runs != 0) {
        auto perft = new Perft(Positions::INITIAL_FEN_POSITION, depth);

		if (type == 1) {
            perft->run(true);
		}
		else {
			perft->runBulk();
		}

		--runs;
	}
	
	UI::addLines(2);
}
