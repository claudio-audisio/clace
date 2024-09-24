#pragma once

#include <unordered_map>

#include "../common/types.h"

using namespace std;


class CheckStatus {
public:
	CheckStatus();
	~CheckStatus();

    void updateAllCheckPositions(const Rawboard newPositions);
    void addCheckPosition(const Position position, const Rawboard newPositions);
    void addXRayPosition(const Position position, const Rawboard newPositions);
    void updateStatus(const Position kingPosition, const Move move);
    void reset();
    void set(const CheckStatus& checkStatus);

    const Rawboard getAllCheckPositions() const {
        return allCheckPositions;
    }

    const unordered_map<Position, Rawboard>& getCheckPositions() const {
        return checkPositions;
    }

    const unordered_map<Position, Rawboard>& getXRayPositions() const {
        return xRayPositions;
    }

    const bool isCheck() const {
        return check;
    }

    const bool isDoubleCheck() const {
        return doubleCheck;
    }

    const bool isDiscoveryCheck() const {
        return discoveryCheck;
    }

    const bool isCheckmate() const {
        return checkmate;
    }

private:
    Rawboard allCheckPositions;                          // all positions under check
    unordered_map<Position, Rawboard> checkPositions;    // positions under check by piece position
    unordered_map<Position, Rawboard> xRayPositions;     // xray positions under check by piece position
    bool check;
    bool discoveryCheck;
    bool doubleCheck;
    bool checkmate;

    bool isDiscoveryCheck(const Position kingPosition, const Move lastMove) const;
    void adjustChecks();

};
