#pragma once

#include <list>
#include <bit>
#include <iostream>

#include "utils.h"
#include "../common/types.h"
#include "../common/bitwise.h"


inline void printBoard(const Rawboard board) {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            int sq = row * 8 + col;
            if (sq % 8 == 0) {
                std::cout << " ";
            }
            if ((board & posInd(sq)) == 0) {
                std::cout << ". ";
            }
            else {
                std::cout << "1 ";
            }
            if ((sq + 1) % 8 == 0) {
                cout << endl;
            }
        }
    }
}

inline void boardToList(Rawboard board, list<Position>& positions) {
    positions.clear();
    for (Position i = 0; i < 64 && (board != 0); ++i) {
        if ((board & 1L) != 0) {
            positions.push_back(i);
        }
        board = board >> 1;
    }
}

template <typename... SetOfPosition>
Rawboard listToBoard(const SetOfPosition... positions) {
    Rawboard board = 0;

    for (Position position : {positions...}) {
        board |= posInd(position);
    }

    return board;
}

inline bool isUnderCheck(const Rawboard board, const Position position) {
    return (board & posInd(position)) != 0;
}

inline bool areUnderCheck(const Rawboard board, const Rawboard positions) {
    return (board & positions) > 0;
}

inline int positionsCount(Rawboard board) {
    return popcount(board);
}

inline void boardToSpaces(Rawboard board, list<Position>& spaces) {
	spaces.push_back(popcount(board));
	while (board) {
	    const Position space = getFirstPos(board);
	    spaces.push_back(space);
	    board = board >> space + 1;
	}
}