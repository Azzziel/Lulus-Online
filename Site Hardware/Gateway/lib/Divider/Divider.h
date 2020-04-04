#ifndef DIVIDER_h
#define DIVIDER_h

#include <Arduino.h>

class Divider
{
public:
    ~Divider() { clearArray(); }

    void loadDivider(const unsigned int amount, const unsigned int divider);

    const unsigned int getAmount() const { return amount; }
    const unsigned int getDivider() const { return divider; }

    const unsigned int getArraySize() const { return arraySize; }

    const unsigned int operator[](const unsigned int index) const;

    void printArray() const;

    void clearArray();

private:
    unsigned int amount;
    unsigned int divider;

    unsigned int arraySize = 0;
    unsigned int *array = nullptr;
};

#endif