#include <Arduino.h>
#include "SensorNode.h"

#include <HexConverter.h>

unsigned int SensorNode::totalNumberOfNodes{};
unsigned int SensorNode::totalNumberOfInitializedNodes{};

void SensorNode::begin(const bool status, const unsigned int battery, const unsigned int id, const unsigned int display)
{
    if (!nodeID)
    {
        nodeStatus = status;
        nodeBattery = battery;
        nodeID = id;
        nodeDisplay = display;

        ++totalNumberOfNodes;
    }
}

void SensorNode::end()
{
    if (nodeID)
    {
        nodeStatus = false;
        nodeBattery = false;
        nodeID = false;
        nodeDisplay = false;

        --totalNumberOfNodes;
    }
}

SensorNode::operator String() const
{
    String string;

    string += "[N] ID\t: ";
    string += getNodeIDHexString();
    string += "\r\n";

    string += "[N] Init'd\t: ";
    string += getInitializationStatus() ? "YES" : "NO";
    string += "\r\n";

    string += "[N] Status\t: ";
    string += getNodeStatus() ? "PARKED" : "EMPTY";
    string += "\r\n";

    string += "[N] Battery\t: ";
    string += getNodeBattery();
    string += "\r\n";

    string += "[N] Display\t: ";
    string += getDisplayIDHexString();
    string += "\r\n";

    return string;
}

void SensorNode::setInitializationStatus(const bool status)
{
    if (isInitialized != status)
    {
        if (status)
        {
            isInitialized = true;
            ++totalNumberOfInitializedNodes;
        }
        else
        {
            isInitialized = false;
            --totalNumberOfInitializedNodes;
        }
    }
}