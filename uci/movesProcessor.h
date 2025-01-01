#pragma once

#include <string>
#include <vector>

#include "../utils/logger.h"

using namespace std;

Logger& logger = Logger::getInstance();

class MovesProcessor {
public:
	vector<string> movesHistory;
	unsigned int movesHistoryIndex = 0;
	VectorPool<Move>* pool = new VectorPool<Move>(1, MAX_MOVES);
	IEngine* engine = new BF_Engine(2);

	void processMove(Game& game, string& command) {
		if (movesHistoryIndex + 1 <= movesHistory.size()) {
			if (movesHistory[movesHistoryIndex] != command) {
				throw runtime_error("error processing moves");
			}
			//logger.log("move " + command + " already processed");
			++movesHistoryIndex;
			return;
		}

		Move move = MoveHelper::parseUciMove(command);
		Position sourcePosition = MoveHelper::getSourcePosition(move);
		Side side = game.board.isWhite(sourcePosition) ? WHITE : BLACK;

		if (game.sideToMove != side) {
			throw runtime_error("uci move side incorrect");
		}

		MoveHelper::setSide(move, side);

		if (side == BLACK) {
			MoveHelper::setPromotion(move, MoveHelper::getPromotion(move) + SIDE_GAP);
		}

		MoveHelper::decorate(move, game.board.piecePositions[sourcePosition], game.board.enPassantPosition, true);
		game.applyMove(move);
		movesHistory.push_back(command);
		++movesHistoryIndex;
	}

	string calculateMove(Game& game) {
		logger.log("calculating best move...");
		game.verifyChecks();
		vector<Move>& moves = pool->getVector(0);
		MovesGenerator::generateLegalMoves(game, moves);

		Evaluation evaluation = engine->calculateMove(game, moves);

		this_thread::sleep_for(chrono::milliseconds (200));

		if (evaluation.first) {
			game.applyMove(evaluation.first);
			string uciMove = MoveHelper::toUciMove(evaluation.first);
			movesHistory.push_back(uciMove);
			++movesHistoryIndex;
			return uciMove;
		}

		movesHistory.push_back("0000");
		++movesHistoryIndex;
		return "0000";
	}

	void resetIndex() {
		movesHistoryIndex = 0;
	}
};