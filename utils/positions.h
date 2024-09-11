#pragma once

#include <unordered_set>

#include "../clace.h"
#include "../utils/boardUtils.h"
#include "../game/game.h"
#include "../utils/pieceHelper.h"

using namespace std;

class Positions {
public:

    inline static const int NULL_PIECE_POSITION = -1;
    inline static const string INITIAL_FEN_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    inline static const string INITIAL_FEN_POSITION_SHORT = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    inline static const string CASTLING_FEN_POSITION = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";
    inline static const string PERFT_FEN_POSITION_2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    inline static const string PERFT_FEN_POSITION_3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    inline static const string PERFT_FEN_POSITION_4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    inline static const string PERFT_FEN_POSITION_5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    inline static const string PERFT_FEN_POSITION_6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

    static void calculateCheckPositions(Game& game, bool white) {
        for (Position pos = 0; pos < 64; pos++) {
            if (game.checkColor(pos, white)) {
                const Piece piece = game.getPiece(pos);

                switch (piece) {
                    case WPawn: {
                        const Rawboard pawnAttacks = game.getBoard().getPawnAttacks(pos, true);
                        game.getCheckStatus().updateAllCheckPositions(pawnAttacks);
                        game.getCheckStatus().addCheckPosition(pos, pawnAttacks);
                        break;
                    }
                    case BPawn: {
                        const Rawboard pawnAttacks = game.getBoard().getPawnAttacks(pos, false);
                        game.getCheckStatus().updateAllCheckPositions(pawnAttacks);
                        game.getCheckStatus().addCheckPosition(pos, pawnAttacks);
                        break;
                    }
                    case WRook: {
                        const Rawboard rookAttacks = game.getBoard().getRookMoves(pos, true);
                        game.getCheckStatus().updateAllCheckPositions(rookAttacks);
                        game.getCheckStatus().addCheckPosition(pos, rookAttacks);
                        game.getCheckStatus().addXRayPosition(pos, rookAttacks);
                        break;
                    }
                    case BRook: {
                        const Rawboard rookAttacks = game.getBoard().getRookMoves(pos, false);
                        game.getCheckStatus().updateAllCheckPositions(rookAttacks);
                        game.getCheckStatus().addCheckPosition(pos, rookAttacks);
                        game.getCheckStatus().addXRayPosition(pos, rookAttacks);
                        break;
                    }
                    case WKnight: {
                        const Rawboard knightAttacks = game.getBoard().getKnightMoves(pos, true);
                        game.getCheckStatus().updateAllCheckPositions(knightAttacks);
                        game.getCheckStatus().addCheckPosition(pos, knightAttacks);
                        break;
                    }
                    case BKnight: {
                        const Rawboard knightAttacks = game.getBoard().getKnightMoves(pos, false);
                        game.getCheckStatus().updateAllCheckPositions(knightAttacks);
                        game.getCheckStatus().addCheckPosition(pos, knightAttacks);
                        break;
                    }
                    case WBishop: {
                        const Rawboard bishopAttacks = game.getBoard().getBishopMoves(pos, true);
                        game.getCheckStatus().updateAllCheckPositions(bishopAttacks);
                        game.getCheckStatus().addCheckPosition(pos, bishopAttacks);
                        game.getCheckStatus().addXRayPosition(pos, bishopAttacks);
                        break;
                    }
                    case BBishop: {
                        const Rawboard bishopAttacks = game.getBoard().getBishopMoves(pos, false);
                        game.getCheckStatus().updateAllCheckPositions(bishopAttacks);
                        game.getCheckStatus().addCheckPosition(pos, bishopAttacks);
                        game.getCheckStatus().addXRayPosition(pos, bishopAttacks);
                        break;
                    }
                    case WQueen: {
                        const Rawboard queenAttacks = game.getBoard().getQueenMoves(pos, true);
                        game.getCheckStatus().updateAllCheckPositions(queenAttacks);
                        game.getCheckStatus().addCheckPosition(pos, queenAttacks);
                        game.getCheckStatus().addXRayPosition(pos, queenAttacks);
                        break;
                    }
                    case BQueen: {
                        const Rawboard queenAttacks = game.getBoard().getQueenMoves(pos, false);
                        game.getCheckStatus().updateAllCheckPositions(queenAttacks);
                        game.getCheckStatus().addCheckPosition(pos, queenAttacks);
                        game.getCheckStatus().addXRayPosition(pos, queenAttacks);
                        break;
                    }
                    case WKing: {
                        const Rawboard kingAttacks = game.getBoard().getKingMoves(pos, true, game.getCastlingInfo());
                        game.getCheckStatus().updateAllCheckPositions(kingAttacks);
                        game.getCheckStatus().addCheckPosition(pos, kingAttacks);
                        break;
                    }
                    case BKing: {
                        const Rawboard kingAttacks = game.getBoard().getKingMoves(pos, false, game.getCastlingInfo());
                        game.getCheckStatus().updateAllCheckPositions(kingAttacks);
                        game.getCheckStatus().addCheckPosition(pos, kingAttacks);
                        break;
                    }
                    default: break;
                }
            }
        }
    }
    
    static unsigned int getAllDestinationQty(const Game& game, const bool white) {
        unsigned int count = 0;

        for (Position i = 0; i < 64; i++) {
            if (game.checkColor(i, white)) {
                count += BoardUtils::positionsCount(getDestinationPositions(game, i)->getBoard());
            }
        }

        return count;
    }

    // TODO da trasformare in bitset
    class PieceAndDest {
    public:
        PieceAndDest(const Rawboard board, const Piece piece) {
            this->board = board;
            this->piece = piece;
        }

        Rawboard board;
        Piece piece;

        Rawboard getBoard() {
            return board;
        }

        Piece getPiece() {
            return piece;
        }
    };
    
    static PieceAndDest* getDestinationPositions(const Game& game, const Position position) {
        return getPositions(game, position);
    }
    
    static PieceAndDest* getPositions(const Game& game, const Position position) {
        const Piece piece = game.getPiece(position);

        switch (piece) {
            case WPawn: return new PieceAndDest(game.getBoard().getPawnMoves(position, true, game.getEnPassantPosition()), WPawn);
            case BPawn: return new PieceAndDest(game.getBoard().getPawnMoves(position, false, game.getEnPassantPosition()), BPawn);
            case WRook: return new PieceAndDest(game.getBoard().getRookMoves(position, true), WRook);
            case BRook: return new PieceAndDest(game.getBoard().getRookMoves(position, false), BRook);
            case WKnight: return new PieceAndDest(game.getBoard().getKnightMoves(position, true), WKnight);
            case BKnight: return new PieceAndDest(game.getBoard().getKnightMoves(position, false), BKnight);
            case WBishop: return new PieceAndDest(game.getBoard().getBishopMoves(position, true), WBishop);
            case BBishop: return new PieceAndDest(game.getBoard().getBishopMoves(position, false), BBishop);
            case WQueen: return new PieceAndDest(game.getBoard().getQueenMoves(position, true), WQueen);
            case BQueen: return new PieceAndDest(game.getBoard().getQueenMoves(position, false), BQueen);
            case WKing: return new PieceAndDest(game.getBoard().getKingMoves(position, true, game.getCastlingInfo()), WKing);
            case BKing: return new PieceAndDest(game.getBoard().getKingMoves(position, false, game.getCastlingInfo()), BKing);
            default: return new PieceAndDest(0, Empty);
        };
    }

    static unordered_set<Position>* getPiecePositions(const Game& game, const unordered_set<Piece>& pieces) {
        unordered_set<Position>* positions = new unordered_set<Position>();

        for (Position i = 0; i < 64; i++) {
            if (pieces.find(game.getPiece(i)) != pieces.end()) {
                positions->insert(i);
            }
        }

        return positions;
    }

    static Position coordsToIndex(const string& coordinates) {
        const Position sourceColumn = coordinates.at(0) - 97;
        const Position sourceRow = 7 - (coordinates.at(1) - 49);

        if (sourceColumn < 0 || sourceColumn > 7 || sourceRow < 0 || sourceRow > 7) {
            // TODO tirare eccezione
            // throw new RuntimeException("piece coordinates malformed");
        }

        return sourceRow * 8 + sourceColumn;
    }

    static string indexToCoords(const Position position) {
        string coords;
        const char sourceColumn = (char)((position % 8) + 97);
        const char sourceRow = (char)(7 - (position / 8) + 49);
        coords.append(1, sourceColumn);
        coords.append(1, sourceRow);
        return coords;
    }

    
    static bool isFirstRow(const Position position, const bool white) {
        return white ? getRow(position) == 7 : getRow(position) == 0;
    }

    static bool isSecondRow(const Position position, const bool white) {
        return white ? getRow(position) == 6 : getRow(position) == 1;
    }

    static bool isFourthRow(const Position position, const bool white) {
        return white ? getRow(position) == 4 : getRow(position) == 3;
    }

    static bool isFifthRow(const Position position, const bool white) {
        return white ? getRow(position) == 3 : getRow(position) == 4;
    }
    
    static bool isEighthRow(const Position position, const bool white) {
        return white ? getRow(position) == 0 : getRow(position) == 7;
    }
    
    static bool areOnSameRowOrColumn(const Position firstPosition, const Position secondPosition) {
        return getRow(firstPosition) == getRow(secondPosition) || getColumn(firstPosition) == getColumn(secondPosition);
    }

    static bool areOnSameDiagonal(const Position firstPosition, const Position secondPosition) {
        return abs(getRow(firstPosition) - getRow(secondPosition)) == abs(getColumn(firstPosition) - getColumn(secondPosition));
    }

    static bool isOnXRay(const Game& game, const Position sourcePosition, const Position excludePosition) {
        unordered_set<Position>* piecePositions = getPiecePositions(game, PieceHelper::getXRayTypes(!game.isWhiteToMove()));
        piecePositions->erase(excludePosition);

        Rawboard xRayPositions = 0;

        for (Position position : *piecePositions) {
            xRayPositions |= getPositions(game, position)->getBoard();
        }

        return BoardUtils::isUnderCheck(xRayPositions, sourcePosition);
    }
    
    static Position getRow(const Position position) {
        return position / 8;
    }
    
    static Position getColumn(const Position position) {
        return position % 8;
    }

};

