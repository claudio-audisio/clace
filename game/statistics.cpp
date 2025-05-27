#include <iostream>
#include <cmath>

#include "statistics.h"

#include <format>
#include <thread>

Statistics::Statistics(const unsigned int gamesAmount) :
running(gamesAmount) {
	consoleOutput = gamesAmount == 1;
	whiteWins.store(0);
	blackWins.store(0);
	draws.store(0);
}

Statistics::~Statistics() = default;

void Statistics::gameEnded(const int gameResult) {
	if (gameResult > 0) {
		++whiteWins;
	}
	else if (gameResult < 0) {
		++blackWins;
	}
	else {
		++draws;
	}

	running.count_down();

	/*if (!consoleOutput) {
		cout << "\r" << running.load();
	}*/
}

void Statistics::waitAllEnds() const {
	while (!running.try_wait()) {
		this_thread::sleep_for(chrono::seconds(1));
		cout << "\r completed " << whiteWins.load() + blackWins.load() + draws.load() << flush;
	}
}

void Statistics::print() {
	if (!consoleOutput) {
		cout << "\r white wins: " << whiteWins.load() << " (" << percentage(whiteWins.load()) << ")" << endl;
		cout << " black wins: " << blackWins.load() << " (" << percentage(blackWins.load()) << ")" << endl;
		cout << " draws: " << draws.load() << " (" << percentage(draws.load()) << ")" << endl;
		cout << endl;
	}
}

string Statistics::percentage(const unsigned int value) {
	const double total = whiteWins.load() + blackWins.load() + draws.load();
	const double percentage = ceil((((double)value * 100.0) / total) * 100.0) / 100.0;
	return format("{:.2f} %", percentage);
}