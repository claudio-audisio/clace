#include <list>

#include "perft.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"


Perft::Perft(const string& fenGame, const unsigned int depth) {
	this->fenGame = fenGame;
	this->depth = depth;
	this->game = FEN::fenToNewGame(fenGame);
	this->result = new Result(depth);
    this->pool = new VectorPool<Move>(depth + 1, MAX_MOVES);
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

unsLL Perft::runBulkPerft(const unsigned int currentDepth) {
    vector<Move>& moves = pool->getVector(currentDepth - 1);
    MovesGenerator::calculateLegalMoves(*game, moves);

    if (currentDepth == 1) {
        /*for (Move move : moves) {
            cout << MoveHelper::toString(move) << " ";
        }
        cout << endl;*/
        return moves.size();
    }

    unsLL nodes = 0;
    //int counter = 0;

    for (Move move : moves) {
        game->save();
        game->applyMove(move);
        //cout << ++counter << "\t" << MoveHelper::toString(move) << endl;
        const unsLL newNodes = runBulkPerft(currentDepth - 1);
        nodes += newNodes;
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
    vector<Move>& moves = pool->getVector(currentDepth);
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
        const MoveResult moveResult = game->applyMove(move);
        result->incrementCounters(moveResult, depth - currentDepth);
        game->verifyChecks();
        result->incrementCounters(game->getCheckStatus(), depth - currentDepth);
        //cout << game->printMovesHistory() << " done " << endl;
        runPerft(currentDepth - 1);
        game->rollbackLastMove();
    }
}