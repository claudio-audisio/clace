#pragma once

#include <vector>

#include "../common/constants.h"

using namespace std;

template<class T>
class VectorPool {
public:
    VectorPool(unsigned int size, unsigned int vectorSize) {
        this->size = size;
        pool = new vector<vector<T>*>(size);

        for (unsigned int index = 0; index < size; index++) {
            (*pool)[index] = new vector<T>(vectorSize);
        }
    }

    ~VectorPool() {
        for (unsigned int index = 0; index < size; index++) {
            delete (*pool)[index];
        }

        delete pool;
    }

    vector<T>& getVector(unsigned int index) {
        vector<T>& v = *(*pool)[index];
        v.clear();
        return v;
    }

    unsigned int size;
    vector<vector<T>*>* pool;
};