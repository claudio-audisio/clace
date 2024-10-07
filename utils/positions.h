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
	inline static const string PERFT_FEN_POSITION_4_MIRRORED = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";
    inline static const string PERFT_FEN_POSITION_5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    inline static const string PERFT_FEN_POSITION_6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

    static void calculateCheckPositions(Game& game, Side side) {
		Rawboard positions = game.getBoard().BOARD(side);

		while(positions) {
			const Position position = FIRST_POS(positions);
			const Piece piece = game.getPiece(position);
			const Rawboard oppositeBoard = game.getBoard().OPPOSITE(side);

			if (PieceHelper::isPawn(piece)) {
				const Rawboard pawnAttacks = game.getBoard().getPawnAttacks(position, side);
				game.getCheckStatus().updateAllCheckPositions(pawnAttacks);
				game.getCheckStatus().addCheckPosition(position, pawnAttacks);
			} else if (PieceHelper::isRook(piece)) {
				const Rawboard rookAttacks = game.getBoard().getRookMoves(position, oppositeBoard);
				game.getCheckStatus().updateAllCheckPositions(rookAttacks);
				game.getCheckStatus().addCheckPosition(position, rookAttacks);
				game.getCheckStatus().addXRayPosition(position, rookAttacks);
			} else if (PieceHelper::isKnight(piece)) {
				const Rawboard knightAttacks = game.getBoard().getKnightMoves(position, side);
				game.getCheckStatus().updateAllCheckPositions(knightAttacks);
				game.getCheckStatus().addCheckPosition(position, knightAttacks);
			} else if (PieceHelper::isBishop(piece)) {
				const Rawboard bishopAttacks = game.getBoard().getBishopMoves(position, oppositeBoard);
				game.getCheckStatus().updateAllCheckPositions(bishopAttacks);
				game.getCheckStatus().addCheckPosition(position, bishopAttacks);
				game.getCheckStatus().addXRayPosition(position, bishopAttacks);
			} else if (PieceHelper::isQueen(piece)) {
				const Rawboard queenAttacks = game.getBoard().getQueenMoves(position, oppositeBoard);
				game.getCheckStatus().updateAllCheckPositions(queenAttacks);
				game.getCheckStatus().addCheckPosition(position, queenAttacks);
				game.getCheckStatus().addXRayPosition(position, queenAttacks);
			} else if (PieceHelper::isKing(piece)) {
				const Rawboard kingAttacks = game.getBoard().getKingMoves(position, side, game.getCastlingInfo());
				game.getCheckStatus().updateAllCheckPositions(kingAttacks);
				game.getCheckStatus().addCheckPosition(position, kingAttacks);
			}

			positions &= (positions - 1);
		}
    }

	// TODO da fare refactoring
    static unsigned int getAllDestinationQty(const Game& game, const bool white) {
        /*unsigned int count = 0;

        for (Position i = 0; i < 64; i++) {
            if (game.checkColor(i, white)) {
                const Rawboard destinations = getDestinationPositions(game, i);
                count += BoardUtils::positionsCount(destinations);
            }
        }

        return count;*/
		return 0;
    }

    static Rawboard getDestinationPositions(const Game& game, const Position position, const Rawboard opposite) {
        return getDestinationPositions(game, position, game.getPiece(position), opposite);
    }
    
    static Rawboard getDestinationPositions(const Game& game, const Position position, const Piece piece, const Rawboard opposite) {
        switch (piece) {
            case WPawn: return game.getBoard().getPawnMoves(position, WHITE, game.getEnPassantPosition());
            case BPawn: return game.getBoard().getPawnMoves(position, BLACK, game.getEnPassantPosition());
            case WRook: return game.getBoard().getRookMoves(position, opposite);
            case BRook: return game.getBoard().getRookMoves(position, opposite);
            case WKnight: return game.getBoard().getKnightMoves(position, WHITE);
            case BKnight: return game.getBoard().getKnightMoves(position, BLACK);
            case WBishop: return game.getBoard().getBishopMoves(position, opposite);
            case BBishop: return game.getBoard().getBishopMoves(position, opposite);
            case WQueen: return game.getBoard().getQueenMoves(position, opposite);
            case BQueen: return game.getBoard().getQueenMoves(position, opposite);
            case WKing: return game.getBoard().getKingMoves(position, WHITE, game.getCastlingInfo());
            case BKing: return game.getBoard().getKingMoves(position, BLACK, game.getCastlingInfo());
            default: return 0;
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
        unordered_set<Position>* piecePositions = getPiecePositions(game, PieceHelper::getXRayTypes(game.getOppositeSide()));
        piecePositions->erase(excludePosition);

        Rawboard xRayPositions = 0;
		const Rawboard opposite = game.getBoard().OPPOSITE(game.getOppositeSide());

        for (Position position : *piecePositions) {
            xRayPositions |= getDestinationPositions(game, position, opposite);
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

