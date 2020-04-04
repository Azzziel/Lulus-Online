#ifndef BERAS_STAPLE_DIVIDER_h
#define BERAS_STAPLE_DIVIDER_h

#include <Arduino.h>

class Divider
{
public:
    ~Divider() { clearArray(); }

    void loadDivider(const unsigned int amountToBeDivided, const unsigned int divider);

    void printArray() const;

    const unsigned int getArraySize() const { return arraySize; }

    const unsigned int operator[](const unsigned int index) const;

    void clearArray();

private:
    unsigned int arraySize = 0;
    unsigned int *array = nullptr;
};

#endif