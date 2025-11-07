#include "gui.h"
#include "guiConstants.h"
#include "../../game/gameRunner.h"
#include "../../clace.h"

Gui::Gui() {
}

void Gui::init() {
    newGame();
    resetGameInfo();
    InitWindow(WINDOW_W, WINDOW_H, TITLE);
    SetTargetFPS(60);
    loadFonts();
    loadTextures();
    loadButtons();
    loadInfo();
    showFenPanel = false;
    showPromotionPanel = false;
}


void Gui::run() {
    init();
    int selectedCell = -1;

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();

        // cell selection
        /*if (gameRunner && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            const int newSelection = getCell(GetMousePosition());
            selectedCell = newSelection == selectedCell ? -1 : newSelection;
            logger.log(format("cell selected: {}", selectedCell));
        }*/

        checkButtonPressed();
        dragAndDropPiece();

        BeginDrawing();
        ClearBackground(BG_COLOR);

        drawBoard();
        drawPieces();
        drawButtons();
        drawInfoPanel();

        if (showFenPanel) {
            drawFenPanel();
        }

        if (showPromotionPanel) {
            drawPromotionPanel();
        }

        EndDrawing();
    }

    CloseWindow();

    UnloadTexture(boardTexture);
    unloadTextures();
    UnloadFont(font);
}

void Gui::drawFenPanel() const {
    DrawRectangle(FEN_PANEL_X, FEN_PANEL_Y, FEN_PANEL_W, FEN_PANEL_H, RAYWHITE);
    DrawRectangleLinesEx({FEN_PANEL_X, FEN_PANEL_Y, FEN_PANEL_W, FEN_PANEL_H}, 4, BG_COLOR);
    copyButton.draw();
    editButton.draw();
    DrawTextEx(font, fen.c_str(), {FEN_PANEL_X + 40, FEN_PANEL_Y + 15}, 30, 1, BG_COLOR);
}

void Gui::drawPromotionPanel() const {
    if (cellDestination < 0 || cellDestination > 7) {
        throw std::runtime_error("Invalid cell for pawn promotion");
    }

    DrawRectangleV({BOARD_START_X, BOARD_START_Y}, {BOARD_W, BOARD_H}, ColorAlpha(WHITE, 0.7));
    Vector2 pos = getPromotionPanelPosition();
    DrawRectangleLinesEx({pos.x - 5, pos.y - 5, CELL_SIZE * 4 + 10, CELL_SIZE + 10}, 5, BG_COLOR);

    for (int i = 0; i < 4; i++) {
        Color cellColor = ((i % 2) + ((i / 8) % 2)) % 2 == 0 ? CELL_COLOR_1 : CELL_COLOR_2;
        DrawRectangleV({pos.x + CELL_SIZE * i, pos.y}, {CELL_SIZE, CELL_SIZE}, cellColor);
        DrawTextureEx(pieces[WKnight + i * 2 - 1], {pos.x + 15 + CELL_SIZE * i, pos.y + 15}, 0, PIECE_SCALE, ColorAlpha(WHITE, PIECE_ALPHA));
    }
}

Vector2 Gui::getPromotionPanelPosition() const {
    Vector2 pos = getStartPos(cellDestination);
    pos.x = pos.x + CELL_SIZE / 2;
    pos.y = pos.y + CELL_SIZE / 2;

    if (cellDestination > 3) {
        pos.x = pos.x - CELL_SIZE * 4;
    }

    return pos;
}

void Gui::drawBoard() const {
    for (int i = 0; i < 64; i++) {
        Color cellColor = ((i % 2) + ((i / 8) % 2)) % 2 == 0 ? CELL_COLOR_1 : CELL_COLOR_2;
        DrawRectangleV(getStartPos(i), {CELL_SIZE, CELL_SIZE}, cellColor);
    }
    DrawTextEx(font, "ABCDEFGH", {BOARD_START_X + 40, BOARD_END_Y + 10}, 30, CELL_SIZE - 13.5, ColorAlpha(RAYWHITE, 0.8));

    for (int i = 0; i < 8; i++) {
        DrawTextEx(font, format("{}", i + 1).c_str(), {BOARD_START_X - 30, BOARD_END_Y - (CELL_SIZE * i) - 65}, 30, 1, ColorAlpha(RAYWHITE, 0.8));
    }
}

void Gui::drawPieces() const {
    for (int i = 0; i < 64; i++) {
        if (piecePositions[i]) {
            Vector2 piecePosition;

            if (i == cellSelected) {
                Vector2 mousePosition = GetMousePosition();
                piecePosition.x = mousePosition.x - PIECE_SIZE / 2 * PIECE_SCALE;
                piecePosition.y = mousePosition.y - PIECE_SIZE / 2 * PIECE_SCALE;
            } else {
                Vector2 startCell = getStartPos(i);
                piecePosition.x = startCell.x + 15;
                piecePosition.y = startCell.y + 15;
            }

            // TODO far vedere la mossa del computer lentamente
            /*if (i == computerCellDestination) {

            } else {*/
                DrawTextureEx(pieces[piecePositions[i] - 1], piecePosition, 0, PIECE_SCALE, ColorAlpha(WHITE, PIECE_ALPHA));
            //}
        }
    }
}

void Gui::drawInfoPanel() {
    const int lineSpacing = INFO_FONT_SIZE + 5;
    DrawTextEx(font, format("{} to move", sideToMove ? "black" : "white").c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing}, INFO_FONT_SIZE, 1, RAYWHITE);
    DrawTextEx(font, format("time {} - {}", whiteTime, blackTime).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 2}, INFO_FONT_SIZE, 1, RAYWHITE);
    DrawTextEx(font, format("{} moves", moves).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 3}, INFO_FONT_SIZE, 1, RAYWHITE);
    DrawTextEx(font, format("evaluation {:.2f}", evaluation).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 4}, INFO_FONT_SIZE, 1, RAYWHITE);
    DrawTextEx(font, format("last move {}", lastMove).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 5}, INFO_FONT_SIZE, 1, RAYWHITE);

    if (check) {
        DrawTextEx(font, format("CHECK!", fen).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 6}, INFO_FONT_SIZE, 1, RAYWHITE);
    }

    panel.draw();
}

void Gui::drawSelection(const int selectedCell) const {
    if (selectedCell != -1) {
        Vector2 startCell = getStartPos(selectedCell);
        DrawCircle(startCell.x + CELL_SIZE / 2, startCell.y + CELL_SIZE / 2, 25, ColorAlpha(VIOLET, 0.3));
    }
}

void Gui::drawButtons() const {
    newButton.draw();
    fenButton.draw();
    hintButton.draw();
    noneButton.draw();
}

void Gui::loadFonts() {
    font = LoadFont("../ui/gui/asset/NotoSans-SemiBold.ttf");
}

void Gui::loadTextures() {
    boardTexture = LoadTexture("../ui/gui/asset/chessBoard.png");
    pieces[WPawn - 1] = LoadTexture("../ui/gui/asset/whitePawn.png");
    pieces[WKnight - 1] = LoadTexture("../ui/gui/asset/whiteKnight.png");
    pieces[WBishop - 1] = LoadTexture("../ui/gui/asset/whiteBishop.png");
    pieces[WRook - 1] = LoadTexture("../ui/gui/asset/whiteRook.png");
    pieces[WQueen - 1] = LoadTexture("../ui/gui/asset/whiteQueen.png");
    pieces[WKing - 1] = LoadTexture("../ui/gui/asset/whiteKing.png");
    pieces[BPawn - 1] = LoadTexture("../ui/gui/asset/blackPawn.png");
    pieces[BKnight - 1] = LoadTexture("../ui/gui/asset/blackKnight.png");
    pieces[BBishop - 1] = LoadTexture("../ui/gui/asset/blackBishop.png");
    pieces[BRook - 1] = LoadTexture("../ui/gui/asset/blackRook.png");
    pieces[BQueen - 1] = LoadTexture("../ui/gui/asset/blackQueen.png");
    pieces[BKing - 1] = LoadTexture("../ui/gui/asset/blackKing.png");
    copyIcon = LoadTexture("../ui/gui/asset/copyIcon.png");
    editIcon = LoadTexture("../ui/gui/asset/editIcon.png");
}

void Gui::loadButtons() {
    const int padding = (WINDOW_W - PADDING - BOARD_W - PADDING - BUTTON_W * 4 - PADDING) / 3;
    newButton.init({INFO_START_X, INFO_START_Y}, {BUTTON_W, BUTTON_H}, CELL_COLOR_1, BG_COLOR, "New", font);
    fenButton.init({INFO_START_X + BUTTON_W + padding, INFO_START_Y}, {BUTTON_W, BUTTON_H}, CELL_COLOR_1, BG_COLOR, "Fen", font);
    hintButton.init({INFO_START_X + (BUTTON_W + padding) * 2, INFO_START_Y}, {BUTTON_W, BUTTON_H}, CELL_COLOR_1, BG_COLOR, "Hint", font);
    noneButton.init({INFO_START_X + (BUTTON_W + padding) * 3, INFO_START_Y}, {BUTTON_W, BUTTON_H}, CELL_COLOR_1, BG_COLOR, "", font);
    copyButton.init({FEN_PANEL_X + FEN_PANEL_W - 80, FEN_PANEL_Y + 18}, {25, 25}, RAYWHITE, WHITE, copyIcon, 0.05);
    editButton.init({FEN_PANEL_X + FEN_PANEL_W - 40, FEN_PANEL_Y + 18}, {25, 25}, RAYWHITE, WHITE, editIcon, 0.05);
}

void Gui::loadInfo() {
    panel.init({PANEL_START_X, PANEL_START_Y}, {PANEL_W, PANEL_H}, font);
}

void Gui::onBoardChange(const bool isComputerMove /* = false */) {
    // TODO far vedere la mossa del computer lentamente
    /*if (isComputerMove) {
        Position computerCellSource = getSourcePosition(gameRunner->game->lastMove);
        computerCellDestination = getDestinationPosition(gameRunner->game->lastMove);
        computerPiecePosition = getStartPos(computerCellSource);
        // TODO qua prendo le coordinate iniziale del pezzo da far muove
        // devo calcolare le coordinate finali e il delta di movimento (x e y)
        // quando a forza di aggiugnere i delta arrivo a destinazione ho finito di fare il movimento
    }*/

    memcpy(piecePositions, gameRunner->game->board->piecePositions, sizeof(Piece) * 64);
}

void Gui::newGame(const string& fenGame /*= ""*/) {
    panel.clean();
    memset(piecePositions, Empty, sizeof(piecePositions));
    statistics = new Statistics(1, true);
    gameRunner = fenGame.empty() ? new GameRunner(statistics) : new GameRunner(statistics, HvsC, fenGame);
    gameFuture = async(launch::async, [&]() {
        return gameRunner->run(this);
    });
}

void Gui::closeGame() const {
    gameRunner->stop();
    gameFuture.wait();
    delete gameRunner;
    delete statistics;
}

void Gui::dragAndDropPiece() {
    if (cellSelected == -1) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            const int selection = getCell(GetMousePosition());

            if (isWhite(gameRunner->game->board, selection)) {
                cellSelected = selection;
            }
        } else {
            cellSelected = -1;
        }
    } else {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            cellDestination = getCell(GetMousePosition());

            if (cellDestination != cellSelected) {
                if (cellDestination < 8 && piecePositions[cellSelected] == WPawn) {  // promotion
                    showPromotionPanel = true;
                } else {
                    makeMove(piecePositions[cellSelected]);
                }
            } else {
                cellSelected = cellDestination = -1;
            }
        }
    }
}

void Gui::makeMove(const Piece piece) {
    piecePositions[cellDestination] = piece;
    piecePositions[cellSelected] = Empty;
    gameRunner->setHumanMove(indexToCoords(cellSelected)+indexToCoords(cellDestination));
    panel.clean();
    cellSelected = cellDestination = -1;
}

void Gui::unloadTextures() const {
    UnloadTexture(boardTexture);

    for (int i = 0; i < 12; i++) {
        UnloadTexture(pieces[i]);
    }
}

void Gui::checkButtonPressed() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        const Vector2 mousePos = GetMousePosition();

        if (showPromotionPanel) {
            const Vector2 pos = getPromotionPanelPosition();

            if (CheckCollisionPointRec(mousePos, {pos.x, pos.y, CELL_SIZE, CELL_SIZE})) {
                promotion = WKnight;
            } else if (CheckCollisionPointRec(mousePos, {pos.x + CELL_SIZE, pos.y, CELL_SIZE, CELL_SIZE})) {
                promotion = WBishop;
            } else if (CheckCollisionPointRec(mousePos, {pos.x + CELL_SIZE * 2, pos.y, CELL_SIZE, CELL_SIZE})) {
                promotion = WRook;
            } else if (CheckCollisionPointRec(mousePos, {pos.x + CELL_SIZE * 3, pos.y, CELL_SIZE, CELL_SIZE})) {
                promotion = WQueen;
            } else {
                return;
            }

            makeMove(promotion);
            showPromotionPanel = false;
            return;
        }

        if (showFenPanel) {
            if (copyButton.isPressed(mousePos)) {
                SetClipboardText(fen.c_str());
                panel.write("fen copied into clipboard");
            } else if (editButton.isPressed(mousePos)) {
                const string newFen = GetClipboardText();

                if (newFen.empty()) {
                    panel.write("inserted fen is empty");
                } else {
                    if (FEN::isValid(newFen)) {
                        panel.write("inserted valid fen");
                        closeGame();
                        newGame(newFen);
                    }
                }
            }

            showFenPanel = false;
            return;
        }

        if (newButton.isPressed(mousePos)) {
            closeGame();
            newGame();
            return;
        }

        if (fenButton.isPressed(mousePos)) {
            showFenPanel = true;
            return;
        }

        if (hintButton.isPressed(mousePos)) {
            Clace::manageNextMove(gameRunner, this);
        }
    }
}

void Gui::showMessage(const string& message) {
    panel.write(message);
}

void Gui::setGameInfo(Side sideToMove, bool check, int moves, double evaluation, const std::string& fen, const std::string& lastMove, const std::string& whiteTime, const std::string& blackTime) {
    this->sideToMove = sideToMove;
    this->check = check;
    this->moves = moves;
    this->evaluation = evaluation;
    this->fen = fen;
    this->lastMove = lastMove;
    this->whiteTime = whiteTime;
    this->blackTime = blackTime;
}

void Gui::resetGameInfo() {
    this->sideToMove = _WHITE;
    this->check = false;
    this->moves = 0;
    this->evaluation = 0.0;
    this->fen = "";
    this->lastMove = "";
    this->whiteTime = "00:00:00";
    this->blackTime = "00:00:00";
}

Piece Gui::choosePromotionType() {
    return promotion;
}
