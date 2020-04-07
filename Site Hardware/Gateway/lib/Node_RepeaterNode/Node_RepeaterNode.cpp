#include <Arduino.h>
#include "Node_RepeaterNode.h"

#include <HexConverter.h>

unsigned int Node_RepeaterNode::pointer{};
unsigned int Node_RepeaterNode::totalNumberOfRepeaters{};
unsigned int Node_RepeaterNode::totalNumberOfRepeaterObjects{};

void Node_RepeaterNode::begin(const unsigned int id, const unsigned int route)
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

void Node_RepeaterNode::setPointerToTheLastRepeater()
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