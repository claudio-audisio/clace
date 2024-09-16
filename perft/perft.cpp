#include <list>

#include "perft.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"


Perft::Perft(const string& fenGame, const unsigned int depth) {
	this->fenGame = fenGame;
	this->depth = depth;
	this->game = FEN::fenToNewGame(fenGame);
	this->result = new Result(depth);
}

Perft::~Perft() {

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
    MovesGenerator::calculateLegalMoves(*game);

    if (depth == 1) {
        return game->getNextMoves().size();
    }

    unsLL nodes = 0;
    list<Move*> moves = game->getNextMoves();

    for (Move* move : moves) {
        game->save();
        game->applyMove(*move);
        nodes += runBulkPerft(depth - 1);
        game->rollbackLastMove();
    }

    return nodes;
}

unsLL Perft::runBulkPerft2(const unsigned int depth) {
    if (depth == 0) {
        return 1;
    }

    unsLL nodes = 0;
    list<Move*> moves;
    MovesGenerator::getPseudoLegalMoves(*game, moves);

    for (Move* move : moves) {
        game->save();
        game->applyMove(*move);
        if (game->checkControl(*move)) {
            nodes += runBulkPerft2(depth - 1);
        }
        game->rollbackLastMove();
    }

    return nodes;
}