#pragma once

#include <algorithm>

#include "../clace.h"
#include "../board/piece.h"
#include "../board/board.h"
#include "../game/game.h"
#include "../utils/fen.h"
#include "../utils/pieceHelper.h"
#include "../move/move.h"
#include "../game/player.h"

using namespace std;

class UI {
public:

	static void clearScreen() {
		cout << "\033[2J\033[1;1H";
	}

	static unsigned int readInt() {
		unsigned int value;
		cout << "> ";
		cin >> value;

		return value;
	}

	/*static string readString() {
		string value;
		cout << "> ";
		getline(cin, value);
		return value;
	}*/

	static string readString(bool withPrompt = true) {
		string value;
		if (withPrompt) {
			cout << "> ";
		}
		cin >> value;
		return value;
	}

	static unsigned int menu() {
		cout << endl;
		cout << "\t1. game" << endl;
		cout << "\t2. perft" << endl;
		cout << "\t3. print boards" << endl;
		cout << "\t0. <-- exit" << endl;
		return readInt();
	}

	static string readName(const string& color) {
		if (!color.empty()) {
			cout << "  Enter " + color + " name: ";
		} else {
			cout << "  Enter name: ";
		}

		return readString(false);
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

	static void printLogo() {
		cout << "        |" << endl;
		cout << "   __|  |   _` |   __|   _ \\ " << endl;
		cout << "  (     |  (   |  (      __/ " << endl;
		cout << " \\___| _| \\__,_| \\___| \\___| " << endl;
	}

    static void addLines(unsigned int lines) {
        while (lines-- > 0) {
            cout << endl;
        }
    }

	static void printBoard(const Board& board) {
		int row = 0;

		cout << "        a   b   c   d   e   f   g   h      " << endl;
		cout << "      ╔═══╤═══╤═══╤═══╤═══╤═══╤═══╤═══╗    " << endl;

		while (row <= 56) {
			cout << "    " << ((64 - row) / 8) << " ║";

			for (int i = 0; i < 8; i++) {
				cout << " " << (char)PieceHelper::getPieceCode(board.getPiece(row + i)) << " ";

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

	static void printData(Game& game) {
		cout << endl;
		cout << "  moves: " << game.fullMoves << endl;
		cout << "  times: " << game.whitePlayer->getMoveTime() << " - " << game.blackPlayer->getMoveTime() << endl;

		const string whiteCaptured = game.getCapturedList(WHITE);
		const string blackCaptured = game.getCapturedList(BLACK);

		if (!whiteCaptured.empty() || !blackCaptured.empty()) {
			cout << "  captured: " << whiteCaptured << "- " << blackCaptured << endl;
		}

		// TODO
		//cout << "  evaluation: " << String.format("%.2f", game.evaluateAsWhite())<< endl;

		if (game.lastMove != 0) {
			cout << "  last move: " << MoveHelper::toString(game.lastMove) << " (" << (MoveHelper::isWhite(game.lastMove) ? game.whitePlayer->name : game.blackPlayer->name) << ")" << endl;
		}

		cout << "  fen: " << FEN::gameToFEN(game) << endl;
		cout << endl;
	}

	static void printFENGame(Game game) {
		cout << "  " << FEN::gameToFEN(game) << endl;
	}

	static Move getNextMove(Player* player) {
		while (true) {
			cout << "  " << player->getNameAndColor() << " to move: ";
			try {
				string line = UI::readString(false);
				return MoveHelper::getMove(line, player->white);
			}
			catch (exception e) {
				cout << "  --> malformed move: please use this notation e2-e4" << endl;
			}
		}
	}

	static Piece choosePromotionType(bool white) {
		while(true) {
			cout << "Pawn promotion, choose piece type: ";

			string piece = readString(false);
			transform(piece.begin(), piece.end(), piece.begin(), ::toupper);

			if (piece.length() != 1) {
				continue;
			}

			switch (piece.at(0)) {
				case 'P': return white ? WPawn : BPawn;
				case 'R': return white ? WRook : BRook;
				case 'N': return white ? WKnight : BKnight;
				case 'B': return white ? WBishop : BBishop;
				case 'Q': return white ? WQueen : BQueen;
				default: break;
			}

			cout << "Use piece's initial char" << endl;
		}
	}

};


