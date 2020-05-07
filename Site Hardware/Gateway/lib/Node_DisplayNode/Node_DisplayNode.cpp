#include <Arduino.h>
#include "Node_DisplayNode.h"

unsigned int Node_DisplayNode::totalNumberOfDisplays{};
unsigned int Node_DisplayNode::totalNumberOfDisplayObjects{};

Node_DisplayNode::~Node_DisplayNode()
{
    end();
    --totalNumberOfDisplayObjects;
}

void Node_DisplayNode::begin(const unsigned short id, const String &route)
{
    if (!displayID)
    {
        displayID = id;
        setDisplayRoute(route);

        ++totalNumberOfDisplays;
    }
}

void Node_DisplayNode::end()
{
    if (displayID)
    {
        displayID = false;
        unsetDisplayRoute();

        --totalNumberOfDisplays;
    }
}

void Node_DisplayNode::setDisplayRoute(const String &route)
{
    if (displayRoute == nullptr)
    {
        displayRoute = new const String(route);
    }
}

void Node_DisplayNode::unsetDisplayRoute()
{
    delete displayRoute;
    displayRoute = nullptr;
}

const String &Node_DisplayNode::getDisplayRoute()
{
    static const String ROUTE_UNSET PROGMEM = "ROUTE_UNSET";

    if (displayRoute == nullptr)
    {
        return ROUTE_UNSET;
    }

    return *displayRoute;
}

void Node_DisplayNode::printTableHeader()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print("D_ID");
    Serial.print('\t');
    Serial.print("COMPLETE_ROUTE");

    Serial.println();
}

void Node_DisplayNode::printTable()
{
    Serial.print("[T]");
    Serial.print('\t');

    Serial.print(getDisplayIDInHexString());
    Serial.print('\t');
    Serial.print(getDisplayRoute());

    Serial.println();
}

// ---------------------------------------------------------------------------------------------------

unsigned int Node_DisplayNode::pointer{};

void Node_DisplayNode::setPointerToEnd()
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