#pragma once

#include <vector>
#include <algorithm> 
#include <cctype>

#include "../clace.h"
#include "../game/game.h"
#include "../utils/castlingHelper.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;

class FEN {
public:

    static Game* fenToNewGame(const string& fenPosition) {
        Game* game = new Game();
        game->initPlayers();
        game->resetPlayersPieces();

        fenToGame(fenPosition, *game);

        return game;
    }

    static void fenToGame(const string& fenPosition, Game& game) {
        vector<string>* tokens = tokenize(fenPosition, SEPARATOR, 6);
        fenToChessBoard(tokens->at(0), game);
        game.setKingPositions();
        game.setSideToMove((tokens->at(1) == "w" || tokens->at(1) == "W") ? WHITE : BLACK);
        game.setCastlingInfo(fenToCastlingInfo(tokens->at(2)));
        game.setEnPassantPosition(fenToEnPassantPosition(tokens->at(3)));
        game.setHalfMoveClock(stoi(tokens->at(4)));
        game.setFullMoves(stoi(tokens->at(5)));
        delete tokens;
    }

    static string gameToFEN(const Game& game) {
        string fenBoard;
        fenBoard.append(chessBoardToFEN(game.getBoard()));
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(game.isWhiteToMove() ? "w" : "b");
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(castlingInfoToFEN(game.getCastlingInfo()));
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(enPassantToFEN(game.getEnPassantPosition()));
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(to_string(game.getHalfMoveClock()));
        fenBoard.append(string(1, SEPARATOR));
        fenBoard.append(to_string(game.getFullMoves()));
        return fenBoard;
    }

    // TODO scrivere test per FENKey
    static string gameToFENKey(const Game& game) {
        string fenKey;
        fenKey.append(chessBoardToFENKey(game.getBoard()));
        fenKey.append(game.isWhiteToMove() ? "w" : "b");
        fenKey.append(castlingInfoToFEN(game.getCastlingInfo()));
        fenKey.append(enPassantToFEN(game.getEnPassantPosition()));
        return fenKey;
    }

    static Game* mirrorGame(const Game& game) {
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
                    game.setEmptyPiece(start++);
                }
            }
            else {
                const Piece piece = FEN_TO_PIECE.at(c);
                game.setPiece(start++, piece);
                game.incrementPlayerPieces(piece);
            }
        }
		//game.update();
    }

static CastlingInfo fenToCastlingInfo(const string& fenCastlingInfo) {
        CastlingInfo castlingInfo = CastlingHelper::reset();

        if (strcmp(fenCastlingInfo.c_str(), "-") != 0) {
            CastlingHelper::setBlackKing(castlingInfo, fenCastlingInfo.find_first_of('k') != string::npos);
            CastlingHelper::setBlackQueen(castlingInfo, fenCastlingInfo.find_first_of('q') != string::npos);
            CastlingHelper::setWhiteKing(castlingInfo, fenCastlingInfo.find_first_of('K') != string::npos);
            CastlingHelper::setWhiteQueen(castlingInfo, fenCastlingInfo.find_first_of('Q') != string::npos);
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
                    fenKey.append(EMPTY_FEN[empty]);
                    empty = 0;
                }
                fenKey.append(PIECE_TO_FEN[piece]);
            }

            if ((i + 1) % 8 == 0) {
                if (empty > 0) {
                    fenKey.append(EMPTY_FEN[empty]);
                    empty = 0;
                }
            }
        }

        return fenKey;
    }

public:
    static string castlingInfoToFEN(const CastlingInfo castlingInfo) {
        if (!castlingInfo) {
            return "-";
        }

        string fen;
        if (CastlingHelper::isWhiteKingCastling(castlingInfo)) {
            fen.append("K");
        }

        if (CastlingHelper::isWhiteQueenCastling(castlingInfo)) {
            fen.append("Q");
        }

        if (CastlingHelper::isBlackKingCastling(castlingInfo)) {
            fen.append("k");
        }

        if (CastlingHelper::isBlackQueenCastling(castlingInfo)) {
            fen.append("q");
        }

        return fen;
    }

private:
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
            // TODO tirare eccezione
        }

        return result;
    }

};
