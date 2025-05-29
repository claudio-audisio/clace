#include "checkStatus.h"
#include "../utils/boardUtils.h"
#include "../move/move.h"


CheckStatus::CheckStatus() {
	reset();
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

void CheckStatus::updateStatus(const Position kingPosition, const Move move) {
    if (move) {
        check = BoardUtils::isUnderCheck(checkPositions[MoveHelper::getDestinationPosition(move)], kingPosition);
        discoveryCheck = isDiscoveryCheck(kingPosition, move);
        adjustChecks();
    }
    else {
        check = BoardUtils::isUnderCheck(allCheckPositions, kingPosition);
    }
}

bool CheckStatus::isDiscoveryCheck(const Position kingPosition, const Move& lastMove) const {
    const Position destination = MoveHelper::getDestinationPosition(lastMove);

    if (MoveHelper::isEnPassant(lastMove) && MoveHelper::isCaptured(lastMove)) {
        for (int pos = 0; pos < 64; pos++) {
            if (pos != destination && BoardUtils::isUnderCheck(checkPositions[pos], kingPosition)) {
                return true;
            }
        }
    }
    else {
        for (int pos = 0; pos < 64; pos++) {
            if (pos != destination && BoardUtils::isUnderCheck(xRayPositions[pos], kingPosition)) {
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

    for (int i = 0; i < 64; i++) {
        checkPositions[i] = 0;
        xRayPositions[i] = 0;
    }
}

void CheckStatus::set(const CheckStatus& checkStatus) {
    reset();
    check = checkStatus.check;
    discoveryCheck = checkStatus.discoveryCheck;
    doubleCheck = checkStatus.doubleCheck;
    checkmate = checkStatus.checkmate;
    allCheckPositions = checkStatus.allCheckPositions;

    for (int i = 0; i < 64; i++) {
        checkPositions[i] = checkStatus.checkPositions[i];
        xRayPositions[i] = checkStatus.xRayPositions[i];
    }
}
