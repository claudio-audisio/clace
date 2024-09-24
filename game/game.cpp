#include "game.h"
#include "../utils/pieceHelper.h"
#include "../utils/castlingHelper.h"
#include "../utils/fen.h"
#include "../utils/utils.h"


Game::Game() {
}

Game::~Game() {
}

void Game::init() {
	initPlayers();
	initFromFEN(Positions::INITIAL_FEN_POSITION);	
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

Piece Game::rawMove(const Position source, const Position destination, const Piece piece) {
	return board.move(source, destination, piece);
}

MoveResult Game::finalizeMove(Move& move) {
	Piece captured = rawMove(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move), MoveHelper::getPiece(move));
	updateKingPosition(move);
	updateCastlingInfo(move);
	updateEnPassantInfo(move);

    if (MoveHelper::isPawnPromotion(move)) {
        completePawnPromotion(move);
        // TODO rimuovere pawn promosso
    }

    if (captured == Empty) {
        if (MoveHelper::isCastling(move)) {
            completeCastlingMove(move);
        }
        else if (MoveHelper::isEnPassant(move)) {
            captured = completeEnPassant(move);
            // TODO rimuovere pawn avversario
        }
    }

    if (captured) {
        MoveHelper::setCaptured(move, captured);
        processCapture(captured, MoveHelper::isWhite(move));
    }

	if (captured == Empty && !MoveHelper::isPawnPromotion(move) && !isPawn(MoveHelper::getDestinationPosition(move))) {
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
	checkStatus.reset();

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
	Piece captured = rawMove(MoveHelper::getSourcePosition(move), MoveHelper::getDestinationPosition(move), MoveHelper::getPiece(move));
	updateKingPosition(move);

    if (MoveHelper::isPawnPromotion(move)) {
        completePawnPromotion(move);
    }

    if (captured == Empty) {
        if (MoveHelper::isCastling(move)) {
            completeCastlingMove(move);
        }
        else if (MoveHelper::isEnPassant(move)) {
            captured = completeEnPassant(move);
        }
    }

    MoveHelper::setCaptured(move, captured);
}

void Game::undoSimulateMove(Move& move) {
    const Piece piece = MoveHelper::getPiece(move);
    rawMove(MoveHelper::getDestinationPosition(move), MoveHelper::getSourcePosition(move), piece);
    const Piece captured = MoveHelper::getCaptured(move);

    if (captured != Empty) {
        if (MoveHelper::isEnPassant(move)) {
            undoEnPassant(move);
        } else {
            board.setPiece(MoveHelper::getDestinationPosition(move), captured);
        }
    } else {
        if (MoveHelper::isCastling(move)) {
            undoCastlingMove(move);
        }
    }

    // Undo Pawn promotion is not needed because undoing original move we have already moved a Pawn

    undoKingPosition(move);
}

void Game::undoKingPosition(Move& move) {
    if (PieceHelper::isKing(MoveHelper::getPiece(move))) {
        if (MoveHelper::isWhite(move)) {
            whiteKingPosition = MoveHelper::getSourcePosition(move);
        }
        else {
            blackKingPosition = MoveHelper::getSourcePosition(move);
        }
    }
}

void Game::undoCastlingMove(Move& move) {
    switch (MoveHelper::getDestinationPosition(move)) {
        case 2: rawMove(3, 0, BRook); break;
        case 6: rawMove(5, 7, BRook); break;
        case 58: rawMove(59, 56, WRook); break;
        case 62: rawMove(61, 63, WRook); break;
        default: break;
    }
}

void Game::undoEnPassant(Move& move) {
    const Position destination = MoveHelper::getDestinationPosition(move) + (MoveHelper::isWhite(move) ? 8 : -8);
    setPiece(destination, MoveHelper::isWhite(move) ? BPawn : WPawn);
}

void Game::verifyChecks() {
	Positions::calculateCheckPositions(*this, !isWhiteToMove());
	const Position kingPosition = isWhiteToMove() ? whiteKingPosition : blackKingPosition;
	checkStatus.updateStatus(kingPosition, movesHistory.empty() ? 0 : movesHistory.front());
}

EndGameType Game::checkEndGame(const bool noMoves) {
	if (noMoves) {
		return checkStatus.isCheck() ? EndGameType::CHECKMATE : EndGameType::STALEMATE;
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

bool Game::isUnderCheck(const Position position, const bool white) {
	return board.isUnderCheck(position, white);
}

bool Game::checkControl(const Move& move) {
	const bool whiteMove = MoveHelper::isWhite(move);
	const Rawboard checkBoard = whiteMove ? board.getBlackAttacks() : board.getWhiteAttacks();
	const Position kingPosition = whiteMove ? whiteKingPosition : blackKingPosition;

	if (BoardUtils::isUnderCheck(checkBoard, kingPosition)) {
		return false;
	}

	bool castlingNotValid = false;

	if (MoveHelper::isCastling(move)) {
		if (!whiteMove) {
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

void Game::setKingPositions() {
	for (int i = 0; i < 64; i++) {
		if (isKing(i)) {
			if (isWhite(i)) {
				whiteKingPosition = i;
			}
			else {
				blackKingPosition = i;
			}
		}
	}
}

void Game::updateKingPosition(const Move& move) {
	if (PieceHelper::isKing(MoveHelper::getPiece(move))) {
		if (MoveHelper::isWhite(move)) {
			whiteKingPosition = MoveHelper::getDestinationPosition(move);
		}
		else {
			blackKingPosition = MoveHelper::getDestinationPosition(move);
		}
	}
}

void Game::updateEnPassantInfo(const Move& move) {
	if (PieceHelper::isPawn(MoveHelper::getPiece(move)) &&
		Positions::isSecondRow(MoveHelper::getSourcePosition(move), MoveHelper::isWhite(move)) &&
		Positions::isFourthRow(MoveHelper::getDestinationPosition(move), MoveHelper::isWhite(move))) {
		enPassantPosition = MoveHelper::getSourcePosition(move) + (MoveHelper::isWhite(move) ? -8 : 8);
	}
	else {
		enPassantPosition = NO_POS;
	}
}

void Game::updateCastlingInfo(const Move& move) {
	castlingInfo = CastlingHelper::update(castlingInfo, move);
}

void Game::completeCastlingMove(const Move& move) {
	switch (MoveHelper::getDestinationPosition(move)) {
		case 2: rawMove(0, 3, BRook); break;
		case 6: rawMove(7, 5, BRook); break;
		case 58: rawMove(56, 59, WRook); break;
		case 62: rawMove(63, 61, WRook); break;
		default: break;
	}
}

Piece Game::completeEnPassant(const Move& move) {
	const Position destination = MoveHelper::getDestinationPosition(move) + (MoveHelper::isWhite(move) ? 8 : -8);
	return setEmptyPiece(destination);
}

void Game::completePawnPromotion(const Move& move) {
	const Piece promotionPiece = MoveHelper::getPromotion(move);
	if (promotionPiece == Empty) {
		// TODO tirare eccezione
		//throw new RuntimeException("Pawn promotion piece type not set");
	}

	//System.out.print("Pawn promotion --> " + MoveHelper::getPieceTypeForPawnPromotion().name());
	setPiece(MoveHelper::getDestinationPosition(move), promotionPiece);
}

bool Game::processCapture(const Piece piece, const bool white) {
	if (PieceHelper::isEmpty(piece)) {
		return false;
	}

	if (white) {
		blackPlayer->onCaptured(piece);
	}
	else {
		whitePlayer->onCaptured(piece);
	}

	return true;
}

void Game::changeTurn() {
	whiteToMove = !whiteToMove;
}

Piece Game::getPiece(const Position position) const {
	return board.getPiece(position);
}

Piece Game::setPiece(const Position position, const Piece piece) {
	return board.setPiece(position, piece);
}

Piece Game::setEmptyPiece(const Position position) {
	return setPiece(position, Empty);
}

bool Game::checkColor(const Position position) const {
	return checkColor(position, isWhiteToMove());
}

bool Game::checkColor(const Position position, const bool white) const {
	return !board.isEmpty(position) && board.isWhite(position) == white;
}

bool Game::checkColor(const Move& move) const {
	return MoveHelper::isWhite(move) == isWhiteToMove();
}

bool Game::isWhite(const Position position) const {
	return board.isWhite(position);
}
bool Game::isEmpty(const Position position) const {
	return board.isEmpty(position);
}

bool Game::isPawn(const Position position) const {
	return board.isPawn(position);
}

bool Game::isRook(const Position position) const {
	return board.isRook(position);
}

bool Game::isRook(const Position position, bool white) const {
	return board.isRook(position, white);
}

bool Game::isKing(const Position position) const {
	return board.isKing(position);
}

bool Game::isWhiteKingCastling() const {
	return CastlingHelper::isWhiteKingCastling(castlingInfo);
}

bool Game::isWhiteQueenCastling() const {
	return CastlingHelper::isWhiteQueenCastling(castlingInfo);
}

bool Game::isBlackKingCastling() const {
	return CastlingHelper::isBlackKingCastling(castlingInfo);
}

bool Game::isBlackQueenCastling() const {
	return CastlingHelper::isBlackQueenCastling(castlingInfo);
}

void Game::save() {
	rollback.save(*this);
}

void Game::lightSave() {
	rollback.lightSave(*this);
}

void Game::rollbackLastMove() {
	checkStatus.reset();
	movesHistory.pop_front();
	lastMove = movesHistory.empty() ? 0 : movesHistory.front();
	rollback.rollback(*this);
}

void Game::lightRollback() {
	rollback.lightRollback(*this);
}

/*
public Evaluation evaluate(Move move) {
	return Optional.ofNullable(evaluator).map(e->e.evaluate(this, move)).orElseGet(() -> new Evaluation(move, Double.NaN));
}

public double evaluateAsWhite() {
	if (getLastMove() != null) {
		return evaluate(getLastMove()).getValue() * (isWhiteToMove() ? -1 : 1);
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
	
	return player->isComputer();
}

void Game::setLastMove(const Move& move) {
	movesHistory.push_front(move);
	lastMove = move;
}

void Game::setPlayerPieces(const Game& game) {
	whitePlayer->setPieces(game.getWhitePlayer()->getPieces());
	blackPlayer->setPieces(game.getBlackPlayer()->getPieces());
}

void Game::resetPlayersPieces() {
	whitePlayer->resetPieces();
	blackPlayer->resetPieces();
}

void Game::incrementPlayerPieces(const Piece piece) {
	if (whitePlayer) {
		if (PieceHelper::isWhite(piece)) {
			whitePlayer->incrementPieces(piece);
		}
		else {
			blackPlayer->incrementPieces(piece);
		}
	}
}

Game* Game::duplicate() {
	Game* newGame = new Game();
	newGame->init();
	newGame->setBoard(board);
	newGame->setCastlingInfo(castlingInfo);
	newGame->setWhiteToMove(whiteToMove);
	newGame->setWhiteKingPosition(whiteKingPosition);
	newGame->setBlackKingPosition(blackKingPosition);
	newGame->setEnPassantPosition(enPassantPosition);
	newGame->setFullMoves(fullMoves);
	newGame->setHalfMoveClock(halfMoveClock);
	Utils::dequeAddAll(movesHistory, newGame->movesHistory);
	newGame->getCheckStatus().set(checkStatus);
	newGame->setPlayerPieces(*this);
	return newGame;
}

/*
Rawboard Game::getRawBoard() {
	return getRawBoard(isWhiteToMove());
}
*/

Rawboard Game::getRawBoard(const bool white) const {
	return white ? board.WHITE() : board.BLACK();
}