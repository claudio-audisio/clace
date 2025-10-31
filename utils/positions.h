#pragma once

#include "../common/types.h"

using namespace std;


inline Position coordsToIndex(const string& coordinates) {
    const Position sourceColumn = coordinates.at(0) - 97;
    const Position sourceRow = 7 - (coordinates.at(1) - 49);

    if (sourceColumn < 0 || sourceColumn > 7 || sourceRow < 0 || sourceRow > 7) {
		throw std::runtime_error("piece coordinates malformed");
    }

    return sourceRow * 8 + sourceColumn;
}

inline string indexToCoords(const Position position) {
    string coords;
    const char sourceColumn = (char)((position % 8) + 97);
    const char sourceRow = (char)(7 - (position / 8) + 49);
    coords.append(1, sourceColumn);
    coords.append(1, sourceRow);
    return coords;
}

inline Position getRow(const Position position) {
    return position / 8;
}

inline Position getColumn(const Position position) {
    return position % 8;
}

inline bool isFirstRow(const Position position, const Side side) {
    return getRow(position) == 7 - side * 7;
}

inline bool isSecondRow(const Position position, const Side side) {
    return getRow(position) == 6 - side * 5;
}

inline bool isFourthRow(const Position position, const Side side) {
    return getRow(position) == 4 - side;
}

inline bool isFifthRow(const Position position, const Side side) {
    return getRow(position) == 3 + side;
}

inline bool isEighthRow(const Position position, const Side side) {
    return getRow(position) == side * 7;
}

inline bool areOnSameRowOrColumn(const Position firstPosition, const Position secondPosition) {
    return getRow(firstPosition) == getRow(secondPosition) || getColumn(firstPosition) == getColumn(secondPosition);
}

inline bool areOnSameDiagonal(const Position firstPosition, const Position secondPosition) {
    return abs(getRow(firstPosition) - getRow(secondPosition)) == abs(getColumn(firstPosition) - getColumn(secondPosition));
}
