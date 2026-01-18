#include <list>

#include "perft.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"
#include "../utils/movePool.h"


Perft::Perft(const string& fenGame, const unsigned int depth) {
	this->fenGame = fenGame;
	this->depth = depth;
	this->game = FEN::fenToNewGame(fenGame);
	this->result = new Result(depth);
    this->pool = new MovePool(depth + 1);
}

Perft::~Perft() {
	delete this->game;
    delete this->result;
    delete this->pool;
    table.clear();
}

Result* Perft::runComplete(const bool consoleMode) {
    result->startTime();
    runCompletePerft(depth);
    result->stopTime();
    result->print(fenGame, consoleMode);

    if (table.getUsage() != -1) {
        cout << "Cache usage " << table.getUsage() * 100 / totalUsage << " %" << endl;
    }

    return result;
}

void Perft::runCompletePerft(const unsigned int currentDepth) {
    Move* moves = pool->getArray(currentDepth);
    MovesAmount amount;

    table.getMoves(*game, moves, amount);
    totalUsage++;

    if (game->checkStatus.check && amount.legal == 0) {
        result->incrementCheckmates((depth - currentDepth) - 1);
        return;
    }

    if (currentDepth == 0) {
        return;
    }

    for (unsigned int i = 0; i < amount.total; i++) {
        if (moves[i]) {
            game->save();
            const MoveResult moveResult = game->applyMove(moves[i]);
            result->incrementCounters(moveResult, depth - currentDepth);
            game->verifyChecks();
            result->incrementCounters(game->checkStatus, depth - currentDepth);
            //cout << game->printMovesHistory() << " done " << endl;
            runCompletePerft(currentDepth - 1);
            /*if (currentDepth == 3) {
                cout << moveToString(move) << "\t" << result->getCaptures(2) << endl;
            }*/
            game->rollbackLastMove();
        }
    }
}

Result* Perft::runBulk() {
    result->startTime();
    const unsLL nodes = runBulkPerft(depth);
    result->stopTime();
    result->incrementNodes(nodes, depth - 1);
    result->print();

    if (table.getUsage() != -1) {
        cout << "Cache usage " << table.getUsage() * 100 / totalUsage << " %" << endl;
    }

    return result;
}

unsLL Perft::runBulkPerft(const unsigned int currentDepth) {
    Move* moves = pool->getArray(currentDepth - 1);
    MovesAmount amount;

    table.getMoves(*game, moves, amount);
    totalUsage++;

    if (currentDepth == 1) {
        return amount.legal;
    }

    unsLL nodes = 0;

    for (unsigned int i = 0; i < amount.total; i++) {
        if (moves[i]) {
            game->save();
            game->applyMove(moves[i]);
            const unsLL newNodes = runBulkPerft(currentDepth - 1);
            nodes += newNodes;
            game->rollbackLastMove();
        }
    }

    return nodes;
}

// TODO qua viene fatta una iterazione in piu' (come per complete)
// ma la simulate/uindoSimulate della mossa non viene fatta
// eppure i tempi sono duplicati
unsLL Perft::runBulkPerft_NEW(const unsigned int currentDepth) {
    if (currentDepth == 0) {
        return 1LL;
    }

    Move* moves = pool->getArray(currentDepth - 1);
    const unsigned int tot = generatePseudoLegalMoves(*game, moves);

    unsLL nodes = 0;

    for (unsigned int i = 0; i < tot; i++) {
        game->save();
        game->applyMove(moves[i]);

        if (game->checkControl(moves[i])) {
            const unsLL newNodes = runBulkPerft_NEW(currentDepth - 1);
            nodes += newNodes;
        }

        game->rollbackLastMove();
    }

    return nodes;
}
