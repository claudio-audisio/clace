#include <iostream>
#include <cmath>

#include "statistics.h"

Statistics::Statistics(const unsigned int gamesAmount) {
	consoleOutput = gamesAmount == 1;
	running.store(gamesAmount);
	whiteWins.store(0);
	blackWins.store(0);
	draws.store(0);
}

Statistics::~Statistics() {

}

void Statistics::gameEnded(const int gameResult) {
	if (gameResult > 0) {
		whiteWins++;
	}
	else if (gameResult < 0) {
		blackWins++;
	}
	else {
		draws++;
	}

	running--;

	if (!consoleOutput) {
		cout << "\r" << running.load();
	}
}

void Statistics::print() {
	if (!consoleOutput) {
		cout << "\r  White wins: " << whiteWins.load() << " (" << percentage(whiteWins.load()) << ")" << endl;
		cout << "  Black wins: " << blackWins.load() << " (" << percentage(blackWins.load()) << ")" << endl;
		cout << "  Draws: " << draws.load() << " (" << percentage(draws.load()) << ")" << endl;
	}
}

string Statistics::percentage(const unsigned int value) {
	const double total = whiteWins.load() + blackWins.load() + draws.load();
	const double percentage = ceil((((double)value * 100.0) / total) * 100.0) / 100.0;
	return to_string(percentage) + " %%";
}