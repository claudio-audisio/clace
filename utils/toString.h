#pragma once
#include <stdexcept>
#include <string>

#include "../move/move.h"


static std::string sideToString(const Side side) {
    return side == 0 ? "white" : "black";
}

static std::string moveToString(const Move move) {
    if (!move) {
        return "--";
    }

    return indexToCoords(getSourcePosition(move)) + indexToCoords(getDestinationPosition(move));
}

static std::string endGameToString(const EndGameType endgame) {
    switch (endgame) {
        case NONE: return "NONE"; break;
        case FIFTY_MOVE_RULE: return "FIFTY MOVE"; break;
        case FIVEFOLD_REPETITION: return "FIVEFOLD REPETITION"; break;
        case STALEMATE: return "STALEMATE"; break;
        case CHECKMATE: return "CHECKMATE"; break;
        default: throw new runtime_error("Invalid endgame type");
    }
}

static std::string pieceToString(const Piece piece) {
    return PIECE_TO_STRING.at(piece);
}

static std::string movesToString(const Move* moves, const unsigned int size) {
    if (moves ==  nullptr) {
        return "--";
    }

    string list = "[";

    for (int i = 0; i < size; i++) {
        list += moveToString(moves[i]);

        if (i != size - 1) {
            list += ", ";
        }
    }

    list += "]";

    return list;
}

static std::string evalValueToString(const Evaluation& evaluation) {
    return /*evaluation.endGameType != NONE ? endGameToString(evaluation.endGameType) :*/ format("{:.2f}", evaluation.value);
}

static std::string evalShortToString(const Evaluation& evaluation) {
    return format("{} --> {}", movesToString(evaluation.pvMoves, evaluation.depth), evalValueToString(evaluation));
}

static std::string evalToString(const Evaluation& evaluation) {
    return format("{} --> {} {} {}", moveToString(evaluation.move), evalValueToString(evaluation), movesToString(evaluation.pvMoves, evaluation.depth), evaluation.depth);
}
