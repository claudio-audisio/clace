#pragma once

#include <list>

#include "move.h"
#include "../game/game.h"
#include "../utils/positions.h"
#include "../utils/pieceHelper.h"

using namespace std;

class MovesGenerator {
public:
	static list<Move*>* getPseudoLegalMoves(Game& game) {
		list<Move*>* pseudoLegalMoves = new list<Move*>();

		for (Position i = 0; i < 64; i++) {
			if (game.checkColor(i)) {
				if (game.getCheckStatus().isDoubleCheck() && !game.isKing(i)) {
					continue;
				}

				Positions::PieceAndDest* pieceAndDest = Positions::getDestinationPositions(game, i);
				const list<Position> destinationCells = BoardUtils::boardToList(pieceAndDest->getBoard());

				for (Position destination : destinationCells) {
					Move* move = new Move(i, destination, game.isWhiteToMove());
					move->decorate(pieceAndDest->getPiece(), game.getEnPassantPosition(), game.isComputerToMove());

					if (move->isPawnPromotion()) {
						for (Piece promotion : PieceHelper::getPromotionTypes(move->isWhite())) {
							Move* promotionMove = new Move(i, destination, game.isWhiteToMove());
							promotionMove->decorate(pieceAndDest->getPiece(), game.getEnPassantPosition(), game.isComputerToMove());
							promotionMove->setPromotion(promotion);
							pseudoLegalMoves->push_back(promotionMove);
						}
					}
					else {
						pseudoLegalMoves->push_back(move);
					}
				}

				delete pieceAndDest;
			}
		}

		return pseudoLegalMoves;
	}

	static void calculateLegalMoves(Game& game) {
		game.getNextMoves().clear();
		list<Move*>* pseudoLegalMoves = getPseudoLegalMoves(game);

		for (Move* move : *pseudoLegalMoves) {
			if (isValid(game, move)) {
				game.getNextMoves().push_back(move);
			}
			else {
				delete move;
			}
		}

		delete pseudoLegalMoves;
	}

	static bool isValid(Game& game, Move* move) {
		game.lightSave();
		game.simulateMove(*move);
		const bool checkControl = game.checkControl(*move);
		game.lightRollback();

		return checkControl;
	}

};