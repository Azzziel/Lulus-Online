#include <vector>

#include <Arduino.h>

#include <EEPROM.h>

#include <SoftwareSerial.h>

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
StaticJsonDocument<3840> document;

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

namespace Receiver
{
const unsigned int SOFT_RX = 16U;
const unsigned int SOFT_TX = 17U;
SoftwareSerial serial(SOFT_RX, SOFT_TX);

const unsigned int SET = 4U;
Node_HC12 HC12(&serial, SET);

const uint32_t BAUDRATE = 2400U;
const uint8_t CHANNEL = 19U;
} // namespace Receiver

namespace Transmitter
{
const unsigned int SOFT_RX = 26U;
const unsigned int SOFT_TX = 27U;
SoftwareSerial serial(SOFT_RX, SOFT_TX);

const unsigned int SET = 25U;
Node_HC12 HC12(&serial, SET);

const uint32_t BAUDRATE = 2400U;
const uint8_t CHANNEL = 13U;
} // namespace Transmitter

namespace Message
{
const char START = '<';
const char SEPARATOR = '/';
const char SUBSEPARATOR = ':';
const char END = '>';
} // namespace Message

// Access this array only by the class-provided pointer
Node_SensorNode sensorNodes[512U];
void printSensorNodes();

// Access this array only by the class-provided pointer
Node_RepeaterNode repeaterNodes[32U];
void printRepeaterNodes();

// Access this array only by the class-provided pointer
Node_DisplayNode displayNodes[32U];
void printDisplayNodes();

const String encodeDisplayRoute(const String &displayID, const String &receiverRouteID, const char separator);
const String encodeDisplayRoute(const unsigned short displayID, const unsigned short receiverRouteID, const char separator);

Divider dividedQuery;

HTTPClient client;
POSTHandler post(&client, SERVER_URL, SERVER_KEY);

void setup()
{
    Serial.begin(MONITOR_SPEED);

    Receiver::HC12.begin(Receiver::BAUDRATE, Receiver::CHANNEL);
    Transmitter::HC12.begin(Transmitter::BAUDRATE, Transmitter::CHANNEL);

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
        delay(0U);

    post.addRequestData("gtwy_id", This::ID.getIDInHexString().c_str());
    Query::loadStringPayload("get_gateway_status.php", Query::getGatewayStatus, nullptr);

    Serial.print("[M] Gateway ID: ");
    Serial.print(This::ID.getIDInHexString());
    Serial.println();

    const unsigned long queryStartMillis = millis();

    // Query ------------------------------------------------------------------------------------------
    Query::loadStringPayload("get_repeater_nodes.php", Query::loadJsonPayload, Query::loadRepeaterNodes);
    Query::loadStringPayload("get_display_nodes.php", Query::loadJsonPayload, Query::loadDisplayNodes);

    Query::loadStringPayload("count_sensor_nodes.php", Query::getSensorCount, nullptr);
    Node_SensorNode::setPointerToHome();
    while (QueryByParts::pointer < dividedQuery.getArraySize())
    {
        post.addRequestData("query_index", String(QueryByParts::index).c_str());
        post.addRequestData("query_limit", String(QueryByParts::LIMIT).c_str());

        Query::loadStringPayload("get_sensor_nodes_with_battery.php", Query::loadJsonPayload, Query::loadSensorNodes);

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

    printRepeaterNodes();
    printDisplayNodes();
    printSensorNodes();
}

void loop()
{
    if (Receiver::serial.available())
    {
        String buffer{"[R] "};
        do
        {
            buffer += static_cast<char>(Receiver::serial.read());
            delay(10);
        } while (Receiver::serial.available());

        Serial.println(buffer);
    }

    if (Transmitter::serial.available())
    {
        String buffer{"[T] "};
        do
        {
            buffer += static_cast<char>(Transmitter::serial.read());
            delay(10);
        } while (Transmitter::serial.available());

        Serial.println(buffer);
    }
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

        while (true) // Halt operation
            delay(0U);
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

        while (true) // Halt operation
            delay(0U);
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

        while (true) // Halt operation
            delay(0U);
    }
    else
    {
        loadFunction();
    }
}

void Query::loadDisplayNodes()
{
    Node_DisplayNode::setPointerToHome();

    for (size_t index = 0U; index < Query::document.size(); ++index)
    {
        if (Node_DisplayNode::getPointer() >= Node_DisplayNode::getTotalNumberOfDisplayObjects())
        {
            Serial.println(F("[M][E] Repeater objects are not enough to contain the devices"));

            while (true) // Halt operation
                delay(0U);
        }

        displayNodes[Node_DisplayNode::getPointer()].begin(
            HexConverter::hexStringToUShort(Query::document[index]["disp_id"]),
            encodeDisplayRoute(
                HexConverter::hexStringToUShort(Query::document[index]["disp_id"]),
                HexConverter::hexStringToUShort(Query::document[index]["recv_rt"]),
                Message::SUBSEPARATOR));

        Node_DisplayNode::preincrementPointer();
    }
}

void Query::loadRepeaterNodes()
{
    Node_RepeaterNode::setPointerToHome();

    for (size_t index = 0U; index < Query::document.size(); ++index)
    {
        if (Node_RepeaterNode::getPointer() >= Node_RepeaterNode::getTotalNumberOfRepeaterObjects())
        {
            Serial.println(F("[M][E] Repeater objects are not enough to contain the devices"));

            while (true) // Halt operation
                delay(0U);
        }

        repeaterNodes[Node_RepeaterNode::getPointer()].begin(
            HexConverter::hexStringToUShort(Query::document[index]["rptr_id"]),
            HexConverter::hexStringToUShort(Query::document[index]["send_rt"]));

        Node_RepeaterNode::preincrementPointer();
    }
}

void Query::loadSensorNodes()
{
    for (size_t index = 0U; index < Query::document.size(); ++index)
    {
        if (Node_SensorNode::getPointer() >= Node_SensorNode::getTotalNumberOfNodeObjects())
        {
            Serial.println(F("[M][E] Node objects are not enough to contain the devices"));

            while (true) // Halt operation
                delay(0U);
        }

        sensorNodes[Node_SensorNode::getPointer()].begin(
            HexConverter::hexStringToUShort(Query::document[index]["node_id"]),
            HexConverter::hexStringToUShort(Query::document[index]["disp_rt"]));

        sensorNodes[Node_SensorNode::getPointer()].setNodeBattery(Query::document[index]["battery"]);

        Node_SensorNode::preincrementPointer();
    }
}

const String encodeDisplayRoute(const String &displayID, const String &receiverRouteID, const char separator)
{
    return encodeDisplayRoute(
        HexConverter::hexStringToUShort(displayID),
        HexConverter::hexStringToUShort(receiverRouteID),
        separator);
}

const String encodeDisplayRoute(const unsigned short displayID, const unsigned short receiverRouteID, const char separator)
{
    std::vector<unsigned short> routes{displayID, receiverRouteID};

    unsigned int watchdogCounter = 0U;
    while (routes.back() != This::ID.getID())
    {
        for (
            Node_RepeaterNode::setPointerToHome();
            Node_RepeaterNode::getPointer() < Node_RepeaterNode::getTotalNumberOfRepeaters();
            Node_RepeaterNode::preincrementPointer())
        {
            if (repeaterNodes[Node_RepeaterNode::getPointer()] == routes.back())
            {
                routes.push_back(repeaterNodes[Node_RepeaterNode::getPointer()].getRouteID());
                watchdogCounter = 0U;
                break;
            }
        }

        if (watchdogCounter)
        {
            Serial.println(F("[M][W] Watchdog counter tripped! Final route not found"));
            break;
        }
    }

    String completeRoute;
    for (size_t index = routes.size() - 2 /* Intentional ignorance to the last index */; index < routes.size(); --index)
    {
        completeRoute += HexConverter::UIntToHexString(routes.at(index));

        if (index)
        {
            completeRoute += separator;
        }
    }

    return completeRoute;
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