#include <list>

#include "perft.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"
#include "../common/defines.h"




Perft::Perft(const string& fenGame, const unsigned int depth) {
	this->fenGame = fenGame;
	this->depth = depth;
	this->game = FEN::fenToNewGame(fenGame);
	this->result = new Result(depth);
    this->pool = new ArrayPool<Move>(depth + 1);
#ifdef PERFT_USE_CACHE
    this->cache = new MovesCache(0);
#else
    this->cache = nullptr;
#endif
}

Perft::~Perft() {
	delete this->game;
    delete this->result;
    delete this->pool;

    if (this->cache) {
        delete this->cache;
    }
}

Result* Perft::runComplete(const bool consoleMode) {
    result->startTime();
    runCompletePerft(depth);
    result->stopTime();
    result->print(fenGame, consoleMode);
#ifdef PERFT_USE_CACHE
    cout << "Cache usage " << (cacheUsage * 100) / (cacheUsage + generatorUsage) << " %" << endl;
#endif
    return result;
}

void Perft::runCompletePerft(const unsigned int currentDepth) {
    Move* moves = pool->getArray(currentDepth);
    MovesAmount amount;

#ifdef PERFT_USE_CACHE
    const string fenKey = FEN::gameToFENKey(*game);
    if (!cache->get(fenKey, moves, amount)) {
        generateLegalMoves(*game, moves, &amount);
        cache->add(fenKey, moves, amount.total, amount.legal);
        generatorUsage++;
    } else {
        cacheUsage++;
    }
#else
    generateLegalMoves(*game, moves, &amount);
#endif

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
#ifdef PERFT_USE_CACHE
    cout << "Cache usage " << (cacheUsage * 100) / (cacheUsage + generatorUsage) << " %" << endl;
#endif
    return result;
}

unsLL Perft::runBulkPerft(const unsigned int currentDepth) {
    Move* moves = pool->getArray(currentDepth - 1);
    MovesAmount amount;

#ifdef PERFT_USE_CACHE
    const string fenKey = FEN::gameToFENKey(*game);
    if (!cache->get(fenKey, moves, amount)) {
        generateLegalMoves(*game, moves, &amount);
        cache->add(fenKey, moves, amount.total, amount.legal);
        generatorUsage++;
    } else {
        cacheUsage++;
    }
#else
    generateLegalMoves(*game, moves, &amount);
#endif

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
