#ifndef SENSOR_NODE_h
#define SENSOR_NODE_h

#include <Arduino.h>
#include <HexConverter.h>

class SensorNode
{
public:
    SensorNode();
    ~SensorNode();

    inline void begin(const bool status, const unsigned int battery, const String &id, const String &display)
    {
        begin(status, battery, HexConverter::hexStringToUInt(id), HexConverter::hexStringToUInt(display));
    }

    void begin(const bool status, const unsigned int battery, const unsigned int id, const unsigned int display);

    inline bool getNodeStatus() const
    {
        return nodeStatus;
    }

    void setNodeStatus(const bool status);

    inline void flipNodeStatus()
    {
        setNodeStatus(!getNodeStatus());
    }

    inline unsigned int getNodeBattery() const
    {
        return nodeBattery;
    }

    void setNodeBattery(const unsigned int battery);

    inline bool checkNodeID(const unsigned int id) const
    {
        return id == getNodeID();
    }

    inline unsigned int getNodeID() const
    {
        return nodeID;
    }

    inline unsigned int getMatrixNodePointsTo() const
    {
        return nodeDisplay;
    }

    void initialize();
    
    inline bool getInitializationStatus() const
    {
        return isInitialized;
    }

    operator String() const;

    static inline unsigned int getNodeCount()
    {
        return nodeCount;
    }

private:
    static unsigned int nodeCount;

    bool isInitialized = false;
    
    bool nodeStatus{};
    unsigned int nodeBattery{};

    unsigned int nodeID{};
    unsigned int nodeDisplay{};
};

#endif