#include <Arduino.h>
#include "PayloadHandler.h"

void PayloadHandler::loadPayload(const String *const payload)
{
    unloadPayload();

    countPayloadSize(payload);

    payloadArray = new String[getPayloadSize()];

    size_t payloadArrayIndex = 0;
    for (size_t payloadIndex = 0; payloadIndex < payload->length(); ++payloadIndex)
    {
        char payloadIndexedCharacter = payload->operator[](payloadIndex);

        if (payloadIndexedCharacter == getPayloadSeparator())
        {
            ++payloadArrayIndex;
        }
        else
        {
            payloadArray[payloadArrayIndex] += payloadIndexedCharacter;
        }
    }
}

unsigned int PayloadHandler::countSeparator(const String *const payload) const
{
    unsigned int separatorCount = 0;

    for (size_t payloadIndex = 0; payloadIndex < payload->length(); ++payloadIndex)
    {
        if (payload->operator[](payloadIndex) == getPayloadSeparator())
        {
            ++separatorCount;
        }
    }

    return separatorCount;
}

const String &PayloadHandler::operator[](unsigned int index) const
{
    static const String PAYLOAD_NOT_LOADED PROGMEM{"PAYLOAD_NOT_LOADED"};
    static const String ARRAY_OUT_OF_BOUNDS PROGMEM{"ARRAY_OUT_OF_BOUNDS"};

    if (payloadArray == nullptr)
    {
        return PAYLOAD_NOT_LOADED;
    }

    if (index >= getPayloadSize())
    {
        return ARRAY_OUT_OF_BOUNDS;
    }

    return payloadArray[index];
}

void PayloadHandler::unloadPayload()
{
    payloadSize = 0;

    delete[] payloadArray;
    payloadArray = nullptr;
}