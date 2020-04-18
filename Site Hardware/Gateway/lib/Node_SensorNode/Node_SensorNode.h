#ifndef NODE_SENSOR_NODE_h
#define NODE_SENSOR_NODE_h

#include <Arduino.h>
#include <HexConverter.h>

class Node_SensorNode
{
public:
    Node_SensorNode() { ++totalNumberOfNodeObjects; }
    ~Node_SensorNode();

    void begin(const String &id, const String &display)
    {
        begin(HexConverter::hexStringToUShort(id), HexConverter::hexStringToUShort(display));
    }

    void begin(const unsigned short id, const unsigned short display);
    void end();

    void setNodeStatus(const bool status) { nodeStatus = status; }
    const bool getNodeStatus() const { return nodeStatus; }
    void flipNodeStatus() { setNodeStatus(!getNodeStatus()); }

    void setNodeBattery(const unsigned char battery) { nodeBattery = battery; }
    const unsigned char getNodeBattery() const { return nodeBattery; }

    const bool operator==(const unsigned short id) const { return id == getNodeID(); }
    const bool operator==(const String &id) const { return operator==(HexConverter::hexStringToUShort(id)); }

    const unsigned short getNodeID() const { return nodeID; }
    const String getNodeIDInHexString() const { return HexConverter::UIntToHexString(getNodeID(), 4); }

    const unsigned short getDisplayID() const { return nodeDisplay; }
    const String getDisplayIDInHexString() const { return HexConverter::UIntToHexString(getDisplayID(), 4); }

    void printTable();

    static void printTableHeader();

    static const unsigned int getTotalNumberOfNodes() { return totalNumberOfNodes; }
    static const unsigned int getTotalNumberOfNodeObjects() { return totalNumberOfNodeObjects; }

    enum Keys
    {
        CAR,
        BAT
    };

    static const char *const KEY_CAR; // 0 in enum
    static const char *const KEY_BAT; // 1 in enum

private:
    static unsigned int totalNumberOfNodes;
    static unsigned int totalNumberOfNodeObjects;

    bool nodeStatus{};
    unsigned char nodeBattery{};

    unsigned short nodeID{};
    unsigned short nodeDisplay{};

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