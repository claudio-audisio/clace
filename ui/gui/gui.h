#pragma once

#include <future>
#include <string>

#include "button.h"
#include "raylib.h"
#include "guiConstants.h"
#include "igui.h"
#include "panel.h"
#include "../../game/gameRunner.h"
#include "../../game/statistics.h"

class Gui : public IGui {
public:
    Gui();
    ~Gui() = default;

    void init();
    void run();
    void newGame(const string& fenGame = "");
    void closeGame() const;
    void loadFonts();
    void loadTextures();
    void loadButtons();
    void loadInfo();
    void onBoardChange(bool isComputerMove = false) override;
    void drawBoard() const;
    void drawPieces() const;
    void drawInfoPanel();
    void drawSelection(int selectedCell) const;
    void drawButtons() const;
    void drawFenPanel() const;
    void drawPromotionPanel() const;
    Vector2 getPromotionPanelPosition() const;
    void dragAndDropPiece();
    void makeMove(Piece piece);
    void unloadTextures() const;
    void checkButtonPressed();
    void showMessage(const string& message) override;
    void setGameInfo(Side sideToMove, bool check, int moves, double evaluation, const std::string& fen, const std::string& lastMove, const std::string& whiteTime, const std::string& blackTime) override;
    void resetGameInfo();
    Piece choosePromotionType() override;

    int getCell(Vector2 pos) const {
        if (pos.x < BOARD_START_X || pos.x > BOARD_END_X || pos.y < BOARD_START_Y || pos.y > BOARD_END_Y) {
            return -1;
        }

        return (static_cast<int>(pos.y - BOARD_START_Y) / CELL_SIZE) * 8 + (static_cast<int>(pos.x - BOARD_START_X) / CELL_SIZE);
    }

    Vector2 getStartPos(int cell) const {
        return { BOARD_START_X + (cell % 8) * CELL_SIZE , BOARD_START_Y + (cell / 8) * CELL_SIZE };
    }

private:
    Statistics* statistics = nullptr;
    GameRunner* gameRunner = nullptr;
    future<void> gameFuture;
    Piece piecePositions[64];
    int cellSelected = -1;
    int cellDestination = -1;
    int computerCellDestination = -1;
    Vector2 computerPiecePosition;

    Texture2D boardTexture;
    Texture2D pieces[12];
    Texture2D copyIcon;
    Texture2D editIcon;
    Font font;
    Button newButton;
    Button fenButton;
    Button hintButton;
    Button noneButton;
    Button copyButton;
    Button editButton;
    Panel panel;
    bool showFenPanel;
    bool showPromotionPanel;
    Piece promotion;

    // Game info
    Side sideToMove;
    bool check;
    int moves;
    double evaluation;
    string fen;
    string lastMove;
    string whiteTime;
    string blackTime;

};


