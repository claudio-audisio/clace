#include "gui.h"
#include "guiConstants.h"
#include "../../game/gameRunner.h"
#include "../../clace.h"
#include "utils.h"

Gui::Gui() {
}

void Gui::init() {
    newGame(INITIAL_FEN_POSITION);
    resetGameInfo();
    InitWindow(WINDOW_W, WINDOW_H, TITLE);
    SetTargetFPS(FPS);
    loadFonts();
    loadTextures();
    loadButtons();
    loadInfo();
    showFenPanel = false;
    showPromotionPanel = false;
}


void Gui::run() {
    init();

    while (!WindowShouldClose())
    {
        checkButtonPressed();

        if (endGame == NONE) {
            dragAndDropPiece();
        }

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

void Gui::drawPieces() {
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

            if (i == computerCellDestination) {
                computerMovePosition.x += computerMoveDelta.x;
                computerMovePosition.y += computerMoveDelta.y;

                if (getCell(computerMovePosition) == i) {
                    computerCellDestination = -1;
                } else {
                    DrawTextureEx(pieces[piecePositions[i] - 1], computerMovePosition, 0, PIECE_SCALE, ColorAlpha(WHITE, PIECE_ALPHA));
                }
            } else {
                DrawTextureEx(pieces[piecePositions[i] - 1], piecePosition, 0, PIECE_SCALE, ColorAlpha(WHITE, PIECE_ALPHA));
            }
        }
    }
}

void Gui::drawInfoPanel() {
    const int lineSpacing = INFO_FONT_SIZE + 5;
    DrawTextEx(font, format("{} to move", sideToMove ? "black" : "white").c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing}, INFO_FONT_SIZE, 1, RAYWHITE);
    DrawTextEx(font, format("time {} - {}", whiteTime, blackTime).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 2}, INFO_FONT_SIZE, 1, RAYWHITE);
    DrawTextEx(font, format("{} moves", moves).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 3}, INFO_FONT_SIZE, 1, RAYWHITE);
    DrawTextEx(font, format("last move {}", lastMove).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 5}, INFO_FONT_SIZE, 1, RAYWHITE);
    DrawTextEx(font, format("evaluation {}", evalValueToString(evaluation)).c_str(), { INFO_START_X, INFO_START_Y + BUTTON_H + lineSpacing * 4}, INFO_FONT_SIZE, 1, RAYWHITE);

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
    newButton.draw2();
    fenButton.draw2();
    hintButton.draw2();
    noneButton.draw2();
}

void Gui::loadFonts() {
    font = LoadFontEx("../ui/gui/asset/NotoSans-SemiBold.ttf", 30, nullptr, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
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
    if (isComputerMove) {
        Position computerCellSource = getSourcePosition(gameRunner->game->lastMove);
        computerCellDestination = getDestinationPosition(gameRunner->game->lastMove);
        Vector2 startPos = getStartPos(computerCellSource);
        Vector2 endPos = getStartPos(computerCellDestination);
        const float dist = distance(startPos, endPos);
        const float time = dist / MOVE_SPEED;

        computerMoveDelta = {(endPos.x - startPos.x) / (FPS * time), (endPos.y - startPos.y) / (FPS * time)};
        computerMovePosition.x = startPos.x + 15;
        computerMovePosition.y = startPos.y + 15;

        /*cout << "source: " << computerCellSource << " --> " << computerMovePosition.x << "-" << computerMovePosition.y << endl;
        cout << "dest: " << computerCellDestination << " --> " << endPos.x << "-" << endPos.y << endl;
        cout << "delta: " << computerMoveDelta.x << "-" << computerMoveDelta.y << endl;*/
    }

    memcpy(piecePositions, gameRunner->game->board->piecePositions, sizeof(Piece) * 64);
}

void Gui::newGame(const string& fenGame) {
    panel.clean();
    memset(piecePositions, Empty, sizeof(piecePositions));
    statistics = new Statistics(1, true);
    gameRunner = new GameRunner(statistics, HvsC, fenGame);
    gameFuture = async(launch::async, [&]() {
        return gameRunner->run(this);
    });
    endGame = NONE;
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

            if (selection != -1 && isWhite(gameRunner->game->board, selection)) {
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
    panel.write(SEPARATION_LINE);
    piecePositions[cellDestination] = piece;
    piecePositions[cellSelected] = Empty;
    gameRunner->setHumanMove(indexToCoords(cellSelected)+indexToCoords(cellDestination));
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
        //panel.write(format("{}-{}", mousePos.x, mousePos.y));

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
            newGame(INITIAL_FEN_POSITION);
            return;
        }

        if (fenButton.isPressed(mousePos)) {
            showFenPanel = true;
            return;
        }

        if (hintButton.isPressed(mousePos)) {
            panel.write(SEPARATION_LINE);
            Clace::manageNextMove(gameRunner, this);
        }
    }
}

void Gui::showMessage(const string& message) {
    panel.write(message);
}

void Gui::setGameInfo(const Game& game) {
    sideToMove = game.sideToMove;
    check = game.checkStatus.check && !game.checkStatus.checkmate;
    moves = game.fullMoves;
    evaluation = game.currentEvaluation;
    fen = FEN::gameToFEN(game);
    lastMove = moveToString(game.lastMove);
    whiteTime = game.whitePlayer->getMoveTime();
    blackTime = game.blackPlayer->getMoveTime();
}

void Gui::resetGameInfo() {
    this->sideToMove = _WHITE;
    this->check = false;
    this->moves = 0;
    this->evaluation = NO_EVAL;
    this->fen = "";
    this->lastMove = "";
    this->whiteTime = "00:00:00";
    this->blackTime = "00:00:00";
}

Piece Gui::choosePromotionType() {
    return promotion;
}

void Gui::notifyEndGame(const EndGameType endGame) {
    this->endGame = endGame;

    if (endGame != NONE) {
        check = false;
    }
}
