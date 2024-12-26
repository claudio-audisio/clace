#pragma once

#include <unordered_set>

#include "../clace.h"
#include "../utils/boardUtils.h"
#include "../game/game.h"
#include "../utils/pieceHelper.h"

using namespace std;

class Positions {
public:

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
			throw std::runtime_error("piece coordinates malformed");
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
    
    static bool isEighthRow(const Position position, const Side side) {
		const Position row = getRow(position) + side;
        return row == 0 || row == 7 + SIDE_GAP;
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

