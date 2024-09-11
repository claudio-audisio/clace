#pragma once

#include <atomic>
#include <string>

using namespace std;

class Statistics {
public:
	Statistics(const unsigned int gamesAmount);
	~Statistics();

	void gameEnded(const int gameResult);
	void print();
	string percentage(const unsigned int value);

private:
	bool consoleOutput;
	atomic<unsigned int> running;
	atomic<unsigned int> blackWins;
	atomic<unsigned int> whiteWins;
	atomic<unsigned int> draws;

};