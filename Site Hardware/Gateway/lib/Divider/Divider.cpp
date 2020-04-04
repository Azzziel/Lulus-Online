#include <Arduino.h>
#include "Divider.h"

void Divider::loadDivider(const unsigned int amount, const unsigned int divider)
{
    if (divider == 0)
        return;

    clearArray();

    this->amount = amount;
    this->divider = divider;

    if (amount % divider == 0)
    {
        arraySize = amount / divider;
    }
    else
    {
        arraySize = (amount / divider) + 1;
    }

    array = new unsigned int[getArraySize()];

    if (getArraySize() == 1)
    {
        array[0] = amount;
    }
    else
    {
        if (amount % divider == 0)
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
                if (index >= getArraySize() - 1)
                {
                    array[index] = amount % divider;
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
    Serial.print("[D] ");
    Serial.print(amount);
    Serial.print(" / ");
    Serial.print(divider);
    Serial.println();
    
    Serial.print("[A] ");

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
    amount = 0;
    divider = 0;

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