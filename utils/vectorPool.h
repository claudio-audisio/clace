#pragma once

#include <vector>

#include "../move/move.h"

using namespace std;

class VectorPool {
public:
    VectorPool(unsigned int size) {
        this->size = size;
        pool = new vector<vector<Move>*>(size);

        for (unsigned int index = 0; index < size; index++) {
            (*pool)[index] = new vector<Move>(218);
        }
    }

    ~VectorPool() {
        for (unsigned int index = 0; index < size; index++) {
            delete (*pool)[index];
        }

        delete pool;
    }

    vector<Move>& getVector(unsigned int index) {
        vector<Move>& moves = *(*pool)[index];
        moves.clear();
        return moves;
    }

private:
    unsigned int size;
    vector<vector<Move>*>* pool;
};