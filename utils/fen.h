#pragma once

#include <vector>
#include <algorithm> 
#include <cctype>

#include "../game/game.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;

class FEN {
public:

    static Game* fenToNewGame(const string& fenPosition) {
        Game* game = new Game();
        fenToGame(fenPosition, *game);

        return game;
    }

    static void fenToGame(const string& fenPosition, Game& game) {
        vector<string>* tokens = tokenize(fenPosition, SEPARATOR, 6);
        fenToChessBoard(tokens->at(0), game);
        game.sideToMove = (tokens->at(1) == "w" || tokens->at(1) == "W") ? WHITE : BLACK;
        game.board.castlingInfo = fenToCastlingInfo(tokens->at(2));
        game.board.enPassantPosition =fenToEnPassantPosition(tokens->at(3));
        game.halfMoveClock = stoi(tokens->at(4));
        game.fullMoves = stoi(tokens->at(5));
        delete tokens;
    }

    static string gameToFEN(Game& game) {
        string fenBoard;
        fenBoard.append(chessBoardToFEN(game.board));
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(game.isWhiteToMove() ? "w" : "b");
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(castlingInfoToFEN(game.board.castlingInfo));
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(enPassantToFEN(game.board.enPassantPosition));
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(to_string(game.halfMoveClock));
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(to_string(game.fullMoves));
        return fenBoard;
    }

    static string gameToFENKey(Game& game) {
        string fenKey;
        fenKey.append(chessBoardToFENKey(game.board));
        fenKey.append(game.isWhiteToMove() ? "w" : "b");
        fenKey.append(castlingInfoToFEN(game.board.castlingInfo));
        fenKey.append(enPassantToFEN(game.board.enPassantPosition));
        return fenKey;
    }

    static Game* mirrorGame(Game& game) {
        return fenToNewGame(mirrorFenGame(gameToFEN(game)));
    }

    static string mirrorFenGame(const string& fenGame) {
        vector<string>* tokens = tokenize(fenGame, SEPARATOR, 6);
        const string positions = mirrorPositions(tokens->at(0));
        const string whoMoves = mirrorWhoMoves(tokens->at(1));
        const string castlingInfo = mirrorCastlingInfo(tokens->at(2));
        // TODO mirror en passant
        const string mirror = positions + SEPARATOR + whoMoves + SEPARATOR + castlingInfo + SEPARATOR + tokens->at(3) + SEPARATOR + tokens->at(4) + SEPARATOR + tokens->at(5);
        delete tokens;
        return mirror;
    }

    static bool isWhiteToMove(const string& fenPosition) {
        vector<string> *tokens = tokenize(fenPosition, SEPARATOR, 6);
        const bool res = tokens->at(1) == "w" || tokens->at(1) == "W";
        delete tokens;
        return res;
    }

private:
    static const char SEPARATOR = ' ';

    static void fenToChessBoard(const string& fenChessBoard, Game& game) {
        vector<string>* tokens = tokenize(fenChessBoard, '/', 8);

        for (int i = 0; i < 8; i++) {
            populateChessBoard(game, i * 8, tokens->at(i));
        }

        delete tokens;
    }

    static void populateChessBoard(Game& game, int start, const string& fenChessBoard) {
        for (int i = 0; i < fenChessBoard.length(); i++) {
            const char c = fenChessBoard.at(i);

            if (c > 48 && c < 58) {
                for (int j = 0; j < (c - 48); j++) {
                    game.board.setEmpty(start++);
                }
            }
            else {
                const Piece piece = FEN_TO_PIECE.at(c);
                game.board.setPiece(start++, piece);
            }
        }
    }

    static CastlingInfo fenToCastlingInfo(const string& fenCastlingInfo) {
        CastlingInfo castlingInfo = 0;

        if (strcmp(fenCastlingInfo.c_str(), "-") != 0) {
            setBKCastling(castlingInfo, fenCastlingInfo.find_first_of('k') != string::npos);
            setBQCastling(castlingInfo, fenCastlingInfo.find_first_of('q') != string::npos);
            setWKCastling(castlingInfo, fenCastlingInfo.find_first_of('K') != string::npos);
            setWQCastling(castlingInfo, fenCastlingInfo.find_first_of('Q') != string::npos);
        }

        return castlingInfo;
    }

    static Position fenToEnPassantPosition(const string& fenEnPassantPosition) {
        if (strcmp(fenEnPassantPosition.c_str(), "-") == 0) {
            return NO_POS;
        }

        return Positions::coordsToIndex(fenEnPassantPosition);
    }

    static string chessBoardToFEN(const Board& board) {
        string fenBoard;
        int empty = 0;

        for (int i = 0; i < 64; i++) {
            const Piece piece = board.getPiece(i);

            if (PieceHelper::isEmpty(piece)) {
                empty++;
            }
            else {
                if (empty > 0) {
                    fenBoard.append(EMPTY_FEN[empty]);
                    empty = 0;
                }
                fenBoard.append(PIECE_TO_FEN[piece]);
            }

            if ((i + 1) % 8 == 0) {
                if (empty > 0) {
                    fenBoard.append(EMPTY_FEN[empty]);
                    empty = 0;
                }

                if (i < 64 - 1) {
                    fenBoard.append("/");
                }
            }
        }

        return fenBoard;
    }

    static string chessBoardToFENKey(const Board& board) {
        string fenKey;
        int empty = 0;

        for (int i = 0; i < 64; i++) {
            const Piece piece = board.getPiece(i);

            if (PieceHelper::isEmpty(piece)) {
                empty++;
            }
            else {
                if (empty > 0) {
                    fenKey.append(to_string(empty));
                    empty = 0;
                }
                fenKey.append(PIECE_TO_FEN[piece]);
            }
        }

        if (empty > 0) {
            fenKey.append(to_string(empty));
            empty = 0;
        }

        return fenKey;
    }

public:
    static string castlingInfoToFEN(const CastlingInfo castlingInfo) {
        if (!castlingInfo) {
            return "-";
        }

        string fen;
        if (isWhiteKingCastling(castlingInfo)) {
            fen.append("K");
        }

        if (isWhiteQueenCastling(castlingInfo)) {
            fen.append("Q");
        }

        if (isBlackKingCastling(castlingInfo)) {
            fen.append("k");
        }

        if (isBlackQueenCastling(castlingInfo)) {
            fen.append("q");
        }

        return fen;
    }

    static string enPassantToFEN(const Position position) {
        if (position == NO_POS) {
            return "-";
        }

        return Positions::indexToCoords(position);
    }

    static string mirrorPositions(const string& fenChessBoard) {
        string positions = "";

        for (int i = 0; i < fenChessBoard.length(); ++i) {
            positions.append(string(1, mirrorPosition(fenChessBoard.at(i))));
        }

        reverse(positions.begin(), positions.end());
        return positions;
    }

    static char mirrorPosition(const char c) {
        if (isalpha(c)) {
            if (isupper(c)) {
                return tolower(c);
            }
            else {
                return toupper(c);
            }
        }

        return c;
    }

    static string mirrorWhoMoves(const string& whoMoves) {
        if (whoMoves == "w" || whoMoves == "W") {
            return "b";
        }
        return "w";
    }

    static string mirrorCastlingInfo(const string& castlingInfo) {
        if (castlingInfo != "-") {
            string newCastlingInfo;

            for (int i = 0; i < castlingInfo.length(); ++i) {
                newCastlingInfo.append(string(1, mirrorPosition(castlingInfo.at(i))));
            }

            return newCastlingInfo;
        }

        return castlingInfo;
    }

    /*************************************************************/

    static vector<string>* tokenize(const string& value, const char separator, const int expectedSize) {
        istringstream iss(value);
        string token;

        vector<string> *result = new vector<string>;

        while (getline(iss, token, separator)) {
            result->push_back(token);
        }

        if (result->size() != expectedSize) {
			throw runtime_error("error tokenizing fen");
		}

        return result;
    }

	static void setBKCastling(CastlingInfo& castlingInfo, const bool info) {
		if (info) {
			castlingInfo |= 0b0010;
		}
		else {
			castlingInfo &= 0b1101;
		}
	}

	static void setBQCastling(CastlingInfo& castlingInfo, const bool info) {
		if (info) {
			castlingInfo |= 0b0001;
		}
		else {
			castlingInfo &= 0b1110;
		}
	}

	static void setWKCastling(CastlingInfo& castlingInfo, const bool info) {
		if (info) {
			castlingInfo |= 0b1000;
		}
		else {
			castlingInfo &= 0b0111;
		}
	}

	static void setWQCastling(CastlingInfo& castlingInfo, const bool info) {
		if (info) {
			castlingInfo |= 0b0100;
		}
		else {
			castlingInfo &= 0b1011;
		}
	}

	static bool isWhiteKingCastling(const CastlingInfo castlingInfo) {
		return (castlingInfo & 0b1000) > 0;
	}

	static bool isWhiteQueenCastling(const CastlingInfo castlingInfo) {
		return (castlingInfo & 0b0100) > 0;
	}

	static bool isBlackKingCastling(const CastlingInfo castlingInfo) {
		return (castlingInfo & 0b0010) > 0;
	}

	static bool isBlackQueenCastling(const CastlingInfo castlingInfo) {
		return (castlingInfo & 0b0001) > 0;
	}

};
