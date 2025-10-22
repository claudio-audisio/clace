#pragma once

#include <future>
#include <string>

#include "game/gameRunner.h"
#include "ui/gui/igui.h"
#include "utils/messenger.h"

class Clace {
public:
    Clace(unsigned int mode);
    ~Clace();

    IGui *gui = new IGui();
    Statistics* statistics = nullptr;
    GameRunner* gameRunner = nullptr;
    future<void> gameFuture;
    Messenger& messenger = Messenger::getInstance();
    bool uciMode;
    bool guiMode;

    void run();
    void newHumanGame(const string& fenGame);
    void newCpuGame(int gamesAmount);
    void stopGame();
    static void managePerft(pair<int, int> params);
    static void managePerftComplete(pair<int, int> params);
    static string getFenPerft(unsigned int index);
    static void printBoards(Rawboard board);
    static void manageNextMove(GameRunner *gameRunner, IGui *gui);
    bool processCommand(const string& command);
    static void processMove(GameRunner *gameRunner, IGui *gui, const string& move);
    static pair<int, int> parsePerftParams(const string& params);
    static bool isValidMove(const string& move);
};
