#pragma once

#include <atomic>
#include <string>

using namespace std;

class Statistics {
public:
	explicit Statistics(unsigned int gamesAmount);
	~Statistics();

	void gameEnded(int gameResult);
	void print();
	string percentage(unsigned int value);

	bool consoleOutput;
	atomic<unsigned int> running;
	atomic<unsigned int> blackWins;
	atomic<unsigned int> whiteWins;
	atomic<unsigned int> draws;

};