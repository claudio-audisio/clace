#pragma once

#include <list>

#include "move.h"
#include "../game/game.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;

class MovesGenerator {
public:

	/*static void generateLegalMoves(Game& game, vector<Move>& moves) {
		generatePseudoLegalMoves(game, moves);
		auto it = moves.begin();

		while (it != moves.end()) {
			if (!isValid(game, *it)) {
				it = moves.erase(it);
			} else {
				++it;
			}
		}
	}*/

	static void generateLegalMoves(Game& game, vector<Move>& moves) {
	//static void generatePseudoLegalMoves(Game& game, vector<Move>& moves) {
		const Side side = game.getSideToMove();
		const bool isComputerToMove = game.isComputerToMove();	// TODO verificare se alla move serve veramente questa info
		Rawboard sources = game.getBoard().BOARD(side);

		/*if (MoveHelper::toString(game.getLastMove()) == "c4-f7") {
			int stop = 1;
		}*/

		while (sources) {
            const Position position = Utils::getFirstPos(sources);
			
			if (game.getCheckStatus().isDoubleCheck() && !game.getBoard().isKing(position)) {
				sources &= (sources - 1);
				continue;
			}

            const Piece piece = game.getBoard().getPiece(position);
			Rawboard destinations = game.getBoard().getDestinationPositions(position, piece, game.getEnPassantPosition(), game.getCastlingInfo());
            unsigned int count = 0;

			while (destinations) {
                const Position destination = Utils::getFirstPos(destinations);
				Move move = MoveHelper::getMove(position, destination, side);
				MoveHelper::decorate(move, piece, game.getEnPassantPosition(), isComputerToMove);

                /*if (MoveHelper::isCastling(move) && game.getMovesHistory().size() == 2) {
                    int stop = 1;
                }*/

                /*if (MoveHelper::isCastling(move) && MoveHelper::toString(move) == "e8-c8" && MoveHelper::toString(game.getLastMove()) == "e5-d7") {
                    int stop = 1;
                }*/

				if (MoveHelper::isPawnPromotion(move)) {
					for (Piece promotion : PieceHelper::getPromotionTypes(side)) {
						Move promotionMove = MoveHelper::getMove(position, destination, side);
						MoveHelper::decorate(promotionMove, piece, game.getEnPassantPosition(), isComputerToMove);
						MoveHelper::setPromotion(promotionMove, promotion);

						if (isValid(game, promotionMove)) {
							moves.push_back(promotionMove);
						}
					}
				}
				else {
					if (isValid(game, move)) {
						moves.push_back(move);
					}
				}

				destinations &= (destinations - 1);
			}

			sources &= (sources - 1);
		}
	}

	static bool isValid(Game& game, Move& move) {
		game.simulateMove(move);
		const bool checkControl = game.checkControl(move);
        game.undoSimulateMove(move);

		return checkControl;
	}

};