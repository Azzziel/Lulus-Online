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
#include <SensorNode.h>

const char WIFI_SSID[] PROGMEM = "HOME-NETWORK";
const char WIFI_PASS[] PROGMEM = ":Waffle/64/Licious:";
const char SERVER_URL[] PROGMEM = "http://mejakalori.xyz/NodeTest/";
const char SERVER_KEY[] PROGMEM = "2fad7ae4be04ce093c8a9e48511fdf6c";

const unsigned long MONITOR_SPEED = 115200UL;

namespace ThisGateway
{
const unsigned int ID_SAVE_ADDRESS = 0U;
unsigned short ID;
} // namespace ThisGateway

namespace HC12
{
const unsigned long BAUD_RATE = 2400UL;
const unsigned int SET_PIN = 4U;

void setToATCommandMode();
void setToTransmissionMode();
} // namespace HC12

namespace MessageChar
{
const char START = '<';
const char END = '>';
const char FINAL = '#';
const char SEPARATOR = '/';
const char SUBSEPARATOR = ':';
} // namespace MessageChar

HTTPClient httpClient;
POSTHandler post{&httpClient, SERVER_URL, SERVER_KEY};

PayloadHandler messagePayload(MessageChar::SEPARATOR);

SensorNode *nodeArray = nullptr;

void setup()
{
    pinMode(HC12::SET_PIN, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT);

    Serial.begin(MONITOR_SPEED);
    Serial2.begin(HC12::BAUD_RATE);

    // WiFi has yet to connect, turn the LED on
    digitalWrite(BUILTIN_LED, HIGH);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED)
        delay(0);

    // Now that it's connected, turn the LED off
    digitalWrite(BUILTIN_LED, LOW);

    EEPROM.begin(sizeof(ThisGateway::ID));
    ThisGateway::ID = EEPROM.readUShort(ThisGateway::ID_SAVE_ADDRESS);

    Serial.print("[M] This gateway's ID is ");
    Serial.print(HexConverter::UIntToHexStringWithLiteral(ThisGateway::ID));
    Serial.println();

    // Pulling sensor node data from the database ----------------------

    int httpCode;
    const String payload = post.getStringPayload("get_node_hex_table.php", &httpCode);

    if (httpCode != t_http_codes::HTTP_CODE_OK)
    {
        Serial.print("[M] WiFi status: ");
        Serial.print(WiFi.status());
        Serial.println();

        Serial.print("[M] HTTP code: ");
        Serial.print(httpCode);
        Serial.println();

        Serial.println("[M][E] Failed to get payload due to network or HTTP failure.");
    }
    else
    {
        Serial.println("[M] Getting payload successful. Loading sensor data...");

        // StaticJsonDocument<5000> document;
        DynamicJsonDocument document{5000}; // Change this later to static document to reduce heap fragmentation

        DeserializationError deserializationError = deserializeJson(document, payload.c_str());

        if (deserializationError)
        {
            Serial.print("[M][E] Deserialization error: ");
            Serial.print(deserializationError.c_str());
            Serial.println();
        }
        else
        {
            nodeArray = new SensorNode[document.size()];

            for (size_t index{}; index < document.size(); ++index)
            {
                nodeArray[index].begin(
                    document[index]["node_status"],
                    document[index]["node_battery"],
                    HexConverter::hexStringToUInt(document[index]["node_id"]),
                    HexConverter::hexStringToUInt(document[index]["node_display"]));
            }

            Serial.println("[M] Loading data done.");
        }
    }

    // -----------------------------------------------------------------
}

void loop()
{
    if (Serial.available())
    {
        String reading;

        do
        {
            reading += static_cast<char>(Serial.read());
        } while (Serial.available());

        const String PRINT_NODE_TABLE = "PRINT_NODE_TABLE";
        const String PRINT_NODE_ID = "PRINT_NODE_ID_";

        if (reading == PRINT_NODE_TABLE)
        {
            if (nodeArray != nullptr)
            {
                Serial.print("[I]");
                Serial.print('\t');

                Serial.print("ID");
                Serial.print('\t');

                Serial.print("INIT");
                Serial.print('\t');

                Serial.print("STAT");
                Serial.print('\t');

                Serial.print("BATT");
                Serial.print('\t');

                Serial.print("MTRX");
                Serial.println();

                for (size_t index = 0; index < SensorNode::getNodeCount(); ++index)
                {
                    Serial.print('[');
                    Serial.print(index);
                    Serial.print(']');
                    Serial.print('\t');

                    Serial.print(HexConverter::UIntToHexStringWithLiteral(nodeArray[index].getNodeID(), 4));
                    Serial.print('\t');

                    Serial.print(nodeArray[index].getInitializationStatus() ? "YES" : "NO");
                    Serial.print('\t');

                    Serial.print(nodeArray[index].getNodeStatus());
                    Serial.print('\t');

                    Serial.print(nodeArray[index].getNodeBattery());
                    Serial.print('\t');

                    Serial.print(HexConverter::UIntToHexStringWithLiteral(nodeArray[index].getMatrixNodePointsTo(), 4));
                    Serial.println();
                }
            }
            else
            {
                Serial.println("[M][E] Node array is not initialized.");
            }
        }
        else if (reading.startsWith(PRINT_NODE_ID))
        {
            reading.remove(0, PRINT_NODE_ID.length());

            const unsigned int nodeID = HexConverter::hexStringToUInt(reading);

            if (nodeArray != nullptr)
            {
                for (size_t index = 0; index < SensorNode::getNodeCount(); ++index)
                {
                    if (nodeArray[index].checkNodeID(nodeID))
                    {
                        Serial.print('[');
                        Serial.print(index);
                        Serial.print(']');
                        Serial.println();

                        Serial.println(nodeArray[index]);

                        break;
                    }
                }
            }
            else
            {
                Serial.println("[M][E] Node array is not initialized.");
            }
        }
        else
        {
            Serial.println("[M][E] Command message unknown.");
        }
    }

    if (Serial2.available())
    {
        String messageReading;

        do
        {
            messageReading += static_cast<char>(Serial2.read());

            // Stop reading when the end char is detected
            if (messageReading[messageReading.length() - 1] == MessageChar::END)
            {
                // Removal of the end char
                messageReading.remove(messageReading.length() - 1, 1);

                Serial2.flush();
                break;
            }

            // Serial2.available() can return false when the data transmission is slow, delay a little to be safe (7)
            delay(7);
        } while (Serial2.available());

        // Verify that the message starts with the start char defined in the namespace
        bool readingIsGood = true;
        if (messageReading[0] == MessageChar::START)
        {
            // Removal of the start char
            messageReading.remove(0, 1);

            for (size_t index = 0; index < messageReading.length() - 1; ++index)
            {
                // Verify that the message doesn't contain another start char and a stop char
                if (messageReading[index] == MessageChar::START ||
                    messageReading[index] == MessageChar::END)
                {
                    readingIsGood = false;
                    break;
                }
            }

            // Verify that the message ends with a final char
            if (messageReading[messageReading.length() - 1] == MessageChar::FINAL)
            {
                // Removal of the final char
                messageReading.remove(messageReading.length() - 1, 1);
            }
            else
            {
                readingIsGood = false;
            }
        }
        else
        {
            readingIsGood = false;
        }

        if (readingIsGood)
        {
            messagePayload.loadPayload(&messageReading);

            if (messagePayload[0] != HexConverter::UIntToHexString(ThisGateway::ID))
            {
                Serial.println("[M] The message is not intended to be received by this gateway, ignored.");
                messagePayload.unloadPayload();
            }
        }
    }

    if (messagePayload.isSet())
    {
        // Process payload here

        messagePayload.unloadPayload();
    }
}

void HC12::setToATCommandMode()
{
    digitalWrite(HC12::SET_PIN, LOW);
    delay(40);
}

void HC12::setToTransmissionMode()
{
    digitalWrite(HC12::SET_PIN, HIGH);
    delay(80);
}