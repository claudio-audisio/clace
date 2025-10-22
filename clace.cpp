#include "clace.h"

#include <ranges>

#include "engine/ab_engine.h"
#include "engine/bf_engine.h"
#include "perft/perft.h"
#include "uci/uciProcessor.h"
#include "ui/gui/gui.h"

using namespace std;

unsigned int getMode(char* mode) {
	if (strcmp(mode, "console") == 0) {
		return 1;
	}

	if (strcmp(mode, "gui") == 0) {
		return 2;
	}

	cout << "Usage: clace [|console|gui]" << endl;

	exit(0);
}

int main(int argc, char* argv[]) {
	const unsigned int mode = argc == 1 ? 0 : getMode(argv[1]);
	Clace clace(mode);

	auto future = async(launch::async, [&]() {
		clace.run();
	});

	future.wait();

	return 0;
}

/* mode: 0 -> uci, 1 -> console, 2 -> gui */
Clace::Clace(const unsigned int mode) {
	initAttacks();
	this->uciMode = mode == 0;
	this->guiMode = mode == 2;
}

Clace::~Clace() {
	stopGame();
};

void Clace::run() {
	if (uciMode) {
		// uci mode
		messenger.send(MSG_LOG, "clace", "=================================================");
		messenger.send(MSG_LOG, "clace", "started application in uci mode");
		UciProcessor processor;
		processor.run();
	 } else if (guiMode) {
	 	// gui mode
	 	messenger.send(MSG_LOG, "clace", "=================================================");
	 	messenger.send(MSG_LOG, "clace", "started application in gui mode");
	 	gui = new Gui();
	 	messenger.registerGui(*gui);
		auto guiFuture = async(launch::async, [&]() {
	 		return gui->run();
		});
	 	messenger.send(MSG_LOG, "clace", "gui started successfully");
	 } else {
		// console mode
		messenger.send(MSG_LOG, "clace", "=================================================");
		messenger.send(MSG_LOG, "clace", "started application in console mode");
		bool run = true;
		UI::clearScreen();
		UI::printLogo();
		UI::menu();

		while (run) {
			run = processCommand(UI::readLine());
		}
	}
}

bool Clace::processCommand(const string& command) {
	if (strcmp(command.c_str(), "new") == 0) {
		newHumanGame("");
	} else if (strcmp(command.c_str(), "next") == 0) {
		manageNextMove(gameRunner, gui);
	} else if (strncmp(command.c_str(), "load ", 5) == 0) {
		newHumanGame(command.substr(5));
	} else if (strncmp(command.c_str(), "cpu ", 4) == 0) {
		newCpuGame(stoi(command.substr(4)));
	} else if (strncmp(command.c_str(), "perft ", 6) == 0) {
		managePerft(parsePerftParams(command.substr(6)));
	} else if (strncmp(command.c_str(), "perfc ", 6) == 0) {
		managePerftComplete(parsePerftParams(command.substr(6)));
	} else if (strncmp(command.c_str(), "print ", 6) == 0) {
		printBoards(stoull(command.substr(6)));
	} else if (strcmp(command.c_str(), "help") == 0) {
		UI::menu();
	} else if (strcmp(command.c_str(), "quit") == 0) {
		return false;
	} else {
		processMove(gameRunner, gui, command);
	}

	return true;
}

pair<int, int> Clace::parsePerftParams(const string& params) {
	const size_t pos = params.find(' ');

	if (pos == string::npos) {
		return make_pair(stoi(params), 1);
	}

	return make_pair(stoi(params.substr(0, pos)), stoi(params.substr(pos + 1)));
}

void Clace::manageNextMove(GameRunner *gameRunner, IGui *gui) {
	if (!gameRunner) {
		UI::printMessage("no game running");
		gui->showMessage("no game running");
		return;
	}

	Game& game = *(gameRunner->game);

	if (game.getCurrentPlayer()->computer) {
		UI::printMessage("wait your turn");
		gui->showMessage("no game running");
		return;
	}

	const Evaluation evaluation = game.getOtherPlayer()->engine->calculateMove(game);
	cout << endl << " --> " << moveToString(evaluation.move) << endl;
	//processMove(gameRunner, gui, moveToString(evaluation.move));
}


void Clace::processMove(GameRunner *gameRunner, IGui *gui, const string& move) {
	if (gameRunner) {
		if (move.length() == 4 && isValidMove(move)) {
			gameRunner->setHumanMove(move);
		} else {
			UI::printMessage("invalid move");
			gui->showMessage("invalid move");
		}
	} else {
		UI::printMessage("no game running");
		gui->showMessage("no game running");
	}
}

bool Clace::isValidMove(const string& move) {
	return (move.at(0) >= 'a' && move.at(0) <= 'h') &&
			(move.at(1) >= '1' && move.at(1) <= '8') &&
			(move.at(2) >= 'a' && move.at(2) <= 'h') &&
			(move.at(3) >= '1' && move.at(3) <= '8');
}

void Clace::stopGame() {
	if (gameRunner != nullptr) {
		gameRunner->stop();
		gameFuture.wait();
		delete gameRunner;
		delete statistics;
		gameRunner = nullptr;
		statistics = nullptr;
	}
}

void Clace::newHumanGame(const string& fenGame) {
	stopGame();
	statistics = new Statistics(1);
	gameRunner = fenGame.empty() ? new GameRunner(statistics) : new GameRunner(statistics, HvsC, fenGame);
	gameFuture = async(launch::async, [&]() {
		return gameRunner->run();
	});
}

void Clace::newCpuGame(int gamesAmount) {
	stopGame();

	if (gamesAmount > 1) {
		messenger.stopLogger();
	}

	statistics = new Statistics(gamesAmount);
	vector<GameRunner*> games;
	vector<future<void>> futures;

	for (int i = 0; i < gamesAmount; i++) {
		auto game = new GameRunner(statistics, CvsC);
		games.push_back(game);
		futures.push_back(async(launch::async, [game]() {
			return game->run();
		}));
	}

	if (gamesAmount > 1) {
		statistics->waitAllEnds();
	}

	for (auto& future : futures) {
		future.wait();
	}

	for (auto& game : games) {
		delete game;
	}

	statistics->print();

	delete statistics;
	statistics = nullptr;

	if (gamesAmount > 1) {
		messenger.startLogger();
	}
}

void Clace::printBoards(Rawboard board) {
	UI::addLines(1);
	printBoard(board);
	UI::addLines(1);
	cout << " " << positionsCount(board) << endl;
	UI::addLines(1);
}

void Clace::managePerft(pair<int, int> params) {
	for (int i = 0; i < params.second; i++) {
		UI::addLines(1);
		auto perft = new Perft(getFenPerft(1), params.first);
		perft->runBulk();
		delete perft;
	}

	UI::addLines(1);
}

void Clace::managePerftComplete(pair<int, int> params) {
	for (int i = 0; i < params.second; i++) {
		UI::addLines(1);
		auto perft = new Perft(getFenPerft(1), params.first);
		perft->runComplete(true);
		delete perft;
	}

	UI::addLines(1);
}

string Clace::getFenPerft(const unsigned int index) {
    switch (index) {
        default: return INITIAL_FEN_POSITION;
        case 2: return PERFT_FEN_POSITION_2;
        case 3: return PERFT_FEN_POSITION_3;
        case 4: return PERFT_FEN_POSITION_4;
        case 5: return PERFT_FEN_POSITION_5;
        case 6: return PERFT_FEN_POSITION_6;
    }
}
