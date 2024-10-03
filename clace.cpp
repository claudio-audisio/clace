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
string getFenPerft(unsigned int index);

int main(int argc, char* argv[])
{
	if (argc == 0) {
		// uci mode
		// TODO
	} else if (strcmp(argv[1], "console") == 0) {
		// console mode
        locale::global(locale(""));
        cout.imbue(locale(""));
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
	} else {
		const Rawboard board = stoull((argv[1]));
		BoardUtils::printBoard(board);
		cout << BoardUtils::positionsCount(board) << endl;
	}
	
	return 0;
}

void manageGame() {
	// TODO
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
	}
	
	UI::addLines(2);
}

string getFenPerft(const unsigned int index) {
    switch (index) {
        default: return Positions::INITIAL_FEN_POSITION;
        case 2: return Positions::PERFT_FEN_POSITION_2;
        case 3: return Positions::PERFT_FEN_POSITION_3;
        case 4: return Positions::PERFT_FEN_POSITION_4;
        case 5: return Positions::PERFT_FEN_POSITION_5;
        case 6: return Positions::PERFT_FEN_POSITION_6;
    }
}
