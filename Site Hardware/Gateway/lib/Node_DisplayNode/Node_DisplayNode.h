#ifndef NODE_DISPLAY_NODE_h
#define NODE_DISPLAY_NODE_h

#include <Arduino.h>
#include <HexConverter.h>

class Node_DisplayNode
{
public:
    Node_DisplayNode() { ++totalNumberOfDisplayObjects; }
    ~Node_DisplayNode() { --totalNumberOfDisplayObjects; }

    void begin(const String &id, const String &route)
    {
        begin(HexConverter::hexStringToUInt(id), HexConverter::hexStringToUInt(route));
    }

    void begin(const unsigned int id, const unsigned int route);
    void end();

    const bool operator==(const unsigned int id) const { return id == getDisplayID(); }
    const bool operator==(const String &id) const { return operator==(HexConverter::hexStringToUInt(id)); }

    const unsigned int getDisplayID() const { return displayID; }
    const String getDisplayIDInHexString() const { return HexConverter::UIntToHexString(getDisplayID(), 4); }

    const unsigned int getRouteID() const { return displayRoute; }
    const String getRouteIDInHexString() const { return HexConverter::UIntToHexString(getRouteID(), 4); }

    void printTable();

    static void printTableHeader();

    static const unsigned int getTotalNumberOfDisplays() { return totalNumberOfDisplays; }
    static const unsigned int getTotalNumberOfDisplayObjects() { return totalNumberOfDisplayObjects; }

private:
    static unsigned int totalNumberOfDisplays;
    static unsigned int totalNumberOfDisplayObjects;

    unsigned int displayID{};
    unsigned int displayRoute{};

    // ---------------------------------------------------------------------------------------------------

public:
    static void setPointerToTheFirstDisplay() { pointer = 0U; }
    static void setPointerToTheLastDisplay();

    static const unsigned int preincrementPointer();
    static const unsigned int postincrementPointer();

    static const unsigned int predecrementPointer();
    static const unsigned int postdecrementPointer();

    static const unsigned int getPointer() { return pointer; }

private:
    static unsigned int pointer;
};

#endif