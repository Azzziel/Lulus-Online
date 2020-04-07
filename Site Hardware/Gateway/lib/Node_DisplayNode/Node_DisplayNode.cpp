#include <Arduino.h>
#include "Node_DisplayNode.h"

#include <HexConverter.h>

unsigned int Node_DisplayNode::totalNumberOfDisplays{};
unsigned int Node_DisplayNode::totalNumberOfDisplayObjects{};

void Node_DisplayNode::begin(const unsigned int id, const unsigned int route)
{
    if (!displayID)
    {
        displayID = id;
        displayRoute = route;

        ++totalNumberOfDisplays;
    }
}

void Node_DisplayNode::end()
{
    if (displayID)
    {
        displayID = false;
        displayRoute = false;

        --totalNumberOfDisplays;
    }
}

void Node_DisplayNode::printTableHeader()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print("D_ID");
    Serial.print('\t');
    Serial.print("ROUT");

    Serial.println();
}

void Node_DisplayNode::printTable()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print(getDisplayIDInHexString());
    Serial.print('\t');
    Serial.print(getRouteIDInHexString());

    Serial.println();
}

// ---------------------------------------------------------------------------------------------------

void Node_DisplayNode::setPointerToTheLastDisplay()
{
    if (getTotalNumberOfDisplayObjects())
    {
        pointer = getTotalNumberOfDisplays() - 1;
    }
}

const unsigned int Node_DisplayNode::preincrementPointer()
{
    if (getTotalNumberOfDisplays())
    {
        if (getPointer() < getTotalNumberOfDisplays() && getTotalNumberOfDisplays() < getTotalNumberOfDisplayObjects())
        {
            return ++pointer;
        }
    }

    return getPointer();
}

const unsigned int Node_DisplayNode::postincrementPointer()
{
    const unsigned int pointer = getPointer();
    preincrementPointer();
    return pointer;
}

const unsigned int Node_DisplayNode::predecrementPointer()
{
    if (getPointer())
    {
        return --pointer;
    }

    return getPointer();
}

const unsigned int Node_DisplayNode::postdecrementPointer()
{
    const unsigned int pointer = getPointer();
    predecrementPointer();
    return pointer;
}