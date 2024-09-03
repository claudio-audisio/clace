#pragma once

#include "../clace.h"

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

    static Position coordsToIndex(const string& coordinates) {
        const int sourceColumn = coordinates.at(0) - 97;
        const int sourceRow = 7 - (coordinates.at(1) - 49);

        if (sourceColumn < 0 || sourceColumn > 7 || sourceRow < 0 || sourceRow > 7) {
            // TODO tirare eccezione
            // throw new RuntimeException("piece coordinates malformed");
        }

        return sourceRow * 8 + sourceColumn;
    }

    static string indexToCoords(int position) {
        string coords;
        const char sourceColumn = (char)((position % 8) + 97);
        const char sourceRow = (char)(7 - (position / 8) + 49);
        coords.append(1, sourceColumn);
        coords.append(1, sourceRow);
        return coords;
    }

};

