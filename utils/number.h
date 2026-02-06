#pragma once
#include <cmath>

#include "common/constants.h"

using namespace std;

inline bool isPositive(const double a) {
    return a > 0 + EPSILON;
}

inline bool areEqual(const double a, const double b) {
    return abs(a - b) <= EPSILON;
}

inline bool areNotEqual(const double a, const double b) {
    return abs(a - b) > EPSILON;
}