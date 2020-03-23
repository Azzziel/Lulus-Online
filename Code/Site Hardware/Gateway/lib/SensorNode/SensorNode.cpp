#include <Arduino.h>
#include "SensorNode.h"

#include <HexConverter.h>

unsigned int SensorNode::nodeCount{};

SensorNode::SensorNode()
{
    ++nodeCount;
}

SensorNode::~SensorNode()
{
    --nodeCount;
}

void SensorNode::begin(const bool status, const unsigned int battery, const unsigned int id, const unsigned int display)
{
    if (!nodeID)
    {
        nodeStatus = status;
        nodeBattery = battery;
        nodeID = id;
        nodeDisplay = display;
    }
}

void SensorNode::setNodeStatus(const bool status)
{
    nodeStatus = status;
}

void SensorNode::setNodeBattery(const unsigned int battery)
{
    nodeBattery = battery;
}

SensorNode::operator String() const
{
    String string;

    string += "[N] ID (HEX)\t: ";
    string += HexConverter::UIntToHexStringWithLiteral(getNodeID(), 4);
    string += "\r\n";

    string += "[N] Initialized\t: ";
    string += getInitializationStatus() ? "YES" : "NO";
    string += "\r\n";

    string += "[N] Status\t: ";
    string += getNodeStatus();
    string += "\r\n";

    string += "[N] Battery\t: ";
    string += getNodeBattery();
    string += "\r\n";

    string += "[N] Display (HEX)\t: ";
    string += HexConverter::UIntToHexStringWithLiteral(getMatrixNodePointsTo(), 4);
    string += "\r\n";

    return string;
}

void SensorNode::initialize()
{
    isInitialized = true;
}