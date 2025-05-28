#include "checkStatus.h"
#include "../utils/boardUtils.h"
#include "../move/move.h"


CheckStatus::CheckStatus() {
	check = discoveryCheck = doubleCheck = checkmate = false;
	allCheckPositions = 0;
    checkPositions.reserve(64);
    xRayPositions.reserve(64);
}

CheckStatus::~CheckStatus() {

}

void CheckStatus::updateAllCheckPositions(const Rawboard newPositions) {
    allCheckPositions |= newPositions;
}

void CheckStatus::addCheckPosition(const Position position, const Rawboard newPositions) {
    checkPositions.emplace(position, newPositions);
}

void CheckStatus::addXRayPosition(const Position position, const Rawboard newPositions) {
    checkPositions.emplace(position, newPositions);
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
    if (MoveHelper::isEnPassant(lastMove) && MoveHelper::isCaptured(lastMove)) {
        for (const pair<const Position, Rawboard>& checkPos : checkPositions) {
            if (checkPos.first != MoveHelper::getDestinationPosition(lastMove) && BoardUtils::isUnderCheck(checkPos.second, kingPosition)) {
                return true;
            }
        }
    }
    else {
        for (const pair<const Position, Rawboard>& checkPos : xRayPositions) {
            if (checkPos.first != MoveHelper::getDestinationPosition(lastMove) && BoardUtils::isUnderCheck(checkPos.second, kingPosition)) {
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
    check = checkStatus.check;
    discoveryCheck = checkStatus.discoveryCheck;
    doubleCheck = checkStatus.doubleCheck;
    checkmate = checkStatus.checkmate;
    allCheckPositions = checkStatus.allCheckPositions;
    checkPositions = checkStatus.checkPositions;
    xRayPositions = checkStatus.xRayPositions;
}
