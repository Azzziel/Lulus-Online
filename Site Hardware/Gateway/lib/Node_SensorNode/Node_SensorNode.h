#ifndef NODE_SENSOR_NODE_h
#define NODE_SENSOR_NODE_h

#include <Arduino.h>
#include <HexConverter.h>

class Node_SensorNode
{
public:
    Node_SensorNode() { ++totalNumberOfNodeObjects; }
    ~Node_SensorNode() { --totalNumberOfNodeObjects; }

    void begin(const String &id, const String &display)
    {
        begin(HexConverter::hexStringToUInt(id), HexConverter::hexStringToUInt(display));
    }

    void begin(const unsigned int id, const unsigned int display);
    void end();

    void setNodeStatus(const bool status) { nodeStatus = status; }
    const bool getNodeStatus() const { return nodeStatus; }
    void flipNodeStatus() { setNodeStatus(!getNodeStatus()); }

    void setNodeBattery(const unsigned int battery) { nodeBattery = battery; }
    const unsigned int getNodeBattery() const { return nodeBattery; }

    const bool operator==(const unsigned int id) const { return id == getNodeID(); }
    const bool operator==(const String &id) const { return operator==(HexConverter::hexStringToUInt(id)); }

    const unsigned int getNodeID() const { return nodeID; }
    const String getNodeIDInHexString() const { return HexConverter::UIntToHexString(getNodeID(), 4); }

    const unsigned int getDisplayID() const { return nodeDisplay; }
    const String getDisplayIDInHexString() const { return HexConverter::UIntToHexString(getDisplayID(), 4); }

    void initialize(const bool status, const bool battery);
    void uninitialize();
    const bool getInitializationStatus() const { return isInitialized; }

    void printTable();

    static void printTableHeader();

    static const unsigned int getTotalNumberOfNodes() { return totalNumberOfNodes; }
    static const unsigned int getTotalNumberOfInitializedNodes() { return totalNumberOfInitializedNodes; }
    static const unsigned int getTotalNumberOfNodeObjects() { return totalNumberOfNodeObjects; }

private:
    static unsigned int totalNumberOfNodes;
    static unsigned int totalNumberOfInitializedNodes;
    static unsigned int totalNumberOfNodeObjects;

    bool isInitialized{};

    bool nodeStatus{};
    unsigned int nodeBattery{};

    unsigned int nodeID{};
    unsigned int nodeDisplay{};

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