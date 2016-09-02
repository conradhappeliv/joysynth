#ifndef JOYSYNTH2_RTARRAY_H
#define JOYSYNTH2_RTARRAY_H

#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>

template <class T>
class RTArray {
public:
    RTArray() {};
    RTArray(size_t newsize) { resize(newsize); }
    RTArray(const RTArray& old) {
        s = old.s;
        data = new T[s];
        for(size_t i = 0; i < s; i++) data[i] = old.data[i];
    }
    ~RTArray() { if(data != nullptr) delete[] data; }
    void zero() { for(size_t i = 0; i < s; i++) data[i] = 0; }
    void resize(size_t newsize) {
        T* newdata = new T[newsize];
        if(data != nullptr) {
            for (size_t i = 0; i < std::min(s, newsize); i++) {
                newdata[i] = data[i];
            }
            delete[] data;
        }
        data = newdata;
        s = newsize;
    }
    T& operator[](size_t i) const {
        if(i < s) return data[i];
        throw std::out_of_range("out of range");
    }
    size_t size() const { return s; }
private:
    T* data = nullptr;
    size_t s = 0;
};


#endif //JOYSYNTH2_RTARRAY_H
