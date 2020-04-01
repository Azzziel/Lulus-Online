#include <Arduino.h>
#include "HexConverter.h"

const uint32_t HexConverter::hexStringToUInt(String hexString)
{
    // Removes "0x" in the beginning of the string, if any
    if (hexString.startsWith("0x"))
    {
        hexString.remove(0, 2);
    }

    // Trims unnecessary '0' characters in the beginning of the string
    while (hexString[0] == '0')
    {
        if (hexString.length() > 1)
        {
            hexString.remove(0, 1);
        }
        else // Aborts when the entire leftover string contains only a single '0' character
        {
            return 0;
        }
    }

    hexString.toUpperCase();

    // 9 or more hexadecimal digits will overflow the data type, abort
    if (hexString.length() > MAXIMUM_HEX_DIGIT)
    {
        return 0;
    }

    uint32_t returnValue{};
    for (size_t index = hexString.length() - 1, exponent = 0; index < hexString.length(); --index, ++exponent)
    {
        char c = hexString[index];

        if (c >= '0' && c <= '9')
        {
            // Convertion to a real decimal from an ASCII decimal
            c -= '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            // Convertion to a real decimal from an ASCII decimal
            c -= 'A';
            c += 10; // Makes 'A' represent 10, not 0
        }
        else // Not a HEX, abort the entire thing
        {
            return 0;
        }

        // Please use your brain for this
        returnValue += c * pow(16, exponent);
    }

    return returnValue;
}

const String HexConverter::UIntToHexString(const uint32_t integer, unsigned int totalDigit)
{
    if (!totalDigit)
    {
        totalDigit = 1;
    }
    
    String s;

    bool hasReachedSignificantDigit = false;
    for (size_t exponent = MAXIMUM_HEX_DIGIT - 1, iteration = 0; exponent < MAXIMUM_HEX_DIGIT; --exponent, ++iteration)
    {
        uint32_t filteredHexDigit;

        // Example of what actually happens: 0xABCD & 0x0F00 == 0x0B00, similar thing to B10100110 & B11110000 == B10100000 ($data AND $filter == $filtered_data)
        filteredHexDigit = integer & static_cast<uint32_t>((pow(16, exponent + 1) - 1) - (pow(16, exponent) - 1));

        // Assumes significant digit has been found when it reached the total number of digits specified by the function argument
        if (totalDigit >= MAXIMUM_HEX_DIGIT - iteration)
        {
            hasReachedSignificantDigit = true;
        }

        // Skips adding '0' to the beginning of the string unless specified by the function argument
        if (!hasReachedSignificantDigit)
        {
            if (!filteredHexDigit)
            {
                continue;
            }
            else
            {
                hasReachedSignificantDigit = true;
            }
        }

        // Narrowing the filtered hex digit to an integer that values from 0 to 15
        filteredHexDigit /= pow(16, exponent);

        // Convertion to ASCII decimal
        if (filteredHexDigit >= 10)
        {
            filteredHexDigit -= 10;
            filteredHexDigit += 'A';
        }
        else
        {
            filteredHexDigit += '0';
        }

        s += static_cast<char>(filteredHexDigit);
    }

    return s;
}