#pragma once


using namespace std;

template<class T>
class ArrayPool {
public:
    ArrayPool(unsigned int size, unsigned int arraySize = MAX_MOVES) {
        this->size = size;
        this->arraySize = arraySize;
        pool = new T*[size];

        for (unsigned int index = 0; index < size; index++) {
            pool[index] = new T[arraySize];
        }
    }

    ~ArrayPool() {
        for (unsigned int index = 0; index < size; index++) {
            delete pool[index];
        }

        delete pool;
    }

    T* getArray(unsigned int index = 0) {
        memset(pool[index], 0, sizeof(pool[index]));
        return pool[index];
    }

    unsigned int size;
    unsigned int arraySize;
    T** pool;
};