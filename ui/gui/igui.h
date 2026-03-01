#pragma once

#include <string>

#include "../../common/types.h"
#include "../../common/constants.h"

class Game;

class IGui {
public:
    ~IGui() = default;
    virtual void run() {}
    virtual Piece choosePromotionType() { return Empty; }
    virtual void notifyEndGame(const EndGameType endGame) {};
    virtual void onBoardChange(bool isComputerMove = false) {};
    virtual void showMessage(const std::string& message) {};
    virtual void setGameInfo(const Game& game) {};

};
