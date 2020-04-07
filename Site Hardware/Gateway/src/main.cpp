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
const unsigned int LIMIT = 32U;

unsigned int index = 1U;
unsigned int pointer = 0U;
} // namespace Query

// Access this array only by the class-provided pointer
Node_SensorNode sensorNodes[512U];

// Access this array only by the class-provided pointer
Node_RepeaterNode repeaterNodes[32U];

// Access this array only by the class-provided pointer
Node_DisplayNode displayNodes[32U];

Divider divider;

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

    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
        delay(0);

    int httpCode;
    const String payload = post.getStringPayload("count_sensor_nodes.php", &httpCode);

    if (httpCode == t_http_codes::HTTP_CODE_OK)
    {
        divider.loadDivider(payload.toInt(), Query::LIMIT);
    }
    else
    {
        Serial.println(F("[M][E] Failed to get total number of devices"));

        while (1) // Halt operation
            delay(0);
    }

    Node_SensorNode::setPointerToTheFirstNode();
    while (Query::pointer < divider.getArraySize())
    {
        post.addRequestData("query_index", String(Query::index).c_str());
        post.addRequestData("query_limit", String(Query::LIMIT).c_str());

        int httpCode;
        const String payload = post.getStringPayload("get_sensor_nodes.php", &httpCode);

        if (httpCode == t_http_codes::HTTP_CODE_OK)
        {
            StaticJsonDocument<3584> document;
            DeserializationError deserializationError = deserializeJson(document, payload.c_str());

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
                for (size_t index = 0; index < document.size(); ++index)
                {
                    if (Node_SensorNode::getPointer() >= Node_SensorNode::getTotalNumberOfNodeObjects())
                    {
                        Serial.println(F("[M][E] Node objects are not enough to contain the devices"));

                        while (1) // Halt operation
                            delay(0);
                    }

                    sensorNodes[Node_SensorNode::getPointer()].begin(
                        HexConverter::hexStringToUShort(document[index]["node_id"]),
                        HexConverter::hexStringToUShort(document[index]["disp_rt"]));

                    Node_SensorNode::preincrementPointer();
                }
            }
        }
        else
        {
            Serial.println(F("[M][E] Failed to get sensor nodes"));

            while (1) // Halt operation
                delay(0);
        }

        Query::index += divider[Query::pointer];
        ++Query::pointer;
    }

    Serial.println(F("[M] Loading data done"));

    Query::index = 1U;
    Query::pointer = 0U;

    divider.clearArray();

    digitalWrite(BUILTIN_LED, LOW);

    Node_SensorNode::printTableHeader();
    for (
        Node_SensorNode::setPointerToTheFirstNode();
        Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
        Node_SensorNode::preincrementPointer())
    {
        sensorNodes[Node_SensorNode::getPointer()].printTable();
    }
}

void loop()
{
}