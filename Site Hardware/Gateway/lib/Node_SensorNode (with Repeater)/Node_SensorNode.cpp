#include <Arduino.h>
#include "Node_SensorNode.h"

#include <HexConverter.h>

unsigned int Node_SensorNode::pointer{};
unsigned int Node_SensorNode::totalNumberOfNodes{};
unsigned int Node_SensorNode::totalNumberOfInitializedNodes{};
unsigned int Node_SensorNode::totalNumberOfNodeObjects{};

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

void Node_SensorNode::printTableHeader()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print("N_ID");
    Serial.print('\t');
    Serial.print("R_ID");
    Serial.print('\t');
    Serial.print("D_ID");
    Serial.print('\t');
    Serial.print("INIT");
    Serial.print('\t');
    Serial.print("STAT");
    Serial.print('\t');
    Serial.print("BATT");

    Serial.println();
}

void Node_SensorNode::printTable()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print(getNodeIDInHexString());
    Serial.print('\t');
    Serial.print(getRepeaterIDInHexString());
    Serial.print('\t');
    Serial.print(getDisplayIDInHexString());
    Serial.print('\t');
    Serial.print(F(getInitializationStatus() ? "INIT" : "UNIN"));

    if (getInitializationStatus())
    {
        Serial.print('\t');
        Serial.print(F(getNodeStatus() ? "PARK" : "EMPT"));
        Serial.print('\t');
        Serial.print(getNodeBattery());
    }

    Serial.println();
}

// ---------------------------------------------------------------------------------------------------

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