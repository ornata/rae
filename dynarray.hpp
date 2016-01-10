#ifndef DYN_H
#define DYN_H

#define BASE_SIZE 4
#define GROWTH_FACTOR 2

template <typename T>
struct dynArray
{
    T* array;
    int numItems;
    int maxSize;

    dynArray();
    dynArray(int initLen);
    ~dynArray();

    bool append(T item);
    bool truncate();
    void clear() { numItems = 0; }
    bool isEmpty() {return numItems == 0;}

    const T &operator [](int i) { return array[i]; }
};

#endif