#include <Arduino.h>
#include "Node_SensorNode.h"

#include <HexConverter.h>

unsigned int Node_SensorNode::totalNumberOfNodes{};
unsigned int Node_SensorNode::totalNumberOfInitializedNodes{};

void Node_SensorNode::begin(const unsigned int id, const unsigned int repeater, const unsigned int display)
{
    if (!nodeID)
    {
        nodeID = id;
        nodeRepeater = repeater;
        nodeDisplay = display;

        ++totalNumberOfNodes;
    }
}

void Node_SensorNode::end()
{
    if (nodeID)
    {
        nodeStatus = false;
        nodeBattery = false;
        nodeID = false;
        nodeRepeater = false;
        nodeDisplay = false;

        --totalNumberOfNodes;
    }
}

void Node_SensorNode::initialize(const bool status, const bool battery)
{
    if (!getInitializationStatus())
    {
        setNodeStatus(status);
        setNodeBattery(battery);

        isInitialized = true;
        ++totalNumberOfInitializedNodes;
    }
}

void Node_SensorNode::uninitialize()
{
    if (getInitializationStatus())
    {
        setNodeStatus(false);
        setNodeBattery(false);

        isInitialized = false;
        --totalNumberOfInitializedNodes;
    }
}