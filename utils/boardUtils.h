#pragma once

#include <list>
#include <bit>

#include "../clace.h"
//#include "../common/boardDef.h"


class BoardUtils {
public:

	inline static Rawboard posInd(const Position position) {
        return 1LL << position;
    }

    static void printBoard(const Rawboard board) {
        for (int i = 0; i < 64; ++i) {
            if (i % 8 == 0) {
                cout << "\t";
            }
            if ((board & posInd(i)) == 0) {
                cout << ". ";
            }
            else {
                cout << "1 ";
            }
            if ((i + 1) % 8 == 0) {
                cout << endl;
            }
        }
        cout << endl;
    }

    static void boardToList(Rawboard board, list<Position>& positions) {
        positions.clear();
        for (Position i = 0; i < 64 && (board != 0); ++i) {
            if ((board & 1L) != 0) {
                positions.push_back(i);
            }
            board = board >> 1;
        }
    }

    template <typename... SetOfPosition>
    static Rawboard listToBoard(const SetOfPosition... positions) {
        Rawboard board = 0;

        for (Position position : {positions...}) {
            board |= posInd(position);
        }

        return board;
    }

	inline static bool isUnderCheck(const Rawboard board, const Position position) {
        return (board & posInd(position)) != 0;
    }

	inline static int positionsCount(Rawboard board) {
        return popcount(board);
    }

	inline static Side opposite(const Side side) {
		return (side + SIDE_GAP) % (SIDE_GAP * 2);
	}

	static void initRayAttacks();

};