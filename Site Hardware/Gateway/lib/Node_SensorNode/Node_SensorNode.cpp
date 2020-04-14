#include <Arduino.h>
#include "Node_SensorNode.h"

#include <HexConverter.h>

unsigned int Node_SensorNode::totalNumberOfNodes{};
unsigned int Node_SensorNode::totalNumberOfNodeObjects{};

unsigned long Node_SensorNode::MAX_INACTIVE_PERIOD = 120000UL;

Node_SensorNode::~Node_SensorNode()
{
    end();
    --totalNumberOfNodeObjects;
}

void Node_SensorNode::begin(const unsigned short id, const unsigned short display)
{
    if (!nodeID)
    {
        nodeID = id;
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
        nodeDisplay = false;

        --totalNumberOfNodes;
    }
}

void Node_SensorNode::setNodeStatus(const bool status)
{
    nodeStatus = status;
    setLastReported();
}

void Node_SensorNode::printTableHeader()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print("N_ID");
    Serial.print('\t');
    Serial.print("D_ID");
    Serial.print('\t');
    Serial.print("BATT");
    Serial.print('\t');
    Serial.print("ACTV");
    Serial.print('\t');
    Serial.print("STAT");

    Serial.println();
}

void Node_SensorNode::printTable()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print(getNodeIDInHexString());
    Serial.print('\t');
    Serial.print(getDisplayIDInHexString());
    Serial.print('\t');

    if (getNodeBattery())
    {
        Serial.print("LV_");
        Serial.print(getNodeBattery());
    }
    else
    {
        Serial.print("NO_B");
    }

    Serial.print('\t');
    Serial.print(isActive() ? "ACTV" : "LOST");

    if (isActive())
    {
        Serial.print('\t');
        Serial.print(getNodeStatus() ? "PARK" : "EMPT");
    }

    Serial.println();
}

// ---------------------------------------------------------------------------------------------------

unsigned int Node_SensorNode::pointer{};

void Node_SensorNode::setPointerToEnd()
{
    if (getTotalNumberOfNodeObjects())
    {
        pointer = getTotalNumberOfNodeObjects() - 1;
    }
}

const unsigned int Node_SensorNode::preincrementPointer()
{
    if (getTotalNumberOfNodes())
    {
        if (getPointer() < getTotalNumberOfNodes() && getTotalNumberOfNodes() < getTotalNumberOfNodeObjects())
        {
            return ++pointer;
        }
    }

    return getPointer();
}

const unsigned int Node_SensorNode::postincrementPointer()
{
    const unsigned int pointer = getPointer();
    preincrementPointer();
    return pointer;
}

const unsigned int Node_SensorNode::predecrementPointer()
{
    if (getPointer())
    {
        return --pointer;
    }

    return getPointer();
}

const unsigned int Node_SensorNode::postdecrementPointer()
{
    const unsigned int pointer = getPointer();
    predecrementPointer();
    return pointer;
}