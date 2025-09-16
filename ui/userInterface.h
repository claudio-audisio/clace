#pragma once

#include <algorithm>

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
		cin >> value;
		return value;
	}

	static string readString() {
		string value;
		cin >> value;
		return value;
	}

	static string readLine() {
		string line;
		getline(cin, line);
		return line;
	}

	static void menu() {
		cout << endl;
		cout << " new             start new game" << endl;
		cout << " next            start analysis for next move" << endl;
		cout << " load <fen>      load fen position" << endl;
		cout << " cpu <n>         start n cpu game(s)" << endl;
		cout << " perft <d> <n>   compute perft at depth d (n times)" << endl;
		cout << " perfc <d> <n>   compute perft complete at depth d (n times)" << endl;
		cout << " print <b>       print bitboards" << endl;
		cout << " help            show commands" << endl;
		cout << " quit            exit program" << endl;
		cout << endl;
	}

	/*static string readName(const string& color) {
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
	}*/

	static void printLogo() {
		cout << "        |" << endl;
		cout << "   __|  |   _` |   __|   _ \\ " << endl;
		cout << "  (     |  (   |  (      __/ " << endl;
		cout << " \\___| _| \\__,_| \\___| \\___| " << endl;
		cout << " by Claudio Audisio" << endl;
	}

	static void printMessage(const string& message) {
		cout << endl << " " << message << endl << endl;
	}

    static void addLines(unsigned int lines) {
        while (lines-- > 0) {
            cout << endl;
        }
    }

	static void printBoard(const Board& board) {
		cout << endl;
		int row = 0;

		while (row <= 56) {
			cout << " ";
			for (int i = 0; i < 8; i++) {
				cout << getPieceCode(board.getPiece(row + i)) << " ";
			}
			cout << endl;

			row += 8;
		}
	}

	static void printGame(Game& game) {
		cout << endl;
		int row = 0;

		while (row <= 56) {
			cout << " ";
			for (int i = 0; i < 8; i++) {
				cout << getPieceCode(game.board.getPiece(row + i)) << " ";
			}

			switch (row) {
				case 0: {
						if (game.checkStatus.check && !game.checkStatus.checkmate) {
							cout << "\t\tcheck!";
						}
						break;
					}
				case 8: cout << "\t\tmoves: " << game.fullMoves; break;
				case 16: cout << "\t\tevaluation: " << fixed << setprecision(2) << game.evaluator->evaluate(game); break;
				case 24: cout << "\t\ttimes: " << game.whitePlayer->getMoveTime() << " - " << game.blackPlayer->getMoveTime(); break;
				case 32: {
						const string& whiteCaptured = game.getCapturedList(WHITE);
						const string& blackCaptured = game.getCapturedList(BLACK);
						if (!whiteCaptured.empty() || !blackCaptured.empty()) {
							cout << "\t\tcaptured: " << whiteCaptured << "- " << blackCaptured;
						}
						break;
					}
				case 40: cout << "\t\tfen: " << FEN::gameToFEN(game); break;
				case 48: {
						if (game.lastMove != 0) {
							cout << "\t\tlast move: " << toString(game.lastMove);
						}
						break;
					}
				default: break;
			}

			cout << endl;
			row += 8;
		}

		cout << endl;
	}

	static void printData(Game& game) {
		cout << endl;
		cout << " moves: " << game.fullMoves << endl;
		cout << " times: " << game.whitePlayer->getMoveTime() << " - " << game.blackPlayer->getMoveTime() << endl;

		const string whiteCaptured = game.getCapturedList(WHITE);
		const string blackCaptured = game.getCapturedList(BLACK);

		if (!whiteCaptured.empty() || !blackCaptured.empty()) {
			cout << " captured: " << whiteCaptured << "- " << blackCaptured << endl;
		}

		cout << " evaluation: " << fixed << setprecision(2) << game.evaluator->evaluate(game) << endl;

		if (game.lastMove != 0) {
			cout << " last move: " << toString(game.lastMove) << " (" << (isWhite(game.lastMove) ? "white" : "black") << ")" << endl;
		}

		cout << " fen: " << FEN::gameToFEN(game) << endl;
		cout << endl;
	}

	static void printFENGame(Game game) {
		cout << "  " << FEN::gameToFEN(game) << endl;
	}

	/*
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
	*/

	static Piece choosePromotionType(bool white) {
		while(true) {
			cout << " promotion, choose piece: ";

			string piece = readString();
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

	static void printBoard_old(const Board& board) {
		int row = 0;

		cout << "        a   b   c   d   e   f   g   h      " << endl;
		cout << "      ╔═══╤═══╤═══╤═══╤═══╤═══╤═══╤═══╗    " << endl;

		while (row <= 56) {
			cout << "    " << ((64 - row) / 8) << " ║";

			for (int i = 0; i < 8; i++) {
				cout << " " << (char)getPieceCode(board.getPiece(row + i)) << " ";

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

};


