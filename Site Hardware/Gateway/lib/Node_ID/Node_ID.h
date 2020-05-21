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
        uint32_t location_id;
        uint16_t node_id;
    };

public:
    const bool loadIDFromEEPROM(const unsigned int saveAddress, const char idSignature);

    const uint32_t getLocationID() const { return ID.location_id; }

    const uint16_t getNodeID() const { return ID.node_id; }
    const String getNodeIDInHexString() const { return HexConverter::toString(getNodeID(), 4); }

    const char getIDSignature() const { return ID.signature; }

    // --------------------------------------------------------
    /* 
     * WARNING!!! DANGEROUS IF CONTINUOUSLY CALLED
     * Use only in void setup() and when doing an initial setup
     */
    const ID_t putNewIDToEEPROM(const uint32_t saveAddress, const ID_t id) const { return EEPROM.put<ID_t>(saveAddress, id); }
    // --------------------------------------------------------

private:
    ID_t ID{};
};

#endif