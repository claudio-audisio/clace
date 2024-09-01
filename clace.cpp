// clace.cpp : Defines the entry point for the application.
//

#include "clace.h"
#include "io/userInterface.h"

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

		while (!exit) {
			clearScreen();
			printLogo();
			const unsigned int choice = menu();

			switch (choice) {
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
	const unsigned int type = readPerftType();
	const unsigned int depth = readDepth();
	unsigned int runs = readPerftQuantity();

	while (runs != 0) {
		if (type == 1) {
			// new Perft(INITIAL_FEN_POSITION, depth).run(true);
		}
		else {
			// new Perft(INITIAL_FEN_POSITION, depth).runBulk();
		}

		--runs;
	}
	
	waitInteraction();
}
