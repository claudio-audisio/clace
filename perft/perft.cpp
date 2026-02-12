#include <list>

#include "perft.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"
#include "../utils/arrayPool.h"


Perft::Perft(const string& fenGame, const unsigned int depth) {
	this->fenGame = fenGame;
	this->depth = depth;
	this->game = FEN::fenToNewGame(fenGame);
	this->result = new Result(depth);
    this->table = new TranspositionTable();
}

Perft::~Perft() {
	delete this->game;
    delete this->result;
    delete this->table;
}

Result* Perft::runComplete(const bool consoleMode) {
    result->startTime();
    runCompletePerft(depth);
    result->stopTime();
    result->print(fenGame, consoleMode);
    cout << "Cache usage " << table->getUsagePercentage() << " % (" << table->size() << " entries)" << endl;

    return result;
}

void Perft::runCompletePerft(const unsigned int currentDepth) {
    Move* moves;
    const unsigned int amount = table->getMoves(*game, moves);

    if (game->checkStatus.check && amount == 0) {
        result->incrementCheckmates((depth - currentDepth) - 1);
        return;
    }

    if (currentDepth == 0) {
        return;
    }

    for (unsigned int i = 0; i < amount; i++) {
        game->save();
        game->applyMove(moves[i]);
        result->incrementCounters(moves[i], depth - currentDepth);
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

Result* Perft::runBulk() {
    result->startTime();
    const unsLL nodes = runBulkPerft(depth);
    result->stopTime();
    result->incrementNodes(nodes, depth - 1);
    result->print();
    cout << "Cache usage " << table->getUsagePercentage() << " % (" << table->size() << " entries)" << endl;

    return result;
}

unsLL Perft::runBulkPerft(const unsigned int currentDepth) {
    Move* moves;
    const unsigned int amount = table->getMoves(*game, moves);

    if (currentDepth == 1) {
        return amount;
    }

    unsLL nodes = 0;

    for (unsigned int i = 0; i < amount; i++) {
        game->save();
        game->applyMove(moves[i]);
        const unsLL newNodes = runBulkPerft(currentDepth - 1);
        nodes += newNodes;
        game->rollbackLastMove();
    }

    return nodes;
}

// TODO qua viene fatta una iterazione in piu' (come per complete)
// ma la simulate/uindoSimulate della mossa non viene fatta
// eppure i tempi sono duplicati
/*unsLL Perft::runBulkPerft_NEW(const unsigned int currentDepth) {
    if (currentDepth == 0) {
        return 1LL;
    }

    Move* moves = pool->getArray();
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

    pool->release(moves);

    return nodes;
}*/
