#pragma once

#include <list>

#include "../clace.h"

using namespace std;

class BoardUtils {
public:

    static void printBoard(const Rawboard board) {
        for (int i = 0; i < 64; ++i) {
            if (i % 8 == 0) {
                cout << "";
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

    static Rawboard posInd(const Position position) {
        return 1LL << position;
    }

    static Position getFirstPos(Rawboard board) {
        for (Position i = 0; i < 64 && (board != 0); ++i) {
            if ((board & 1L) != 0) {
                return i;
            }
            board = board >> 1;
        }

        return NO_POS;
    }

    static list<Position> boardToList(Rawboard board) {
        list<Position> positions;

        for (Position i = 0; i < 64 && (board != 0); ++i) {
            if ((board & 1L) != 0) {
                positions.push_back(i);
            }
            board = board >> 1;
        }

        return positions;
    }

    template <typename... SetOfPosition>
    static Rawboard listToBoard(const SetOfPosition... positions) {
        Rawboard board = 0;

        for (Position position : {positions...}) {
            board |= posInd(position);
        }

        return board;
    }

    static bool isUnderCheck(const Rawboard board, const Position position) {
        return (board & posInd(position)) != 0;
    }

    static int positionsCount(Rawboard board) {
        int count = 0;

        while (board != 0) {
            count++;
            board &= board - 1; // reset LS1B
        }

        return count;
    }

};