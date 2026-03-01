#pragma once
#include <cmath>
#include <raylib.h>


inline double distance(const Vector2 start, const Vector2 end) {
    return std::hypot(start.x - end.x, start.y - end.y);
}