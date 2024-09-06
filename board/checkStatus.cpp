#include "checkStatus.h"
#include "../utils/boardUtils.h"

CheckStatus::CheckStatus() {
	check = discoveryCheck = doubleCheck = checkmate = false;
	allCheckPositions = 0;
}

CheckStatus::~CheckStatus() {

}

void CheckStatus::updateAllCheckPositions(const Rawboard newPositions) {
    allCheckPositions |= newPositions;
}

void CheckStatus::addCheckPosition(const Position position, const Rawboard newPositions) {
    checkPositions[position] = newPositions;
}

void CheckStatus::addXRayPosition(const Position position, const Rawboard newPositions) {
    xRayPositions[position] = newPositions;
}

void CheckStatus::updateStatus(const Position kingPosition, Move* move) {
    if (move) {
        check = BoardUtils::isUnderCheck(checkPositions.at(move->getDestinationPosition()), kingPosition);
        discoveryCheck = isDiscoveryCheck(kingPosition, move);
        adjustChecks();
    }
    else {
        check = BoardUtils::isUnderCheck(allCheckPositions, kingPosition);
    }
}

bool CheckStatus::isDiscoveryCheck(const Position kingPosition, const Move* lastMove) const {
    if (lastMove->isEnPassant() && lastMove->isCaptured()) {
        for (const pair<const Position, Rawboard>& checkPos : checkPositions) {
            if (checkPos.first != lastMove->getDestinationPosition() && BoardUtils::isUnderCheck(checkPos.second, kingPosition)) {
                return true;
            }
        }
    }
    else {
        for (const pair<const Position, Rawboard>& checkPos : xRayPositions) {
            if (checkPos.first != lastMove->getDestinationPosition() && BoardUtils::isUnderCheck(checkPos.second, kingPosition)) {
                return true;
            }
        }
    }

    return false;
}

void CheckStatus::adjustChecks() {
    doubleCheck = check && discoveryCheck;
    // adjust counters
    if (discoveryCheck && !check) {
        check = true;
    }

    if (doubleCheck) {
        discoveryCheck = false;
    }
}


void CheckStatus::reset() {
    check = discoveryCheck = doubleCheck = checkmate = false;
    allCheckPositions = 0;
    checkPositions.clear();
    xRayPositions.clear();
}

void CheckStatus::set(const CheckStatus& checkStatus) {
    reset();
    check = checkStatus.isCheck();
    discoveryCheck = checkStatus.isDiscoveryCheck();
    doubleCheck = checkStatus.isDoubleCheck();
    checkmate = checkStatus.isCheckmate();
    allCheckPositions = checkStatus.getAllCheckPositions();
    checkPositions = checkStatus.getCheckPositions();
    xRayPositions = checkStatus.getXRayPositions();
}
