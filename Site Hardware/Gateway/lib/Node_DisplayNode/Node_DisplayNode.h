#ifndef NODE_DISPLAY_NODE_h
#define NODE_DISPLAY_NODE_h

#include <Arduino.h>
#include <HexConverter.h>

class Node_DisplayNode
{
public:
    Node_DisplayNode() { ++totalNumberOfDisplayObjects; }
    ~Node_DisplayNode();

    void begin(const String &id, const String &route, const bool isMain) { begin(HexConverter::toUShort(id), route, isMain); }
    void begin(const unsigned short id, const String &route, const bool isMain);
    void end();

    const bool operator==(const unsigned short id) const { return id == getDisplayID(); }
    const bool operator==(const String &id) const { return operator==(HexConverter::toUShort(id)); }

    const unsigned short getDisplayID() const { return displayID; }
    const String getDisplayIDInHexString() const { return HexConverter::toString(getDisplayID(), 4); }

    void setDisplayRoute(const String &route);
    void unsetDisplayRoute();

    const String &getDisplayRoute();

    bool isMain() { return isTheMainDisplay; }

    void setDisplayValue(const unsigned short value) { displayValue = value; }
    const unsigned short getDisplayValue() const { return displayValue; }

    void printTable();

    static void printTableHeader();

    static const unsigned int getTotalNumberOfDisplays() { return totalNumberOfDisplays; }
    static const unsigned int getTotalNumberOfDisplayObjects() { return totalNumberOfDisplayObjects; }

private:
    static unsigned int totalNumberOfDisplays;
    static unsigned int totalNumberOfDisplayObjects;

    unsigned short displayID{};
    const String *displayRoute{};
    bool isTheMainDisplay{};

    unsigned short displayValue{};

    // ---------------------------------------------------------------------------------------------------

public:
    static void setPointerToHome() { pointer = 0U; }
    static void setPointerToEnd();

    static const unsigned int preincrementPointer();
    static const unsigned int postincrementPointer();

    static const unsigned int predecrementPointer();
    static const unsigned int postdecrementPointer();

    static const unsigned int getPointer() { return pointer; }

private:
    static unsigned int pointer;
};

#endif