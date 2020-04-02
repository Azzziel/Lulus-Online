#ifndef HEX_CONVERTER_h
#define HEX_CONVERTER_h

#include <Arduino.h>

// Member functions of this class doesn't deal with 64-bit long long long data type
class HexConverter
{
public:
    HexConverter() = delete;

    static const unsigned int MAXIMUM_HEX_DIGIT = 8;

    static const uint32_t hexStringToUInt(String hexString);
    static const String UIntToHexString(const uint32_t integer, unsigned int totalDigit = 1U);
    static const String UIntToHexStringWithLiteral(const uint32_t integer, const unsigned int totalDigit = 1U)
    {
        return "0x" + UIntToHexString(integer, totalDigit);
    }
};

#endif