#pragma once
#include <iostream>
#include <ostream>

using namespace std;


static string readName(const string& color) {
    if (!color.empty()) {
        cout << "  Enter " + color + " name: ";
    } else {
        cout << "  Enter name: ";
    }

    return readString();
}

static unsigned int readGamesQuantity() {
    cout << "Enter how many games: " << endl;
    return readInt();
}

static string readBoard() {
    cout << "Enter FEN board: " << endl;
    return readString();
}

static unsigned int readDepth() {
    cout << "Enter depth: " << endl;
    return readInt();
}

static unsigned int readPerftQuantity() {
    cout << "Enter how many runs: " << endl;
    return readInt();
}

static unsigned int readPerftType() {
    cout << "Please choose type of perft" << endl;
    cout << "\t1. complete" << endl;
    cout << "\t2. bulk" << endl;
    return readInt();
}

static unsigned int readPerftIndex() {
    cout << "Please choose which perft" << endl;
    cout << "\t1...6" << endl;
    return readInt();
}

static unsigned int menuGame() {
    cout << endl;
    cout << "Please choose type of game" << endl;
    cout << "\t1. computer vs computer" << endl;
    cout << "\t2. white user vs computer" << endl;
    cout << "\t3. black user vs computer" << endl;
    cout << "\t4. user vs user" << endl;
    cout << "\t5. computer vs computer (more games)" << endl;
    cout << "\t6. user vs computer (board provided)" << endl;
    cout << "\t0. <-- back" << endl;
    return readInt();
}

static Move getNextMove(Player* player) {
    while (true) {

        cout << "  " << player->getNameAndColor() << " to move: ";
        try {
            string line = UI::readString(false);
            return
        }
        catch (exception e) {
            cout << "  --> malformed move: please use this notation e2-e4" << endl;
        }
    }
}

static void printBoard_old(const Board *board) {
    int row = 0;

    cout << "        a   b   c   d   e   f   g   h      " << endl;
    cout << "      ╔═══╤═══╤═══╤═══╤═══╤═══╤═══╤═══╗    " << endl;

    while (row <= 56) {
        cout << "    " << ((64 - row) / 8) << " ║";

        for (int i = 0; i < 8; i++) {
            cout << " " << getPieceCode(getPiece(board, row + i)) << " ";

            if (i < 7) {
                cout << "│";
            }
        }
        cout << "║ " << ((64 - row) / 8) << "  " << endl;
        row += 8;

        if (row <= 56) {
            cout << "      ╟───┼───┼───┼───┼───┼───┼───┼───╢    " << endl;
        }
    }

    cout << "      ╚═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╝    " << endl;
    cout << "        a   b   c   d   e   f   g   h      " << endl << endl;
}