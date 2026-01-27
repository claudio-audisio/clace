#pragma once

#include <cstring>
#include <stack>

#include "../common/constants.h"

using namespace std;

template <typename T>
class ArrayPool {

    stack<T*> pool;
    unsigned int size;
    unsigned int arraySize;
    unsigned int incrementSize;

public:

    ArrayPool(const unsigned int poolSize, const unsigned int arraySize, const unsigned int incrementSize) {
        this->size = poolSize;
        this->arraySize = arraySize;
        this->incrementSize = incrementSize;

        for (unsigned int i = 0; i < poolSize; i++) {
            pool.push(new T[arraySize]);
        }
    }

    ArrayPool(const unsigned int poolSize, const unsigned int arraySize) :
        ArrayPool(poolSize, arraySize, max(static_cast<int>(poolSize * POOL_INCREMENT_RATIO), 1)) {
    }

    ~ArrayPool() noexcept(false) {
        if (occupancy() > 0) {
            throw runtime_error("memory leak detected on pool destructor");
        }

        while (pool.size() > 0) {
            delete[] pool.top();
            pool.pop();
        }
    }

    T* getArray() {
        if (!pool.empty()) {
            T *array = pool.top();
            pool.pop();
            return array;
        } else {
            for (int i = 0; i < incrementSize; i++) {
                pool.push(new T[arraySize]);
            }

            size += incrementSize;
            return getArray();
        }
    }

    void release(T *array) {
        memset(array, 0, sizeof(T) * arraySize);
        pool.push(array);
    }

    unsigned int capacitiy() {
        return pool.size();
    }

    unsigned int occupancy() {
        return size - pool.size();
    }

    // only for testing
    unsigned int _incrementSize() const {
        return incrementSize;
    }

};