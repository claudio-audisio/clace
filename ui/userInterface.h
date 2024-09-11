#pragma once

#include <conio.h>

#include "../clace.h"

#include "../board/piece.h"

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

	static string readString() {
		string value;
		cout << "> ";
		getline(cin, value);
		return value;
	}

	static unsigned int menu() {
		cout << endl;
		cout << "Please choose type of game" << endl;
		cout << "\t1. game" << endl;
		cout << "\t2. perft" << endl;
		cout << "\t0. <-- exit" << endl;
		return readInt();
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

	static void waitInteraction() {
		getch();
	}

	static void printLogo() {
		cout << "        |" << endl;
		cout << "   __|  |   _` |   __|   _ \\ " << endl;
		cout << "  (     |  (   |  (      __/ " << endl;
		cout << " \\___| _| \\__,_| \\___| \\___| " << endl;
	}

	static char getPieceCode(Piece piece) {
		switch (piece) {
		case WPawn: return 0x0050;
		case BPawn: return 0x0070;
		case WRook: return 0x0052;
		case BRook: return 0x0072;
		case WKnight: return 0x004E;
		case BKnight: return 0x006E;
		case WBishop: return 0x0042;
		case BBishop: return 0x0062;
		case WQueen: return 0x0051;
		case BQueen: return 0x0071;
		case WKing: return 0x004B;
		case BKing: return 0x006B;
		default: return 0x0020;
		}
	}

	/*
	public static void printBoard(Board board) {
		printWriter.println();
		int row = 0;

		System.out.println("        a   b   c   d   e   f   g   h      ");
		System.out.println("      ╔═══╤═══╤═══╤═══╤═══╤═══╤═══╤═══╗    ");

		while (row <= 56) {
			System.out.print("    " + ((64 - row) / 8) + " ║");

			for (int i = 0; i < 8; i++) {
				System.out.print(" " + (char)getPieceCode(board.getPieceType(row + i)) + " ");

				if (i < 7) {
					System.out.print("│");
				}
			}
			System.out.println("║ " + ((64 - row) / 8) + "  ");
			row += 8;

			if (row <= 56) {
				System.out.println("      ╟───┼───┼───┼───┼───┼───┼───┼───╢    ");
			}
		}

		System.out.println("      ╚═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╝    ");
		System.out.println("        a   b   c   d   e   f   g   h      ");
		printWriter.println();
	}
	*/

	/*
	public static void printData(Board board) {
		printWriter.println();
		printWriter.println("  moves: " + board.getFullMoves());
		printWriter.println("  times: " + board.getWhitePlayer().getMoveTime() + " - " + board.getBlackPlayer().getMoveTime());

		if (!board.getWhitePlayer().getCapturedList().isEmpty() || !board.getBlackPlayer().getCapturedList().isEmpty()) {
			printWriter.println("  captured: " + board.getWhitePlayer().getCapturedList() + "- " + board.getBlackPlayer().getCapturedList());
		}

		printWriter.println("  evaluation: " + String.format("%.2f", board.evaluateAsWhite()));

		if (board.getLastMove() != null) {
			printWriter.println("  last move: " + board.getLastMove() + " (" + (board.getLastMove().isWhite() ? board.getWhitePlayer().getName() : board.getBlackPlayer().getName()) + ")");
		}

		printWriter.println("  fen: " + FENConverter.boardToFEN(board));

		System.out.println();
	}

	public static void printFENBoard(Board board) {
		printWriter.println("  " + FENConverter.boardToFEN(board));
	}
	*/

};


