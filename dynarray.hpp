/*
* dynarray.hpp
* Contains the implementation of a dynamic array type,
* just in case std::vector<T> is too slow.
*/

#ifndef DYN_H
#define DYN_H

#include <iostream>

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

template <typename T>
std::ostream &operator <<(std::ostream &out, dynArray<T> &toString);

template <typename T>
inline dynArray<T>::dynArray()
{
    numItems = 0;
    maxSize = BASE_SIZE;
    array = new T[maxSize];
}

template <typename T>
inline dynArray<T>::dynArray(int initLen)
{
    numItems = 0;
    maxSize = initLen;
    array = new T[initLen];
}

template <typename T>
inline dynArray<T>::~dynArray()
{
    numItems = 0;
    delete[] array;
}

template <typename T>
inline bool dynArray<T>::truncate()
{
    if (numItems < maxSize)
    {
        T* tmp = array;
        maxSize = numItems;

        if (!(array = new T[numItems])) {
            return false;
        }

        for (int i = 0; i < numItems; i++) {
            array[i] = tmp[i];
        }

        delete[] tmp;
    }

    return true;
}

template <typename T>
inline bool dynArray<T>::append(T newItem)
{
    if (numItems == maxSize) {
        maxSize *= GROWTH_FACTOR;
        T* tmp = array;

        // make sure we can get memory!
        if (!(array = new T[maxSize])) {
            std::cout << "Could not allocate memory for array.\n";
            return false;
        }

        for (int i = 0; i < numItems; i++) {
            array[i] = tmp[i];
        }

        delete[] tmp;
    }

    // finally, append the item to the array
    array[numItems++] = newItem;
    return true;
}

template <typename T>
inline std::ostream &operator <<(std::ostream &out, dynArray<T> &toString)
{
    for (int i = 0; i < toString.numItems; i++) {
        out << toString.array[i] << " ";
    }

    return out;
}

#endif