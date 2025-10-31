#pragma once

#include <list>

#include "move.h"
#include "../game/game.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"
#include "../movesCalculation/movesCalculation.h"

using namespace std;


static bool isValid(Game& game, Move& move) {
	game.simulateMove(move);
	const bool checkControl = game.checkControl(move);
    game.undoSimulateMove(move);

	return checkControl;
}

static unsigned char generatePseudoLegalMoves(Game& game, Move* moves) {
	Rawboard sources = PIECES(game.board, game.sideToMove);
	unsigned char count = 0;

	while (sources) {
		const Position position = getFirstPos(sources);

		if (game.checkStatus.doubleCheck && !isKing(game.board, position)) {
			sources &= (sources - 1);
			continue;
		}

		const Piece piece = getPiece(game.board, position);
		Rawboard destinations = getDestinationPositions(game.board, position, piece, game.sideToMove);

		while (destinations) {
			const Position destination = getFirstPos(destinations);
			Move move = createMove(position, destination, game.sideToMove, piece, game.board.enPassantPosition);

			if (isPawnPromotion(move)) {
				setPromotion(move, WRook + game.sideToMove);
				moves[count++] = move;
				setPromotion(move, WKnight + game.sideToMove);
				moves[count++] = move;
				setPromotion(move, WBishop + game.sideToMove);
				moves[count++] = move;
				setPromotion(move, WQueen + game.sideToMove);
				moves[count++] = move;
			}
			else {
				moves[count++] = move;
			}

			destinations &= (destinations - 1);
		}

		sources &= (sources - 1);
	}

	return count;
}

static void generateLegalMoves(Game& game, Move* moves, MovesAmount* legalMoves) {
	const unsigned char tot = generatePseudoLegalMoves(game, moves);
	unsigned char removed = 0;

	for (unsigned int i = 0; i < tot; ++i) {
		if (!isValid(game, moves[i])) {
			moves[i] = 0;
			++removed;
		}
	}

	legalMoves->total = tot;
	legalMoves->legal = tot - removed;
}

/*static void generateLegalMoves(Game& game, Move* moves, MovesAmount* legalMoves) {
	Rawboard sources = game.board.PIECES(game.sideToMove);
	unsigned char count = 0;

	while (sources) {
		const Position position = getFirstPos(sources);

		if (game.checkStatus.doubleCheck && !game.board.isKing(position)) {
			sources &= (sources - 1);
			continue;
		}

		const Piece piece = game.board.getPiece(position);
		Rawboard destinations = game.board.getDestinationPositions(position, piece);

		while (destinations) {
			const Position destination = getFirstPos(destinations);
			Move move = createMove(position, destination, game.sideToMove, piece, game.board.enPassantPosition);

			if (isValid(game, move)) {
				if (!isPawnPromotion(move)) {
					moves[count++] = move;
				} else {
					setPromotion(move, WRook + game.sideToMove);
					moves[count++] = move;
					setPromotion(move, WKnight + game.sideToMove);
					moves[count++] = move;
					setPromotion(move, WBishop + game.sideToMove);
					moves[count++] = move;
					setPromotion(move, WQueen + game.sideToMove);
					moves[count++] = move;
				}
			}

			destinations &= (destinations - 1);
		}

		sources &= (sources - 1);
	}

	legalMoves->total = count;
	legalMoves->legal = count;
}*/
