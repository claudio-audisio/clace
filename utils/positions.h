#pragma once

#include <unordered_set>

#include "../clace.h"
#include "../utils/boardUtils.h"
#include "../game/game.h"
#include "../utils/pieceHelper.h"

using namespace std;

class Positions {
public:

    static void calculateCheckPositions(Game& game, Side side) {
		Rawboard positions = game.board.PIECES(side);

		while(positions) {
			const Position position = Utils::getFirstPos(positions);
			const Piece piece = game.board.getPiece(position);

			if (PieceHelper::isPawn(piece)) {
				const Rawboard attacks = game.board.getPawnAttacks(position, side);
				game.checkStatus.updateAllCheckPositions(attacks);
				game.checkStatus.addCheckPosition(position, attacks);
			} else if (PieceHelper::isRook(piece)) {
				const Rawboard attacks = game.board.rookAttack(position, ~game.board.EMPTY, ~game.board.PIECES(side));
				game.checkStatus.updateAllCheckPositions(attacks);
				game.checkStatus.addCheckPosition(position, attacks);
				game.checkStatus.addXRayPosition(position, attacks);
			} else if (PieceHelper::isKnight(piece)) {
				const Rawboard attacks = game.board.knightAttack(position, game.board.OPP_PIECES(side));
				game.checkStatus.updateAllCheckPositions(attacks);
				game.checkStatus.addCheckPosition(position, attacks);
			} else if (PieceHelper::isBishop(piece)) {
				const Rawboard attacks = game.board.bishopAttack(position, ~game.board.EMPTY, ~game.board.PIECES(side));
				game.checkStatus.updateAllCheckPositions(attacks);
				game.checkStatus.addCheckPosition(position, attacks);
				game.checkStatus.addXRayPosition(position, attacks);
			} else if (PieceHelper::isQueen(piece)) {
				const Rawboard attacks = game.board.queenAttacks(position, ~game.board.EMPTY, ~game.board.PIECES(side));
				game.checkStatus.updateAllCheckPositions(attacks);
				game.checkStatus.addCheckPosition(position, attacks);
				game.checkStatus.addXRayPosition(position, attacks);
			} else if (PieceHelper::isKing(piece)) {
				const Rawboard attacks = game.board.getKingMoves(position, side);
				game.checkStatus.updateAllCheckPositions(attacks);
				game.checkStatus.addCheckPosition(position, attacks);
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

	static unordered_set<Position>* getPiecePositions(Game& game, const unordered_set<Piece>& pieces) {
        unordered_set<Position>* positions = new unordered_set<Position>();

        for (Position i = 0; i < 64; i++) {
            if (pieces.find(game.board.getPiece(i)) != pieces.end()) {
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

    static bool isOnXRay(Game& game, const Position sourcePosition, const Position excludePosition) {
        unordered_set<Position>* piecePositions = getPiecePositions(game, PieceHelper::getXRayTypes(game.getOppositeSide()));
        piecePositions->erase(excludePosition);
        Rawboard xRayPositions = 0;

        for (Position position : *piecePositions) {
            xRayPositions |= game.board.getDestinationPositions(position);
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

