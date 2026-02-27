#pragma once

#include <list>

#include "move.h"
#include "../game/game.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"
#include "movesCalculation.h"

using namespace std;

inline void initMovesGenerator() {
	initAttacks();
	initDestPosProviders();
	initPawnAttacksProviders();
}

static bool isValid(const Game& game, Move& move) {
	game.simulateMove(move);
	const bool checkControl = game.checkControl(move);
    game.undoSimulateMove(move);

	return checkControl;
}

static unsigned int generatePseudoLegalMoves(const Game& game, Move* moves) {
	Rawboard sources = PIECES(game.board, game.sideToMove);
	unsigned int count = 0;

	while (sources) {
		const Position position = getFirstPos(sources);

		// TODO tutta questa logica andrebbe messa in un generatore di mosse dedicato al caso del re sotto scacco
		// Va gestita in modo completo a seguito della chiamata di verifyChecks.
		// Al momento non la chiamo, quindi possiamo commentare provvisoriamente
		// https://www.chessprogramming.org/Check
		/*if (game.checkStatus.check) {
			if (game.checkStatus.doubleCheck && !isKing(game.board, position)) {
				sources &= (sources - 1);
				continue;
			}

			/*if (game.checkStatus.discoveryCheck) {
				// TODO le mosse ammesse dovrebbero avere solo come destinazione le caselle dei raggi del re
			} else {
				// TODO le mosse ammesse dovrebbero avere solo come destinazione la destinazione della mossa precedente (cattura)
				// oppure essere una mossa del re
			}#1#
		}*/

		const Piece piece = getPiece(game.board, position);
		Rawboard destinations = getDestinationPositions(game.board, position, piece, game.sideToMove);

		while (destinations) {
			const Position destination = getFirstPos(destinations);
			Move move = createMove(position, destination, game.sideToMove, piece, game.board->enPassantPosition);

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

static unsigned int generateLegalMoves(const Game& game, Move* moves) {
	const unsigned int tot = generatePseudoLegalMoves(game, moves);
	unsigned int legal = 0;

	for (unsigned int i = 0; i < tot; ++i) {
		if (isValid(game, moves[i])) {
			moves[legal++] = moves[i];
		}
	}

	return legal;
}

/*static void generateLegalMoves(Game& game, Move* moves, MovesAmount* legalMoves) {
	Rawboard sources = game.board.PIECES(game.sideToMove);
	unsigned int count = 0;

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
