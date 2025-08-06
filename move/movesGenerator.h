#pragma once

#include <list>

#include "move.h"
#include "../game/game.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;

class MovesGenerator {
public:

	static bool isValid(Game& game, Move& move) {
		game.simulateMove(move);
		const bool checkControl = game.checkControl(move);
        game.undoSimulateMove(move);

		return checkControl;
	}

	static MovesAmount generateLegalMoves(Game& game, Move moves[]) {
		const unsigned char tot = generatePseudoLegalMoves(game, moves);
		unsigned char removed = 0;

		for (unsigned int i = 0; i < tot; ++i) {
			if (!isValid(game, moves[i])) {
				moves[i] = 0;
				++removed;
			}
		}

		return make_pair(tot,tot - removed);
	}

	static unsigned char generatePseudoLegalMoves(Game& game, Move moves[]) {
		const bool isComputerToMove = game.isComputerToMove();	// TODO verificare se alla move serve veramente questa info
		Rawboard sources = game.board.PIECES(game.sideToMove);
		unsigned char count = 0;

		/*if (MoveHelper::toString(game.lastMove) == "e2-b5") {
			int stop = 1;
		}*/

		while (sources) {
            const Position position = Utils::getFirstPos(sources);

			if (game.checkStatus.doubleCheck && !game.board.isKing(position)) {
				sources &= (sources - 1);
				continue;
			}

            const Piece piece = game.board.getPiece(position);
			Rawboard destinations = game.board.getDestinationPositions(position, piece);

			while (destinations) {
                const Position destination = Utils::getFirstPos(destinations);
				Move move = MoveHelper::getMove(position, destination, game.sideToMove, piece, game.board.enPassantPosition, isComputerToMove);

                /*if (MoveHelper::isCastling(move) && game.movesHistory.size() == 2) {
                    int stop = 1;
                }*/

                /*if (MoveHelper::isCastling(move) && MoveHelper::toString(move) == "e8-c8" && MoveHelper::toString(game.lastMove) == "e5-d7") {
                    int stop = 1;
                }*/

				if (MoveHelper::isPawnPromotion(move)) {
					for (Piece promotion : PieceHelper::getPromotionTypes(game.sideToMove)) {
						Move promotionMove = move;
						MoveHelper::setPromotion(promotionMove, promotion);

						//if (isValid(game, promotionMove)) {
							moves[count++] = promotionMove;
						//}
					}
				}
				else {
					//if (isValid(game, move)) {
						moves[count++] = move;
					//}
				}

				destinations &= (destinations - 1);
			}

			sources &= (sources - 1);
		}

		return count;
	}

};