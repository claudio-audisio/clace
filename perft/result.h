#pragma once

#include <chrono>
#include <vector>
#include <numeric>

#include "../utils/utils.h"
#include "../board/checkStatus.h"
#include "../game/game.h"


class Result {
public:
	Result(unsigned int depth);
	~Result();

	void init();
	void incrementNodes(unsLL qty, unsigned int currentDepth);
	void incrementCounters(const CheckStatus& checkStatus, unsigned int currentDepth);
	void incrementCounters(MoveResult moveResult, unsigned int currentDepth);
	void incrementCheckmates(unsigned int currentDepth);
	void add(const Result& result);
	void print(const string& fenGame, bool consoleMode);
	void print();

	static void printGeneratedMoves(unsLL moves, unsLL time);
	static string getTabs(unsigned int maxTabs, const string& number);
	static string createTabs(unsigned int amount);
	static string formatUnsLL(unsLL number);

	unsigned int getDepth() const {
		return depth;
	}

	unsLL getMoves() {
		return accumulate(nodes.begin(), nodes.end(), 0LL);
	}

	unsLL getNodes(const unsigned int i) const {
		return nodes[i];
	}

	unsLL getCaptures(const unsigned int i) const {
		return captures[i];
	}

	unsLL getEnPassant(const unsigned int i) const {
		return enPassant[i];
	}

	unsLL getCastling(const unsigned int i) const {
		return castling[i];
	}

	unsLL getPromotions(const unsigned int i) const {
		return promotions[i];
	}

	unsLL getChecks(const unsigned int i) const {
		return checks[i];
	}

	unsLL getDiscoveryChecks(const unsigned int i) const {
		return discoveryChecks[i];
	}

	unsLL getDoubleChecks(const unsigned int i) const {
		return doubleChecks[i];
	}

	unsLL getCheckmates(const unsigned int i) const {
		return checkmates[i];
	}

	void startTime() {
		time = chrono::steady_clock::now();
	}

	void stopTime() {
		elapsed = getElapsedMillis(time);
	}

	unsLL getElapsed() const {
		return elapsed;
	}

private:
	unsigned int depth;
	vector<unsLL> nodes;
	vector<unsLL> captures;
	vector<unsLL> enPassant;
	vector<unsLL> castling;
	vector<unsLL> promotions;
	vector<unsLL> checks;
	vector<unsLL> discoveryChecks;
	vector<unsLL> doubleChecks;
	vector<unsLL> checkmates;
	unsLL elapsed;
	chrono::time_point<chrono::steady_clock> time;

};