#pragma once

#include <atomic>
#include <string>
#include <latch>

using namespace std;

class Statistics {
public:
	explicit Statistics(unsigned int gamesAmount, bool gui = false);
	~Statistics();

	void gameEnded(int gameResult);
	void waitAllEnds() const;
	void print();
	string percentage(unsigned int value);

	latch running;
	atomic<unsigned int> blackWins;
	atomic<unsigned int> whiteWins;
	atomic<unsigned int> draws;
	bool consoleOutput;
};