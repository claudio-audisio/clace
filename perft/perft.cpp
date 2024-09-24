#include <list>

#include "perft.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"


Perft::Perft(const string& fenGame, const unsigned int depth) {
	this->fenGame = fenGame;
	this->depth = depth;
	this->game = FEN::fenToNewGame(fenGame);
	this->result = new Result(depth);
    this->pool = new VectorPool(depth);
}

Perft::~Perft() {
    delete this->result;
    delete this->pool;
}

Result* Perft::runBulk() {
    result->startTime();
    const unsLL nodes = runBulkPerft(depth);
    result->stopTime();
    result->incrementNodes(nodes, depth - 1);
    result->print();
    return result;
}

unsLL Perft::runBulkPerft(const unsigned int depth) {
    vector<Move>& moves = pool->getVector(depth - 1);
    MovesGenerator::calculateLegalMoves(*game, moves);

    if (depth == 1) {
        return moves.size();
    }

    unsLL nodes = 0;

    for (Move move : moves) {
        game->save();
        game->applyMove(move);
        nodes += runBulkPerft(depth - 1);
        game->rollbackLastMove();
    }

    return nodes;
}

Result* Perft::run(const bool consoleMode) {
    result->startTime();
    runPerft(depth);
    result->stopTime();
    result->print(fenGame, consoleMode);
    return result;
}

void Perft::runPerft(const unsigned int currentDepth) {
    vector<Move> moves;
    moves.reserve(200);
    MovesGenerator::calculateLegalMoves(*game, moves);

    if (game->getCheckStatus().isCheck() && moves.empty()) {
        result->incrementCheckmates((depth - currentDepth) - 1);
        return;
    }

    if (currentDepth == 0) {
        return;
    }

    for (Move move : moves) {
        game->save();
        MoveResult moveResult = game->applyMove(move);
        result->incrementCounters(moveResult, depth - currentDepth);
        game->verifyChecks();
        result->incrementCounters(game->getCheckStatus(), depth - currentDepth);
        runPerft(currentDepth - 1);
        game->rollbackLastMove();
    }
}