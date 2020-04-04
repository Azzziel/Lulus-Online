#ifndef NODE_SENSOR_NODE_h
#define NODE_SENSOR_NODE_h

#include <Arduino.h>
#include <HexConverter.h>

class Node_SensorNode
{
public:
    void begin(const String &id, const String &repeater, const String &display)
    {
        begin(HexConverter::hexStringToUInt(id), HexConverter::hexStringToUInt(repeater), HexConverter::hexStringToUInt(display));
    }

    void begin(const unsigned int id, const unsigned int repeater, const unsigned int display);
    void end();

    void setNodeStatus(const bool status) { nodeStatus = status; }
    const bool getNodeStatus() const { return nodeStatus; }
    void flipNodeStatus() { setNodeStatus(!getNodeStatus()); }

    void setNodeBattery(const unsigned int battery) { nodeBattery = battery; }
    const unsigned int getNodeBattery() const { return nodeBattery; }

    const bool operator==(const unsigned int id) const { return id == getNodeID(); }
    const bool operator==(const String &id) const { return operator==(HexConverter::hexStringToUInt(id)); }

    const unsigned int getNodeID() const { return nodeID; }
    const String getNodeIDInHexString() const { return HexConverter::UIntToHexString(nodeID, 4); }

    const unsigned int getRepeaterID() const { return nodeRepeater; }
    const String getRepeaterIDInHexString() const { return HexConverter::UIntToHexString(nodeRepeater, 4); }

    const unsigned int getDisplayID() const { return nodeDisplay; }
    const String getDisplayIDInHexString() const { return HexConverter::UIntToHexString(nodeDisplay, 4); }

    void initialize(const bool status, const bool battery);
    void uninitialize();
    const bool getInitializationStatus() const { return isInitialized; }

    static unsigned int getTotalNumberOfNodes() { return totalNumberOfNodes; }
    static unsigned int getTotalNumberOfInitializedNodes() { return totalNumberOfInitializedNodes; }

private:
    static unsigned int totalNumberOfNodes;
    static unsigned int totalNumberOfInitializedNodes;

    bool isInitialized{};

    bool nodeStatus{};
    unsigned int nodeBattery{};

    unsigned int nodeID{};
    unsigned int nodeRepeater{};
    unsigned int nodeDisplay{};
};

#endif