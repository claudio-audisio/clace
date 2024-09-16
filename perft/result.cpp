#include "result.h"


Result::Result(const unsigned int depth) {
	this->depth = depth;
	this->init();
}

Result::~Result() {
}

void Result::init() {
	for (unsigned int i = 0; i < depth; i++) {
		nodes.push_back(0);
		captures.push_back(0);
		enPassant.push_back(0);
		castling.push_back(0);
		promotions.push_back(0);
		checks.push_back(0);
		discoveryChecks.push_back(0);
		doubleChecks.push_back(0);
		checkmates.push_back(0);
	}
}

void Result::incrementNodes(unsLL qty, const unsigned int currentDepth) {
	nodes[currentDepth] += qty;
}

void Result::incrementCounters(const CheckStatus& checkStatus, const unsigned int currentDepth) {
	nodes[currentDepth]++;

	if (checkStatus.isCheck()) {
		checks[currentDepth]++;
	}

	if (checkStatus.isDiscoveryCheck()) {
		discoveryChecks[currentDepth]++;
	}

	if (checkStatus.isDoubleCheck()) {
		doubleChecks[currentDepth]++;
	}
}

void Result::incrementCounters(const Game::MoveResult& moveResult, const unsigned int currentDepth) {
	if (moveResult.isCaptured()) {
		captures[currentDepth]++;
	}

	if (moveResult.isEnPassant()) {
		enPassant[currentDepth]++;
	}

	if (moveResult.isCastling()) {
		castling[currentDepth]++;
	}

	if (moveResult.isPromoted()) {
		promotions[currentDepth]++;
	}
}

void Result::incrementCheckmates(const unsigned int currentDepth) {
	checkmates[currentDepth]++;
}

void Result::add(const Result& result) {
	if (depth != result.getDepth()) {
		return;
	}

	for (int i = 0; i < depth; i++) {
		nodes[i] += result.getNodes(i);
		captures[i] += result.getCaptures(i);
		enPassant[i] += result.getEnPassant(i);
		castling[i] += result.getCastling(i);
		promotions[i] += result.getPromotions(i);
		checks[i] += result.getChecks(i);
		discoveryChecks[i] += result.getDiscoveryChecks(i);
		doubleChecks[i] += result.getDoubleChecks(i);
		checkmates[i] += result.getCheckmates(i);
	}
}

void Result::print(const string& fenGame, const bool consoleMode) {
	if (!consoleMode) {
		const unsigned int maxTabs = 4;

		cout << endl << fenGame << endl;
		cout << endl << "depth\tnodes"
			<< createTabs(maxTabs) << "captures"
			<< createTabs(maxTabs - 1) << "enPassant"
			<< createTabs(maxTabs - 1) << "castles"
			<< createTabs(maxTabs) << "promotions"
			<< createTabs(maxTabs - 1) << "checks"
			<< createTabs(maxTabs) << "discovery checks"
			<< createTabs(maxTabs - 3) << "double checks"
			<< createTabs(maxTabs - 2) << "checkmates" << endl;

		for (unsigned int i = 0; i < depth; i++) {
			cout << (i + 1) << "\t\t" << formatUnsLL(nodes[i]);
			cout << getTabs(maxTabs + 1, formatUnsLL(nodes[i])) << formatUnsLL(captures[i]);
			cout << getTabs(maxTabs + 1, formatUnsLL(captures[i])) << formatUnsLL(enPassant[i]);
			cout << getTabs(maxTabs + 1, formatUnsLL(enPassant[i])) << formatUnsLL(castling[i]);
			cout << getTabs(maxTabs + 1, formatUnsLL(castling[i])) << formatUnsLL(promotions[i]);
			cout << getTabs(maxTabs + 1, formatUnsLL(promotions[i])) << formatUnsLL(checks[i]);
			cout << getTabs(maxTabs + 1, formatUnsLL(checks[i])) << formatUnsLL(discoveryChecks[i]);
			cout << getTabs(maxTabs + 1, formatUnsLL(discoveryChecks[i])) << formatUnsLL(doubleChecks[i]);
			cout << getTabs(maxTabs + 1, formatUnsLL(doubleChecks[i])) << formatUnsLL(checkmates[i]) << endl;
		}
	}

	printGeneratedMoves(getMoves(), elapsed);
}

void Result::print() {
	print("", true);
}

void Result::printGeneratedMoves(const unsLL moves, const unsLL time) {
	cout << endl << "generated " << moves << " moves in " << time << " msec (" << formatUnsLL((moves * 1000) / time) << " moves/sec)" << endl;
}

string Result::getTabs(unsigned int maxTabs, const string& number) {
	unsigned int tabs = maxTabs - (number.length() / 4);
	return createTabs(tabs);
}

string Result::createTabs(const unsigned int amount) {
	string stringTabs = "";

	for (int i = 0; i < amount; i++) {
		stringTabs = stringTabs.append("\t");
	}

	return stringTabs;
}


string Result::formatUnsLL(const unsLL number) {
	return to_string(number);
	//return DECIMAL_FORMAT.format(number);
}