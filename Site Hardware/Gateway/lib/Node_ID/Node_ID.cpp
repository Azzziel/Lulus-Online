#include <Arduino.h>
#include "Node_ID.h"

#include <EEPROM.h>

const bool Node_ID::loadIDFromEEPROM(const unsigned int saveAddress, const char idSignature)
{
    ID_t payload;
    EEPROM.get<ID_t>(saveAddress, payload);

    if (idSignature == payload.signature)
    {
        ID.signature = payload.signature;
        ID.location_id = payload.location_id;
        ID.node_id = payload.node_id;

        return true;
    }
    else
    {
        Serial.println(F("[M][E] Corrupt EEPROM signature. Abort"));
        return false;
    }
}