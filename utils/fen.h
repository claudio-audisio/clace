#pragma once

#include <vector>
#include <algorithm> 
#include <cctype>

#include "../clace.h"
#include "../game/game.h"
#include "../utils/castlingInfoHelper.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;

class FEN {
public:

    static Game* fenToNewGame(const string& fenPosition) {
        Game* game = new Game();
        game->resetPlayersPieces();

        fenToGame(fenPosition, *game);

        return game;
    }

    static void fenToGame(const string& fenPosition, Game& game) {
        vector<string>* tokens = tokenize(fenPosition, SEPARATOR, 6);
        fenToChessBoard(tokens->at(0), game);
        game.setKingPositions();
        game.setWhiteToMove(tokens->at(1) == "w" || tokens->at(1) == "W");
        game.setCastlingInfo(fenToCastlingInfo(tokens->at(2)));
        game.setEnPassantPosition(fenToEnPassantPosition(tokens->at(3)));
        game.setHalfMoveClock(stoi(tokens->at(4)));
        game.setFullMoves(stoi(tokens->at(5)));
        delete tokens;
    }

    static string gameToFEN(Game& game) {
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

    static Game* mirrorGame(Game& game) {
        return fenToNewGame(mirrorFenGame(gameToFEN(game)));
    }

    static string mirrorFenGame(string& fenGame) {
        vector<string>* tokens = tokenize(fenGame, SEPARATOR, 6);
        const string positions = mirrorPositions(tokens->at(0));
        const string whoMoves = mirrorWhoMoves(tokens->at(1));
        const string castlingInfo = mirrorCastlingInfo(tokens->at(2));
        // TODO mirror en passant
        const string mirror = positions + SEPARATOR + whoMoves + SEPARATOR + castlingInfo + SEPARATOR + tokens->at(3) + SEPARATOR + tokens->at(4) + SEPARATOR + tokens->at(5);
        delete tokens;
        return mirror;
    }

    static bool isWhiteToMove(string& fenPosition) {
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
                const Piece piece = parseFENPiece(c);
                game.setPiece(start++, piece);
                game.incrementPlayerPieces(piece);
            }
        }
    }

    static Piece parseFENPiece(const char fenPiece) {
        switch (fenPiece) {
            case 'K': return WKing;
            case 'Q': return WQueen;
            case 'R': return WRook;
            case 'N': return WKnight;
            case 'B': return WBishop;
            case 'P': return WPawn;
            case 'k': return BKing;
            case 'q': return BQueen;
            case 'r': return BRook;
            case 'n': return BKnight;
            case 'b': return BBishop;
            case 'p': return BPawn;
            default: break;
        }

        return Empty;
    }

    static CastlingInfo fenToCastlingInfo(const string& fenCastlingInfo) {
        CastlingInfo castlingInfo = CastlingInfoHelper::reset();

        if (strcmp(fenCastlingInfo.c_str(), "-") != 0) {
            castlingInfo = CastlingInfoHelper::setBlackKing(castlingInfo, fenCastlingInfo.find_first_of('k') != string::npos);
            castlingInfo = CastlingInfoHelper::setBlackQueen(castlingInfo, fenCastlingInfo.find_first_of('q') != string::npos);
            castlingInfo = CastlingInfoHelper::setWhiteKing(castlingInfo, fenCastlingInfo.find_first_of('K') != string::npos);
            castlingInfo = CastlingInfoHelper::setWhiteQueen(castlingInfo, fenCastlingInfo.find_first_of('Q') != string::npos);
        }

        return castlingInfo;
    }

    static Position fenToEnPassantPosition(const string& fenEnPassantPosition) {
        if (strcmp(fenEnPassantPosition.c_str(), "-") == 0) {
            return NO_POS;
        }

        return Positions::coordsToIndex(fenEnPassantPosition);
    }

    static string chessBoardToFEN(Board& board) {
        string fenBoard;
        int empty = 0;

        for (int i = 0; i < 64; i++) {
            const Piece piece = board.getPiece(i);

            if (PieceHelper::isEmpty(piece)) {
                empty++;
            }
            else {
                if (empty > 0) {
                    fenBoard.append(to_string(empty));
                    empty = 0;
                }
                fenBoard.append(pieceToFEN(piece));
            }

            if ((i + 1) % 8 == 0) {
                if (empty > 0) {
                    fenBoard.append(to_string(empty));
                    empty = 0;
                }

                if (i < 64 - 1) {
                    fenBoard.append("/");
                }
            }
        }

        return fenBoard;
    }

    static string pieceToFEN(Piece piece) {
        switch (piece) {
            case WKing: return "K";
            case BKing: return "k";
            case WQueen: return "Q";
            case BQueen: return "q";
            case WRook: return "R";
            case BRook: return "r";
            case WKnight: return "N";
            case BKnight: return "n";
            case WBishop: return "B";
            case BBishop: return "b";
            case WPawn: return "P";
            case BPawn: return "p";
            default: break;
        }

        // TODO tirare eccezione
        //throw new RuntimeException("error converting an empty piece to FEN");
        return "";
    }

    static string castlingInfoToFEN(CastlingInfo castlingInfo) {
        string fen = "";

        if (CastlingInfoHelper::isWhiteKingCastling(castlingInfo)) {
            fen += "K";
        }

        if (CastlingInfoHelper::isWhiteQueenCastling(castlingInfo)) {
            fen += "Q";
        }

        if (CastlingInfoHelper::isBlackKingCastling(castlingInfo)) {
            fen += "k";
        }

        if (CastlingInfoHelper::isBlackQueenCastling(castlingInfo)) {
            fen += "q";
        }

        if (fen.empty()) {
            fen = "-";
        }

        return fen;
    }

    static string enPassantToFEN(Position position) {
        if (position == NO_POS) {
            return "-";
        }

        return Positions::indexToCoords(position);
    }

    static string mirrorPositions(string fenChessBoard) {
        string positions = "";

        for (int i = 0; i < fenChessBoard.length(); ++i) {
            positions.append(string(1, mirrorPosition(fenChessBoard.at(i))));
        }

        reverse(positions.begin(), positions.end());
        return positions;
    }

    static char mirrorPosition(char c) {
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

    static string mirrorWhoMoves(string& whoMoves) {
        if (whoMoves == "w" || whoMoves == "W") {
            return "b";
        }
        return "w";
    }

    static string mirrorCastlingInfo(string& castlingInfo) {
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
