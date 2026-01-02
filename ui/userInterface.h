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

	static void printBoard(const Board *board) {
		cout << endl;
		int row = 0;

		while (row <= 56) {
			cout << " ";
			for (int i = 0; i < 8; i++) {
				cout << getPieceCode(getPiece(board, row + i)) << " ";
			}
			cout << endl;

			row += 8;
		}
	}

	static void printGame(Game& game) {
		cout << endl;
		int row = 7;

		while (row >= 0) {
			cout << " ";
			for (int i = 0; i < 8; i++) {
				cout << getPieceCode(getPiece(game.board, (row * 8) + i)) << " ";
			}

			switch (row) {
				case 7: {
						if (game.checkStatus.check && !game.checkStatus.checkmate) {
							cout << "\t\tcheck!";
						}
						break;
					}
				case 6: cout << "\t\tmoves: " << game.fullMoves; break;
				case 5: cout << "\t\tevaluation: " << fixed << setprecision(2) << game.currentEvaluation; break;
				case 4: cout << "\t\ttimes: " << game.whitePlayer->getMoveTime() << " - " << game.blackPlayer->getMoveTime(); break;
				case 3: {
						const string& whiteCaptured = game.getCapturedList(_WHITE);
						const string& blackCaptured = game.getCapturedList(_BLACK);
						if (!whiteCaptured.empty() || !blackCaptured.empty()) {
							cout << "\t\tcaptured: " << whiteCaptured << "- " << blackCaptured;
						}
						break;
					}
				case 2: cout << "\t\tfen: " << FEN::gameToFEN(game); break;
				case 1: {
						if (game.lastMove != 0) {
							cout << "\t\tlast move: " << moveToString(game.lastMove);
						}
						break;
					}
				default: break;
			}

			cout << endl;
			row--;
		}

		cout << endl;
	}

	static void printData(Game& game) {
		cout << endl;
		cout << " moves: " << game.fullMoves << endl;
		cout << " times: " << game.whitePlayer->getMoveTime() << " - " << game.blackPlayer->getMoveTime() << endl;

		const string whiteCaptured = game.getCapturedList(_WHITE);
		const string blackCaptured = game.getCapturedList(_BLACK);

		if (!whiteCaptured.empty() || !blackCaptured.empty()) {
			cout << " captured: " << whiteCaptured << "- " << blackCaptured << endl;
		}

		cout << " evaluation: " << fixed << setprecision(2) << game.currentEvaluation << endl;

		if (game.lastMove != 0) {
			cout << " last move: " << moveToString(game.lastMove) << " (" << (isWhite(game.lastMove) ? "white" : "black") << ")" << endl;
		}

		cout << " fen: " << FEN::gameToFEN(game) << endl;
		cout << endl;
	}

	static void printFENGame(Game& game) {
		cout << "  " << FEN::gameToFEN(game) << endl;
	}

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

};


