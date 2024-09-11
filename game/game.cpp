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

// TODO decidere se usare i puntatori per le move o i reference

Game::MoveResult* Game::finalizeMove(Move& move) {
	Piece piece = rawMove(move.getSourcePosition(), move.getDestinationPosition(), move.getPiece());
	updateKingPosition(move);
	updateCastlingInfo(move);
	updateEnPassantInfo(move);

	if (move.isCastling()) {
		completeCastlingMove(move);
	}
	else if (move.isPawnPromotion()) {
		completePawnPromotion(move);
	}
	else if (move.isEnPassant()) {
		piece = completeEnPassant(move);
	}

	move.setCaptured(processCapture(piece, move.isWhite()));

	if (!move.isCaptured() && !move.isPawnPromotion() && !isPawn(move.getDestinationPosition())) {
		halfMoveClock++;
	}
	else {
		halfMoveClock = 0;
	}

	return new MoveResult(move.isCaptured(), move.isPawnPromotion(), move.isEnPassant(), move.isCastling());
}

const Game::MoveResult* Game::applyMove(Move& move) {
	const MoveResult* moveResult = finalizeMove(move);
	lastMove = &move;
	movesHistory.push_front(&move);
	checkStatus.reset();

	if (!move.isWhite()) {
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

void Game::simulateMove(const Move& move) {
	rawMove(move.getSourcePosition(), move.getDestinationPosition(), move.getPiece());
	updateKingPosition(move);

	if (move.isCastling()) {
		completeCastlingMove(move);
	}
	else if (move.isPawnPromotion()) {
		completePawnPromotion(move);
	}
	else if (move.isEnPassant()) {
		completeEnPassant(move);
	}
}

void Game::verifyChecks() {
	Positions::calculateCheckPositions(*this, !isWhiteToMove());
	const Position kingPosition = isWhiteToMove() ? whiteKingPosition : blackKingPosition;
	Move* move = movesHistory.empty() ? nullptr : movesHistory.front();
	checkStatus.updateStatus(kingPosition, move);
}

EndGameType Game::checkEndGame() {
	if (nextMoves.empty()) {
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
	const Rawboard checkBoard = move.isWhite() ? board.getBlackAttacks() : board.getWhiteAttacks();
	const Position kingPosition = move.isWhite() ? whiteKingPosition : blackKingPosition;

	if (BoardUtils::isUnderCheck(checkBoard, kingPosition)) {
		return false;
	}

	bool castlingNotValid = false;

	if (move.isCastling()) {
		if (!move.isWhite()) {
			switch (move.getDestinationPosition()) {
				case 2: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 4) || BoardUtils::isUnderCheck(checkBoard, 3); break;
				case 6: castlingNotValid = BoardUtils::isUnderCheck(checkBoard, 4) || BoardUtils::isUnderCheck(checkBoard, 5); break;
				default: break;
			}
		}
		else {
			switch (move.getDestinationPosition()) {
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
	if (PieceHelper::isKing(move.getPiece())) {
		if (move.isWhite()) {
			whiteKingPosition = move.getDestinationPosition();
		}
		else {
			blackKingPosition = move.getDestinationPosition();
		}
	}
}

void Game::updateEnPassantInfo(const Move& move) {
	if (PieceHelper::isPawn(move.getPiece()) &&
		Positions::isSecondRow(move.getSourcePosition(), move.isWhite()) &&
		Positions::isFourthRow(move.getDestinationPosition(), move.isWhite())) {
		enPassantPosition = move.getSourcePosition() + (move.isWhite() ? -8 : 8);
	}
	else {
		enPassantPosition = NO_POS;
	}
}

void Game::updateCastlingInfo(const Move& move) {
	castlingInfo = CastlingHelper::update(castlingInfo, move);
}

void Game::completeCastlingMove(const Move& move) {
	switch (move.getDestinationPosition()) {
		case 2: rawMove(0, 3, BRook); break;
		case 6: rawMove(7, 5, BRook); break;
		case 58: rawMove(56, 59, WRook); break;
		case 62: rawMove(63, 61, WRook); break;
		default: break;
	}
}

Piece Game::completeEnPassant(const Move& move) {
	const Position destination = move.getDestinationPosition() + (move.isWhite() ? 8 : -8);
	return setEmptyPiece(destination);
}

void Game::completePawnPromotion(const Move& move) {
	if (move.getPromotion() == Empty) {
		// TODO tirare eccezione
		//throw new RuntimeException("Pawn promotion piece type not set");
	}

	//System.out.print("Pawn promotion --> " + move.getPieceTypeForPawnPromotion().name());
	setPiece(move.getDestinationPosition(), move.getPromotion());
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
	return move.isWhite() == isWhiteToMove();
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
	delete lastMove;
	lastMove = movesHistory.empty() ? nullptr : movesHistory.front();
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

void Game::setLastMove(Move* move) {
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
