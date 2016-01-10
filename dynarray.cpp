#include "dynarray.hpp"

template <typename T>
dynArray<T>::dynArray()
{
    numItems = 0;
    maxSize = BASE_SIZE;
    array = new T[maxSize];
}

template <typename T>
dynArray<T>::dynArray(int initLen)
{
    numItems = 0;
    maxSize = initLen;
    array = new T[initLen];
}

template <typename T>
dynArray<T>::~dynArray()
{
    numItems = 0;
    delete[] array;
}

template <typename T>
bool dynArray<T>::truncate()
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
bool dynArray<T>::append(T newItem)
{
    if (numItems == maxSize) {
        maxSize *= GROWTH_FACTOR;
        T* tmp = array;

        // make sure we can get memory!
        if (!(array = new T[maxSize])) {
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