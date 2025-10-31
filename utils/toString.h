#pragma once
#include <stdexcept>
#include <string>

#include "../move/move.h"


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
