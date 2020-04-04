#include <Arduino.h>
#include "Divider.h"

void Divider::loadDivider(const unsigned int amountToBeDivided, const unsigned int divider)
{
    if (divider == 0)
        return;

    clearArray();

    if (amountToBeDivided % divider == 0)
    {
        arraySize = amountToBeDivided / divider;
    }
    else
    {
        arraySize = (amountToBeDivided / divider) + 1;
    }

    array = new unsigned int[getArraySize()];

    if (getArraySize() == 1)
    {
        array[0] = amountToBeDivided;
    }
    else
    {
        if (amountToBeDivided % divider == 0)
        {
            for (size_t index = 0; index < getArraySize(); ++index)
            {
                array[index] = divider;
            }
        }
        else
        {
            for (size_t index = 0; index < getArraySize(); ++index)
            {
                if (index == getArraySize() - 1)
                {
                    array[index] = amountToBeDivided % divider;
                }
                else
                {
                    array[index] = divider;
                }
            }
        }
    }
}

void Divider::printArray() const
{
    Serial.print(F("[A] "));

    for (size_t index = 0; index < getArraySize(); ++index)
    {
        Serial.print(operator[](index));

        if (index < getArraySize() - 1)
        {
            Serial.print(' ');
        }
    }

    Serial.println();
}

void Divider::clearArray()
{
    arraySize = 0;

    delete[] array;
    array = nullptr;
}

const unsigned int Divider::operator[](const unsigned int index) const
{
    if (array == nullptr || index >= getArraySize())
    {
        return 0;
    }

    return array[index];
}