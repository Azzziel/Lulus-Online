#ifndef NODE_ID_h
#define NODE_ID_h

#include <Arduino.h>
#include <EEPROM.h>

#include <HexConverter.h>

class Node_ID
{
public:
    struct ID_t
    {
        char signature;
        uint16_t data;
    };

public:
    const bool loadIDFromEEPROM(const unsigned int saveAddress, const char idSignature);

    const uint16_t getID() const { return ID.data; }
    const String getIDInHexString() const { return HexConverter::UIntToHexString(getID()); }

    const char getIDSignature() const { return ID.signature; }

    // --------------------------------------------------------
    /* 
     * WARNING!!! DANGEROUS OPERATION
     * Use only in void setup() and when doing an initial setup
     */
    const ID_t putNewIDToEEPROM(const uint32_t saveAddress, const ID_t id) const { return EEPROM.put<ID_t>(saveAddress, id); }
    // --------------------------------------------------------

private:
    ID_t ID{};
};

#endif