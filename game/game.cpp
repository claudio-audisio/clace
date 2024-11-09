#include <cassert>

#include "game.h"
#include "../utils/fen.h"
#include "../move/move.h"


Game::Game() {
	BoardUtils::initAttacks();
}

Game::~Game() {
	delete whitePlayer;
	delete blackPlayer;
}

void Game::init() {
	initPlayers();
	initFromFEN(INITIAL_FEN_POSITION);
}

void Game::initFromFEN(const string& fenPosition) {
	initPlayers();
	FEN::fenToGame(fenPosition, *this);
}

void Game::initPlayers() {
	whitePlayer = new Player(true);
	blackPlayer = new Player(false);
}

void Game::initPlayers(Player* white, Player* black) {
	whitePlayer = white;
	blackPlayer = black;
}

MoveResult Game::finalizeMove(Move& move) {
	Piece captured = board.move(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move), MoveHelper::getPiece(move), MoveHelper::isCastling(move));
	board.updateCastlingInfo(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move));
	updateEnPassantInfo(move);

    if (MoveHelper::isPawnPromotion(move)) {
        completePawnPromotion(move);
    }

    if (captured == Empty && MoveHelper::isEnPassant(move)) {
		captured = completeEnPassant(move);
    }

    MoveHelper::setCaptured(move, captured);

	if (captured == Empty && !MoveHelper::isPawnPromotion(move) && !board.isPawn(MoveHelper::getDestinationPosition(move))) {
		halfMoveClock++;
	}
	else {
		halfMoveClock = 0;
	}

	return MoveHelper::getMoveResult(captured != Empty, MoveHelper::isPawnPromotion(move), MoveHelper::isEnPassant(move), MoveHelper::isCastling(move));
}

MoveResult Game::applyMove(Move& move) {
	MoveResult moveResult = finalizeMove(move);
	lastMove = move;
	movesHistory.push_front(move);

	if (!MoveHelper::isWhite(move)) {
		fullMoves++;
	}

	changeTurn();
	return moveResult;
}

void Game::applyMoves(list<Move>& moves) {
    for (reverse_iterator it = moves.rbegin(); it != moves.rend(); it++) {
        applyMove(*it);
    }
}

void Game::simulateMove(Move& move) {
	Piece captured = board.move(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move), MoveHelper::getPiece(move), MoveHelper::isCastling(move));

    if (MoveHelper::isPawnPromotion(move)) {
        completePawnPromotion(move);
    }

    if (captured == Empty) {
        /*if (MoveHelper::isCastling(move)) {
            board.completeCastlingMove(MoveHelper::getDestinationPosition(move));
        }
        else*/ if (MoveHelper::isEnPassant(move)) {
            captured = completeEnPassant(move);
        }
    }

    MoveHelper::setCaptured(move, captured);
}

void Game::undoSimulateMove(Move& move) {
	if (MoveHelper::isCastling(move)) {
		board.undoCastlingMove(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move));
	} else {
		board.setPiece(MoveHelper::getSourcePosition(move), MoveHelper::getPiece(move));
		board.setEmpty(MoveHelper::getDestinationPosition(move));
		const Piece captured = MoveHelper::getCaptured(move);

		if (captured != Empty) {
			if (MoveHelper::isEnPassant(move)) {
				undoEnPassant(move);
			} else {
				board.setPiece(MoveHelper::getDestinationPosition(move), captured);
			}
		}
	}

    // Undo Pawn promotion is not needed because undoing original move we have already moved a Pawn
}

void Game::undoEnPassant(Move& move) {
    const Position destination = MoveHelper::getDestinationPosition(move) + (MoveHelper::isWhite(move) ? 8 : -8);
    board.setPiece(destination, MoveHelper::isWhite(move) ? BPawn : WPawn);
}

void Game::verifyChecks() {
    checkStatus.reset();
	Positions::calculateCheckPositions(*this, getOppositeSide());
	const Position kingPosition = board.getKingPosition(sideToMove);
	checkStatus.updateStatus(kingPosition, movesHistory.empty() ? 0 : movesHistory.front());
}

EndGameType Game::checkEndGame(const bool noMoves) {
	if (noMoves) {
		return checkStatus.check ? EndGameType::CHECKMATE : EndGameType::STALEMATE;
	}

	return checkFiftyMoveRule();
}

EndGameType Game::checkFiftyMoveRule() {
	return halfMoveClock >= 50 ? EndGameType::FIFTY_MOVE_RULE : EndGameType::NONE;
}

bool Game::checkFiveFoldRepetitions() {
	// TODO
	return false;
}

bool Game::checkControl(const Move& move) {
	const Side side = MoveHelper::getSide(move);
	const Rawboard checkBoard = board.getAttacks(BLACK - side);
	const Position kingPosition = board.getKingPosition(side);

	if (BoardUtils::isUnderCheck(checkBoard, kingPosition)) {
		return false;
	}

	bool castlingNotValid = false;

	if (MoveHelper::isCastling(move)) {
		if (side) {
			switch (MoveHelper::getDestinationPosition(move)) {
				case 2: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 4) || BoardUtils::isUnderCheck(checkBoard, 3); break;
				case 6: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 4) || BoardUtils::isUnderCheck(checkBoard, 5); break;
				default: break;
			}
		}
		else {
			switch (MoveHelper::getDestinationPosition(move)) {
				case 58: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 60) || BoardUtils::isUnderCheck(checkBoard, 59); break;
				case 62: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 60) || BoardUtils::isUnderCheck(checkBoard, 61); break;
				default: break;
			}
		}
	}

	return !castlingNotValid;
}

void Game::updateEnPassantInfo(const Move& move) {
	if (PieceHelper::isPawn(MoveHelper::getPiece(move)) &&
		Positions::isSecondRow(MoveHelper::getSourcePosition(move), MoveHelper::isWhite(move)) &&
		Positions::isFourthRow(MoveHelper::getDestinationPosition(move), MoveHelper::isWhite(move))) {
		board.enPassantPosition = MoveHelper::getSourcePosition(move) + (MoveHelper::isWhite(move) ? -8 : 8);
	}
	else {
		board.enPassantPosition = NO_POS;
	}
}

Piece Game::completeEnPassant(const Move& move) {
	const Position destination = MoveHelper::getDestinationPosition(move) + (MoveHelper::isWhite(move) ? 8 : -8);
	return board.setEmpty(destination);
}

void Game::completePawnPromotion(const Move& move) {
	const Piece promotionPiece = MoveHelper::getPromotion(move);

	if (promotionPiece == Empty) {
		throw runtime_error("promotion piece not set");
	}

	//System.out.print("Pawn promotion --> " + MoveHelper::getPieceTypeForPawnPromotion().name());
	board.setPiece(MoveHelper::getDestinationPosition(move), promotionPiece);
}

void Game::changeTurn() {
	sideToMove = BoardUtils::opposite(sideToMove);
}

Side Game::getSide(Position position) const {
	if (board.isEmpty(position)) {
		assert(false);
	}

	if (board.isWhite(position)) {
		return WHITE;
	}

	return BLACK;
}

void Game::save() {
	rollback.save(*this);
}

void Game::rollbackLastMove() {
	checkStatus.reset();
	movesHistory.pop_front();
	lastMove = movesHistory.empty() ? 0 : movesHistory.front();
	rollback.rollback(*this);
}

/*
public Evaluation evaluate(Move move) {
	return Optional.ofNullable(evaluator).map(e->e.evaluate(this, move)).orElseGet(() -> new Evaluation(move, Double.NaN));
}

public double evaluateAsWhite() {
	if (lastMove != null) {
		return evaluate(lastMove).getValue() * (isWhiteToMove() ? -1 : 1);
	}

	return 0;
}
*/
Player* Game::getCurrentPlayer() const {
	return isWhiteToMove() ? whitePlayer : blackPlayer;
}

bool Game::isComputerToMove() const {
	Player* player = this->getCurrentPlayer();

	if (!player) {
		return false;
	}
	
	return player->computer;
}

void Game::setLastMove(const Move& move) {
	movesHistory.push_front(move);
	lastMove = move;
}

Game* Game::duplicate() {
	Game* newGame = new Game();
	newGame->init();
	newGame->board.set(board);
	newGame->board.castlingInfo = board.castlingInfo;
	newGame->sideToMove = sideToMove;
	newGame->board.enPassantPosition = board.enPassantPosition;
	newGame->fullMoves = fullMoves;
	newGame->halfMoveClock = halfMoveClock;
	Utils::dequeAddAll(movesHistory, newGame->movesHistory);
	newGame->checkStatus.set(checkStatus);
	return newGame;
}

string Game::printMovesHistory() {
    string moves;
    for (Move move : movesHistory) {
        moves = MoveHelper::toString(move) + ", " + moves;
    }
    return moves.substr(0, moves.length() - 2);
}

string Game::printCastlingInfo() const {
    return FEN::castlingInfoToFEN(board.castlingInfo);
}