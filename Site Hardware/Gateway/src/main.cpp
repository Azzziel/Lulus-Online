#include <Arduino.h>

#include <EEPROM.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

#include <ArduinoJson.h>
#include <Node_HC12.h>

#include <HexConverter.h>
#include <POSTHandler.h>
#include <PayloadHandler.h>
#include <Divider.h>
#include <Node_DisplayNode.h>
#include <Node_RepeaterNode.h>
#include <Node_SensorNode.h>
#include <Node_ID.h>

const unsigned long MONITOR_SPEED = 115200UL;

const char WIFI_SSID[] PROGMEM = "HOME-NETWORK";
const char WIFI_PASS[] PROGMEM = ":Waffle/192/Licious:";
const char SERVER_URL[] PROGMEM = "http://mejakalori.xyz/NodeTest/";
const char SERVER_KEY[] PROGMEM = "2fad7ae4be04ce093c8a9e48511fdf6c";

namespace This
{
const unsigned int ID_SAVE_ADDRESS = 0U;
const char ID_SIGNATURE = 'G';

Node_ID ID;
} // namespace This

namespace Query
{
StaticJsonDocument<2560> document;

void loadStringPayload(const char *, void (*)(const String &, void (*)()), void (*)());
void getSensorCount(const String &, void (*)());
void getGatewayStatus(const String &, void (*)());

void loadJsonPayload(const String &, void (*)());
void loadDisplayNodes();
void loadRepeaterNodes();
void loadSensorNodes();
} // namespace Query

namespace QueryByParts
{
const unsigned int LIMIT = 32U;

unsigned int index = 1U;
unsigned int pointer = 0U;
} // namespace QueryByParts

// Access this array only by the class-provided pointer
Node_SensorNode sensorNodes[512U];
void printSensorNodes();

// Access this array only by the class-provided pointer
Node_RepeaterNode repeaterNodes[32U];
void printRepeaterNodes();

// Access this array only by the class-provided pointer
Node_DisplayNode displayNodes[32U];
void printDisplayNodes();

Divider dividedQuery;

HTTPClient client;
POSTHandler post(&client, SERVER_URL, SERVER_KEY);

void setup()
{
    Serial.begin(MONITOR_SPEED);

    EEPROM.begin(sizeof(Node_ID::ID_t));
    This::ID.loadIDFromEEPROM(This::ID_SAVE_ADDRESS, This::ID_SIGNATURE);
    EEPROM.end();

    if (!This::ID.getID())
    {
        Serial.println(F("[M][E] EEPROM corrupt or not set"));

        while (1) // Halt operation
            delay(0);
    }

    // Turn BUILTIN_LED on to indicate network-related startup
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
        delay(0);

    post.addRequestData("gtwy_id", This::ID.getIDInHexString().c_str());
    Query::loadStringPayload("get_gateway_status.php", Query::getGatewayStatus, nullptr);

    Serial.print("[M] Gateway ID: ");
    Serial.print(HexConverter::UIntToHexString(This::ID.getID()));
    Serial.println();

    const unsigned long queryStartMillis = millis();

    // Query ------------------------------------------------------------------------------------------
    Query::loadStringPayload("get_display_nodes.php", Query::loadJsonPayload, Query::loadDisplayNodes);
    Query::loadStringPayload("get_repeater_nodes.php", Query::loadJsonPayload, Query::loadRepeaterNodes);

    Query::loadStringPayload("count_sensor_nodes.php", Query::getSensorCount, nullptr);
    Node_SensorNode::setPointerToHome();
    while (QueryByParts::pointer < dividedQuery.getArraySize())
    {
        post.addRequestData("query_index", String(QueryByParts::index).c_str());
        post.addRequestData("query_limit", String(QueryByParts::LIMIT).c_str());

        Query::loadStringPayload("get_sensor_nodes.php", Query::loadJsonPayload, Query::loadSensorNodes);

        QueryByParts::index += dividedQuery[QueryByParts::pointer];
        ++QueryByParts::pointer;
    }
    dividedQuery.clearArray();
    // ------------------------------------------------------------------------------------------------

    Serial.print(F("[M] Data retrieval done in "));
    Serial.print(millis() - queryStartMillis);
    Serial.print(F(" ms"));
    Serial.println();

    // Turn the LED off, by this point it is proven that everything above has succeeded
    digitalWrite(BUILTIN_LED, LOW);

    printDisplayNodes();
    printRepeaterNodes();
    printSensorNodes();
}

void loop()
{
}

void Query::loadStringPayload(const char *phpFilename, void (*stringLoader)(const String &, void (*)()), void (*jsonLoader)())
{
    int httpCode;
    const String payload = post.getStringPayload(phpFilename, &httpCode);

    if (httpCode == t_http_codes::HTTP_CODE_OK)
    {
        stringLoader(payload, jsonLoader);
    }
    else
    {
        Serial.println(F("[M][E] Failed to retrieve data due to network or HTTP error"));

        while (1) // Halt operation
            delay(0);
    }
}

void Query::getSensorCount(const String &stringPayload, void (*)())
{
    dividedQuery.loadDivider(stringPayload.toInt(), QueryByParts::LIMIT);
}

void Query::getGatewayStatus(const String &stringPayload, void (*)())
{
    if (!stringPayload.toInt())
    {
        Serial.println(F("[M][E] Gateway was not found in the database or may have reached its end of service"));

        while (1) // Halt operation
            delay(0);
    }
}

void Query::loadJsonPayload(const String &stringPayload, void (*loadFunction)())
{
    DeserializationError deserializationError = deserializeJson(Query::document, stringPayload.c_str());

    if (deserializationError)
    {
        Serial.print(F("[M][E] Deserialization error: "));
        Serial.print(deserializationError.c_str());
        Serial.println();

        while (1) // Halt operation
            delay(0);
    }
    else
    {
        loadFunction();
    }
}

void Query::loadDisplayNodes()
{
    Node_DisplayNode::setPointerToHome();

    for (size_t index = 0; index < Query::document.size(); ++index)
    {
        if (Node_DisplayNode::getPointer() >= Node_DisplayNode::getTotalNumberOfDisplayObjects())
        {
            Serial.println(F("[M][E] Repeater objects are not enough to contain the devices"));

            while (1) // Halt operation
                delay(0);
        }

        displayNodes[Node_DisplayNode::getPointer()].begin(
            HexConverter::hexStringToUShort(Query::document[index]["disp_id"]),
            HexConverter::hexStringToUShort(Query::document[index]["recv_rt"]));

        Node_DisplayNode::preincrementPointer();
    }
}

void Query::loadRepeaterNodes()
{
    Node_RepeaterNode::setPointerToHome();

    for (size_t index = 0; index < Query::document.size(); ++index)
    {
        if (Node_RepeaterNode::getPointer() >= Node_RepeaterNode::getTotalNumberOfRepeaterObjects())
        {
            Serial.println(F("[M][E] Repeater objects are not enough to contain the devices"));

            while (1) // Halt operation
                delay(0);
        }

        repeaterNodes[Node_RepeaterNode::getPointer()].begin(
            HexConverter::hexStringToUShort(Query::document[index]["rptr_id"]),
            HexConverter::hexStringToUShort(Query::document[index]["send_rt"]));

        Node_RepeaterNode::preincrementPointer();
    }
}

void Query::loadSensorNodes()
{
    for (size_t index = 0; index < Query::document.size(); ++index)
    {
        if (Node_SensorNode::getPointer() >= Node_SensorNode::getTotalNumberOfNodeObjects())
        {
            Serial.println(F("[M][E] Node objects are not enough to contain the devices"));

            while (1) // Halt operation
                delay(0);
        }

        sensorNodes[Node_SensorNode::getPointer()].begin(
            HexConverter::hexStringToUShort(Query::document[index]["node_id"]),
            HexConverter::hexStringToUShort(Query::document[index]["disp_rt"]));

        Node_SensorNode::preincrementPointer();
    }
}

void printSensorNodes()
{
    Node_SensorNode::printTableHeader();
    for (
        Node_SensorNode::setPointerToHome();
        Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
        Node_SensorNode::preincrementPointer())
    {
        sensorNodes[Node_SensorNode::getPointer()].printTable();
    }
}

void printRepeaterNodes()
{
    Node_RepeaterNode::printTableHeader();
    for (
        Node_RepeaterNode::setPointerToHome();
        Node_RepeaterNode::getPointer() < Node_RepeaterNode::getTotalNumberOfRepeaters();
        Node_RepeaterNode::preincrementPointer())
    {
        repeaterNodes[Node_RepeaterNode::getPointer()].printTable();
    }
}

void printDisplayNodes()
{
    Node_DisplayNode::printTableHeader();
    for (
        Node_DisplayNode::setPointerToHome();
        Node_DisplayNode::getPointer() < Node_DisplayNode::getTotalNumberOfDisplays();
        Node_DisplayNode::preincrementPointer())
    {
        displayNodes[Node_DisplayNode::getPointer()].printTable();
    }
}