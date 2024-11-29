#include <list>

#include "perft.h"
#include "../utils/fen.h"
#include "../move/movesGenerator.h"

#define USE_CACHE


Perft::Perft(const string& fenGame, const unsigned int depth) {
	this->fenGame = fenGame;
	this->depth = depth;
	this->game = FEN::fenToNewGame(fenGame);
	this->result = new Result(depth);
    this->pool = new VectorPool<Move>(depth + 1, MAX_MOVES);
#ifdef USE_CACHE
    this->cache = new MovesCache(0);
#endif
}

Perft::~Perft() {
	delete this->game;
    delete this->result;
    delete this->pool;
#ifdef USE_CACHE
	delete this->cache;
#endif
}

Result* Perft::runBulk() {
    result->startTime();
    const unsLL nodes = runBulkPerft(depth);
    result->stopTime();
    result->incrementNodes(nodes, depth - 1);
    result->print();
#ifdef USE_CACHE
    cout << "Cache usage " << (cacheUsage * 100) / (cacheUsage + generatorUsage) << " %" << endl;
#endif
    return result;
}

unsLL Perft::runBulkPerft(const unsigned int currentDepth) {
    vector<Move>& moves = pool->getVector(currentDepth - 1);

#ifdef USE_CACHE
	const string fenKey = FEN::gameToFENKey(*game);
    if (!cache->get(fenKey, moves)) {
        MovesGenerator::generateLegalMoves(*game, moves);
        cache->add(fenKey, moves);
        generatorUsage++;
    } else {
        cacheUsage++;
    }
#else
	MovesGenerator::generateLegalMoves(*game, moves);
#endif

    if (currentDepth == 1) {
        /*for (Move move : moves) {
            cout << MoveHelper::toString(move) << " ";
        }
        cout << endl;*/
        return moves.size();
    }

    unsLL nodes = 0;
    int counter = 0;

    for (Move move : moves) {
        game->save();
        game->applyMove(move);
        const unsLL newNodes = runBulkPerft(currentDepth - 1);
		/*if (currentDepth == 2) {
			cout << ++counter << "\t" << MoveHelper::toString(move) << "\t" << newNodes << endl;
		}*/
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
#ifdef USE_CACHE
    cout << "Cache usage " << (cacheUsage * 100) / (cacheUsage + generatorUsage) << " %" << endl;
#endif
    return result;
}

void Perft::runPerft(const unsigned int currentDepth) {
    vector<Move>& moves = pool->getVector(currentDepth);

#ifdef USE_CACHE
	const string fenKey = FEN::gameToFENKey(*game);
    if (!cache->get(fenKey, moves)) {
        MovesGenerator::generateLegalMoves(*game, moves);
        cache->add(fenKey, moves);
        generatorUsage++;
    } else {
        cacheUsage++;
    }
#else
	MovesGenerator::generateLegalMoves(*game, moves);
#endif

    if (game->checkStatus.check && moves.empty()) {
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
        result->incrementCounters(game->checkStatus, depth - currentDepth);
        //cout << game->printMovesHistory() << " done " << endl;
        runPerft(currentDepth - 1);
		/*if (currentDepth == 3) {
			cout << MoveHelper::toString(move) << "\t" << result->getCaptures(2) << endl;
		}*/
        game->rollbackLastMove();
    }
}