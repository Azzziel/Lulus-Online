#ifndef SENSOR_NODE_h
#define SENSOR_NODE_h

#include <Arduino.h>
#include <HexConverter.h>

class SensorNode
{
public:
    void begin(const bool status, const unsigned int battery, const String &id, const String &display)
    {
        begin(status, battery, HexConverter::hexStringToUInt(id), HexConverter::hexStringToUInt(display));
    }

    void begin(const bool status, const unsigned int battery, const unsigned int id, const unsigned int display);
    void end();

    void setNodeStatus(const bool status) { nodeStatus = status; }
    const bool getNodeStatus() const { return nodeStatus; }
    void flipNodeStatus() { setNodeStatus(!getNodeStatus()); }

    void setNodeBattery(const unsigned int battery) { nodeBattery = battery; }
    const unsigned int getNodeBattery() const { return nodeBattery; }

    const bool checkNodeID(const unsigned int id) const { return id == getNodeID(); }
    const bool checkNodeID(const String &id) const { return checkNodeID(HexConverter::hexStringToUInt(id)); }

    const unsigned int getNodeID() const { return nodeID; }
    const String getNodeIDHexString() const { return HexConverter::UIntToHexStringWithLiteral(nodeID, 4); }

    const unsigned int getDisplayID() const { return nodeDisplay; }
    const String getDisplayIDHexString() const { return HexConverter::UIntToHexStringWithLiteral(nodeDisplay, 4); }

    void setInitializationStatus(const bool status);
    const bool getInitializationStatus() const { return isInitialized; }

    operator String() const;

    static unsigned int getTotalNumberOfNodes() { return totalNumberOfNodes; }
    static unsigned int getTotalNumberOfInitializedNodes() { return totalNumberOfInitializedNodes; }

private:
    static unsigned int totalNumberOfNodes;
    static unsigned int totalNumberOfInitializedNodes;

    bool isInitialized{};

    bool nodeStatus{};
    unsigned int nodeBattery{};

    unsigned int nodeID{};
    unsigned int nodeDisplay{};
};

#endif