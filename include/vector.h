//
// Created by Brouse on 23/01/2026.
//

#ifndef VECTOR_H
#define VECTOR_H

template<typename T, size_t N>
class StaticVector {
public:
    StaticVector() : _size(0) {}

    bool push_back(const T& value) {
        if (_size >= N) return false;
        _data[_size++] = value;
        return true;
    }

    bool pop_back(T &out) {
        if (_size == 0) return false;
        _size--;
        out = _data[_size];
        return true;
    }

    T& operator[](size_t index) {
        return _data[index];
    }

    const T& operator[](size_t index) const {
        return _data[index];
    }

    // Iterators
    T* begin() { return _data; }
    T* end() { return _data + _size; }
    const T* begin() const { return _data; }
    const T* end()   const { return _data + _size; }

    // Functions
    size_t size() const { return _size; }
    size_t capacity() const { return N; }
    bool empty() const { return _size == 0; }
    bool full() const { return _size == N; }
    void clear() { _size = 0; }

private:
    T _data[N];
    size_t _size;
};

#endif //VECTOR_H