#pragma once

#include <cstring>
#include "../common/constants.h"

using namespace std;

class MovePool {
public:
    explicit MovePool(const unsigned int size) {
        this->size = size;
        pool = new Move*[size];

        for (unsigned int index = 0; index < size; index++) {
            pool[index] = new Move[MAX_MOVES];
        }
    }

    ~MovePool() {
        for (unsigned int index = 0; index < size; index++) {
            delete pool[index];
        }

        delete pool;
    }

    Move* getArray(const unsigned int index = 0) const {
        memset(pool[index], 0, sizeof(Move) * MAX_MOVES);
        return pool[index];
    }

    unsigned int size;
    Move** pool;
};