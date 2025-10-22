#pragma once

#include <string>

#include "../../common/types.h"
#include "../../common/constants.h"

class IGui {
public:
    ~IGui() = default;
    virtual void run() {}
    virtual Piece choosePromotionType() { return Empty; };
    virtual void onBoardChange(bool isComputerMove = false) {};
    virtual void showMessage(const std::string& message) {};
    virtual void setGameInfo(Side sideToMove, bool check, int moves, double evaluation, const std::string& fen, const std::string& lastMove, const std::string& whiteTime, const std::string& blackTime) {};

};
