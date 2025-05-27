#pragma once

#include <future>
#include <string>

#include "game/gameRunner.h"

class Clace {
public:
    Clace(bool uciMode);
    ~Clace();

    IEngine* engine = nullptr;
    Logger& logger = Logger::getInstance();
    Statistics* statistics = nullptr;
    GameRunner* gameRunner = nullptr;
    future<void> gameFuture;
    bool uciMode;

    void run();
    void newHumanGame(const string& fenGame);
    void newCpuGame(int gamesAmount);
    void stopGame();
    static void managePerft(int depth);
    static void managePerftComplete();
    static string getFenPerft(unsigned int index);
    static void printBoards(Rawboard board);
    void manageNextMove() const;
    bool processCommand(const string& command);
    void processMove(const string& move) const;
    static bool isValidMove(const string& move);
};
