#pragma once

#include <string>
#include <vector>


using namespace std;


class MovesProcessor {
public:
	vector<string> movesHistory;
	unsigned int movesHistoryIndex = 0;
	MovePool* pool = new MovePool(1);
	IEngine* engine = new BF_Engine(2);
	Messenger& messenger = Messenger::getInstance();

	void processMove(Game& game, string& command) {
		if (movesHistoryIndex + 1 <= movesHistory.size()) {
			if (movesHistory[movesHistoryIndex] != command) {
				throw runtime_error("error processing moves");
			}
			//logger.log("move " + command + " already processed");
			++movesHistoryIndex;
			return;
		}

		Move move = parseUciMove(command);
		Position sourcePosition = getSourcePosition(move);
		Side side = isWhite(game.board, sourcePosition) ? _WHITE : _BLACK;

		if (game.sideToMove != side) {
			throw runtime_error("uci move side incorrect");
		}

		setMoveSide(move, side);

		if (side == _BLACK) {
			setPromotion(move, getPromotion(move) + 1);
		}

		decorate(move, game.board->piecePositions[sourcePosition], game.board->enPassantPosition);
		game.applyMove(move);
		movesHistory.push_back(command);
		++movesHistoryIndex;
	}

	string calculateMove(Game& game) {
		messenger.send(MSG_LOG, "movesProcessor", "calculating best move...");
		Evaluation evaluation = engine->calculateMove(game);

		this_thread::sleep_for(chrono::milliseconds (200));

		if (evaluation.move) {
			game.applyMove(evaluation.move);
			string uciMove = toUciMove(evaluation.move);
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