#include <Arduino.h>
#include "Node_RepeaterNode.h"

unsigned int Node_RepeaterNode::totalNumberOfRepeaters{};
unsigned int Node_RepeaterNode::totalNumberOfRepeaterObjects{};

Node_RepeaterNode::~Node_RepeaterNode()
{
    end();
    --totalNumberOfRepeaterObjects;
}

void Node_RepeaterNode::begin(const unsigned short id, const unsigned short route)
{
    if (!repeaterID)
    {
        repeaterID = id;
        repeaterRoute = route;

        ++totalNumberOfRepeaters;
    }
}

void Node_RepeaterNode::end()
{
    if (repeaterID)
    {
        repeaterID = false;
        repeaterRoute = false;

        --totalNumberOfRepeaters;
    }
}

void Node_RepeaterNode::printTableHeader()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print("R_ID");
    Serial.print('\t');
    Serial.print("ROUT");

    Serial.println();
}

void Node_RepeaterNode::printTable()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print(getRepeaterIDInHexString());
    Serial.print('\t');
    Serial.print(getRouteIDInHexString());

    Serial.println();
}

// ---------------------------------------------------------------------------------------------------

unsigned int Node_RepeaterNode::pointer{};

void Node_RepeaterNode::setPointerToEnd()
{
    if (getTotalNumberOfRepeaterObjects())
    {
        pointer = getTotalNumberOfRepeaters() - 1;
    }
}

const unsigned int Node_RepeaterNode::preincrementPointer()
{
    if (getTotalNumberOfRepeaters())
    {
        if (getPointer() < getTotalNumberOfRepeaters() && getTotalNumberOfRepeaters() < getTotalNumberOfRepeaterObjects())
        {
            return ++pointer;
        }
    }

    return getPointer();
}

const unsigned int Node_RepeaterNode::postincrementPointer()
{
    const unsigned int pointer = getPointer();
    preincrementPointer();
    return pointer;
}

const unsigned int Node_RepeaterNode::predecrementPointer()
{
    if (getPointer())
    {
        return --pointer;
    }

    return getPointer();
}

const unsigned int Node_RepeaterNode::postdecrementPointer()
{
    const unsigned int pointer = getPointer();
    predecrementPointer();
    return pointer;
}