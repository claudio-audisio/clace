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
    this->pool = new VectorPool<Move>(depth + 1, MAX_MOVES);
    this->pool_new = new ArrayPool<Move>(depth + 1, MAX_MOVES);
#ifdef PERFT_USE_CACHE
    this->cache = new MovesCache(0);
#endif
}

Perft::~Perft() {
	delete this->game;
    delete this->result;
    delete this->pool;
    delete this->pool_new;
#ifdef PERFT_USE_CACHE
	delete this->cache;
#endif
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
    vector<Move>& moves = pool->getVector(currentDepth - 1);

#ifdef PERFT_USE_CACHE
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
#ifdef PERFT_USE_CACHE
    cout << "Cache usage " << (cacheUsage * 100) / (cacheUsage + generatorUsage) << " %" << endl;
#endif
    return result;
}

void Perft::runPerft(const unsigned int currentDepth) {
    vector<Move>& moves = pool->getVector(currentDepth);

#ifdef PERFT_USE_CACHE
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

Result* Perft::runNew(const bool consoleMode) {
    result->startTime();
    runPerftNew(depth);
    result->stopTime();
    result->print(fenGame, consoleMode);
#ifdef PERFT_USE_CACHE
    cout << "Cache usage " << (cacheUsage * 100) / (cacheUsage + generatorUsage) << " %" << endl;
#endif
    return result;
}

void Perft::runPerftNew(const unsigned int currentDepth) {
    Move* moves = pool_new->getArray(currentDepth - 1);
    pair<unsigned int, unsigned int> res;

#ifdef PERFT_USE_CACHE
    const string fenKey = FEN::gameToFENKey(*game);
    if (!cache->getNew(fenKey, moves, res)) {
        res = MovesGenerator::generateLegalMoves_new(*game, moves);
        cache->addNew(fenKey, moves, res.first, res.second);
        generatorUsage++;
    } else {
        cacheUsage++;
    }
#else
    res = MovesGenerator::generateLegalMoves_new(*game, moves);
#endif

    if (game->checkStatus.check && res.second == 0) {
        result->incrementCheckmates((depth - currentDepth) - 1);
        return;
    }

    if (currentDepth == 0) {
        return;
    }

    for (unsigned int i = 0; i < res.first; i++) {
        if (moves[i]) {
            game->save();
            const MoveResult moveResult = game->applyMove(moves[i]);
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
}

Result* Perft::runBulk_new() {
    result->startTime();
    const unsLL nodes = runBulkPerft_new(depth);
    result->stopTime();
    result->incrementNodes(nodes, depth - 1);
    result->print();
#ifdef PERFT_USE_CACHE
    cout << "Cache usage " << (cacheUsage * 100) / (cacheUsage + generatorUsage) << " %" << endl;
#endif
    return result;
}

unsLL Perft::runBulkPerft_new(const unsigned int currentDepth) {
    Move* moves = pool_new->getArray(currentDepth - 1);
    pair<unsigned int, unsigned int> res;

#ifdef PERFT_USE_CACHE
    const string fenKey = FEN::gameToFENKey(*game);
    if (!cache->getNew(fenKey, moves, res)) {
        res = MovesGenerator::generateLegalMoves_new(*game, moves);
        cache->addNew(fenKey, moves, res.first, res.second);
        generatorUsage++;
    } else {
        cacheUsage++;
    }
#else
    res = MovesGenerator::generateLegalMoves_new(*game, moves);
#endif

    if (currentDepth == 1) {
        return res.second;
    }

    unsLL nodes = 0;

    for (unsigned int i = 0; i < res.first; i++) {
        if (moves[i]) {
            game->save();
            game->applyMove(moves[i]);
            const unsLL newNodes = runBulkPerft_new(currentDepth - 1);
            nodes += newNodes;
            game->rollbackLastMove();
        }
    }

    return nodes;
}
