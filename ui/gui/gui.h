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
    void newGame(const string& fenGame);
    void closeGame() const;
    void loadFonts();
    void loadTextures();
    void loadButtons();
    void loadInfo();
    void onBoardChange(bool isComputerMove = false) override;
    void drawBoard() const;
    void drawPieces();
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
    void setGameInfo(const Game& game) override;
    void resetGameInfo();
    Piece choosePromotionType() override;
    void notifyEndGame(EndGameType endGame) override;

    static int getCell(const Vector2 pos) {
        if (pos.x < BOARD_START_X || pos.x > BOARD_END_X || pos.y < BOARD_START_Y || pos.y > BOARD_END_Y) {
            return -1;
        }


        return (static_cast<int>(BOARD_END_Y - pos.y) / CELL_SIZE) * 8 + (static_cast<int>(pos.x - BOARD_START_X) / CELL_SIZE);
    }

    static Vector2 getStartPos(const int cell) {
        return { BOARD_START_X + (cell % 8) * CELL_SIZE , (BOARD_END_Y - CELL_SIZE) - (cell / 8) * CELL_SIZE };
    }

private:
    Messenger& messenger = Messenger::getInstance();
    Statistics* statistics = nullptr;
    GameRunner* gameRunner = nullptr;
    future<void> gameFuture;
    Piece piecePositions[64];
    int cellSelected = -1;
    int cellDestination = -1;
    int computerCellDestination = -1;
    Vector2 computerMovePosition;
    Vector2 computerMoveDelta;

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
    unsigned int moves;
    Evaluation evaluation;
    string fen;
    string lastMove;
    string whiteTime;
    string blackTime;
    EndGameType endGame;

};


