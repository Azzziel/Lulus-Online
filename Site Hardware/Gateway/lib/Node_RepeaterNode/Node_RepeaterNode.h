#ifndef NODE_REPEATER_NODE_h
#define NODE_REPEATER_NODE_h

#include <Arduino.h>
#include <HexConverter.h>

class Node_RepeaterNode
{
public:
    Node_RepeaterNode() { ++totalNumberOfRepeaterObjects; }
    ~Node_RepeaterNode() { --totalNumberOfRepeaterObjects; }

    void begin(const String &id, const String &route)
    {
        begin(HexConverter::hexStringToUInt(id), HexConverter::hexStringToUInt(route));
    }

    void begin(const unsigned int id, const unsigned int route);
    void end();

    const bool operator==(const unsigned int id) const { return id == getRepeaterID(); }
    const bool operator==(const String &id) const { return operator==(HexConverter::hexStringToUInt(id)); }

    const unsigned int getRepeaterID() const { return repeaterID; }
    const String getRepeaterIDInHexString() const { return HexConverter::UIntToHexString(repeaterID, 4); }

    const unsigned int getRouteID() const { return repeaterRoute; }
    const String getRouteIDInHexString() const { return HexConverter::UIntToHexString(repeaterRoute, 4); }

    void printTable();

    static void printTableHeader();

    static const unsigned int getTotalNumberOfRepeaters() { return totalNumberOfRepeaters; }
    static const unsigned int getTotalNumberOfRepeaterObjects() { return totalNumberOfRepeaterObjects; }

private:
    static unsigned int totalNumberOfRepeaters;
    static unsigned int totalNumberOfRepeaterObjects;

    unsigned int repeaterID{};
    unsigned int repeaterRoute{};

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