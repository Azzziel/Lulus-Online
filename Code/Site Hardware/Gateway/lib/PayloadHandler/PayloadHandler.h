#ifndef PAYLOAD_HANDLER_h
#define PAYLOAD_HANDLER_h

#include <Arduino.h>

class PayloadHandler
{
public:
    explicit PayloadHandler(const char separator);
    ~PayloadHandler();

    inline bool isSet() const
    {
        return !isEmpty();
    }

    inline bool isEmpty() const
    {
        return payloadArray == nullptr;
    }

    void loadPayload(const String *const payload);

    inline unsigned int getPayloadSize() const
    {
        return payloadSize;
    }

    void loadPayloadSize(const String *const payload);

    unsigned int countSeparator(const String *const payload) const;

    inline char getPayloadSeparator() const
    {
        return payloadSeparator;
    }

    void unloadPayload();

    const String &operator[](unsigned int index) const;

private:
    const char payloadSeparator;

    unsigned int payloadSize = 0;
    String *payloadArray = nullptr;
};

#endif