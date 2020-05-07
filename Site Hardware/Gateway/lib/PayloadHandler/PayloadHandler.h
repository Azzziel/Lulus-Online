#ifndef PAYLOAD_HANDLER_h
#define PAYLOAD_HANDLER_h

#include <Arduino.h>

class PayloadHandler
{
public:
    explicit PayloadHandler(const char separator) : payloadSeparator{separator} {}
    ~PayloadHandler() { unloadPayload(); }

    bool isSet() const { return !isEmpty(); }
    bool isEmpty() const { return payloadArray == nullptr; }

    void loadPayload(const String *const payload);
    void unloadPayload();

    void countPayloadSize(const String *const payload) { payloadSize = countSeparator(payload) + 1; }
    unsigned int getPayloadSize() const { return payloadSize; }

    unsigned int countSeparator(const String *const payload) const;
    char getPayloadSeparator() const { return payloadSeparator; }

    const String &operator[](unsigned int index) const;

private:
    const char payloadSeparator;

    unsigned int payloadSize = 0;
    String *payloadArray = nullptr;
};

#endif