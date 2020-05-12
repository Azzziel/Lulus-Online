#include <deque>
#include <vector>

#include <Arduino.h>

#include <EEPROM.h>

#include <SoftwareSerial.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

#include <ArduinoJson.h>
#include <Node_HC12.h>    // https://github.com/ivan0kurnia/Node_HC12
#include <HexConverter.h> // https://github.com/ivan0kurnia/HexConverter

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
const char SERVER_URL[] PROGMEM = "http://onspot.my.id/api/gateway/";
const char SERVER_KEY[] PROGMEM = "5cb4bf3e42ff76ca9186850b9017bdc8";

// Definitions to seed random()
const unsigned int NOISE_PIN = 36;

namespace This
{
const unsigned int ID_SAVE_ADDRESS = 0U;
const char ID_SIGNATURE = 'G';

Node_ID ID;
} // namespace This

namespace Query
{
StaticJsonDocument<5120U> document;

void loadStringPayload(const char *, void (*)(const String &, void (*)()), void (*)());
void getSensorCount(const String &, void (*)());
void getGatewayStatus(const String &, void (*)());

void loadJsonPayload(const String &, void (*)());
void loadDisplayNodes();
void loadRepeaterNodes();
void loadSensorNodes();

struct Queue
{
    Queue(const unsigned char t, const unsigned short id, const unsigned char val) : type{t}, node_id{id}, value{val} {}

    const unsigned char type; // Refers to the enum in Node_SensorNode

    const unsigned short node_id;
    const unsigned char value;

    bool operator==(const Queue &q) const
    {
        return this->type == q.type && this->node_id == q.node_id && this->value == q.value;
    }
};

std::deque<Queue> queue;
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

const long timeLimit() { return random(5000, 12000); }

struct Queue
{
    Queue(const unsigned short device_id, const String &t, const String &k, const unsigned char v) : id{device_id}, to{t}, key{k}, value{v}, millisLimit{0UL} {}

    const unsigned short id;
    bool isSent = false;

    const String to;
    const String key;
    const unsigned char value;

    unsigned long millisLimit;

    bool operator==(const Queue &q) const
    {
        return this->id == q.id && this->to == q.to && this->key == q.key;
    }
};

std::deque<Queue> queue;
} // namespace Transmitter

namespace Message
{
const char START = '<';
const char SEPARATOR = '/';
const char SUBSEPARATOR = ':';
const char END = '>';

const unsigned int MAX_SUBSTRING_LENGTH = 4U;

// Caution! Will crash if the 'from' char array is not terminated with '\0'
const char *const load(const char *const from, char *const to, const size_t toSize);

const bool isSafeForStrtok(const char *const string, const size_t size);
const unsigned int countTokens(const char *const string, const size_t length);
} // namespace Message

// Access this array only by the class-provided pointer
Node_SensorNode sensorNodes[512U];
void iterateSensorNodes(void (*)());
void printSensorNodes();

const bool verifySensorNode(const String &ID);
const bool verifySensorNode(unsigned short ID);

// Access this array only by the class-provided pointer
Node_RepeaterNode repeaterNodes[32U];
void iterateRepeaterNodes(void (*)());
void printRepeaterNodes();

const bool verifyRepeaterNode(const String &ID);
const bool verifyRepeaterNode(unsigned short ID);

// Access this array only by the class-provided pointer
Node_DisplayNode displayNodes[32U];
void iterateDisplayNodes(void (*)());
void printDisplayNodes();

const String encodeDisplayRoute(const String &displayID, const String &receiverRouteID, const char separator);
const String encodeDisplayRoute(const unsigned short displayID, const unsigned short receiverRouteID, const char separator);

const unsigned int countDisplayValue(const String &displayID);
const unsigned int countDisplayValue(const unsigned short displayID);

const unsigned int countMainDisplayValue();

void setDisplayValue(const String &displayID, const unsigned short displayValue);
void setDisplayValue(const unsigned short displayID, const unsigned short displayValue);

// Untested for user-defined types, use with fundamental data types only
template <typename T>
const bool isArraySet(const T *t, const size_t length);

void printProcessTime(const char *const, void (*)());

Divider dividedQuery;

HTTPClient client;
POSTHandler post(&client, SERVER_URL, SERVER_KEY);

void setup()
{
    pinMode(NOISE_PIN, INPUT);
    randomSeed(analogRead(NOISE_PIN));

    Serial.begin(MONITOR_SPEED);

    /* Uncomment this section to write EEPROM for the first time */
    // EEPROM.begin(sizeof(Node_ID::ID_t));
    // This::ID.putNewIDToEEPROM(This::ID_SAVE_ADDRESS, {'G', HexConverter::toUShort("AAA1")});
    // EEPROM.end();

    EEPROM.begin(sizeof(Node_ID::ID_t));
    This::ID.loadIDFromEEPROM(This::ID_SAVE_ADDRESS, This::ID_SIGNATURE);
    EEPROM.end();

    if (!This::ID.getID())
    {
        Serial.println(F("[M][E] EEPROM corrupt or not set"));

        while (true) // Halt operation
            delay(0);
    }

    // Turn BUILTIN_LED on to indicate network-related startup
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
        delay(0U);

    // for (size_t i = 0; i < 10; ++i)
    // {
    //     printProcessTime("Bench", []() {
    //         int httpCode;
    //         const String payload = post.getStringPayload(".php", &httpCode);

    //         if (httpCode == t_http_codes::HTTP_CODE_OK)
    //         {
    //             // Do nothing
    //         }
    //         else
    //         {
    //             Serial.println(F("[M][E] Failed to retrieve data due to network or HTTP error"));
    //         }
    //     });
    // }

    post.addRequestData("gtwy_id", This::ID.getIDInHexString().c_str());
    Query::loadStringPayload("get_gateway_status.php", Query::getGatewayStatus, nullptr);

    Serial.print(F("[M] Gateway ID: "));
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

        Query::loadStringPayload("get_sensor_nodes_all.php", Query::loadJsonPayload, Query::loadSensorNodes);

        QueryByParts::index += dividedQuery[QueryByParts::pointer];
        ++QueryByParts::pointer;
    }
    dividedQuery.clearArray();

    iterateDisplayNodes([]() {
        unsigned int currentDisplayValue;

        if (displayNodes[Node_DisplayNode::getPointer()].isMain())
        {
            currentDisplayValue = countMainDisplayValue();
        }
        else
        {
            currentDisplayValue = countDisplayValue(displayNodes[Node_DisplayNode::getPointer()].getDisplayID());
        }

        displayNodes[Node_DisplayNode::getPointer()].setDisplayValue(currentDisplayValue);

        Transmitter::Queue newTransmission{displayNodes[Node_DisplayNode::getPointer()].getDisplayID(),
                                           displayNodes[Node_DisplayNode::getPointer()].getDisplayRoute(),
                                           F("NUM"),
                                           static_cast<unsigned char>(currentDisplayValue)};

        Transmitter::queue.push_back(newTransmission);
    });
    // ------------------------------------------------------------------------------------------------

    Serial.print(F("[M] Data retrieval done in "));
    Serial.print(millis() - queryStartMillis);
    Serial.print(F(" ms"));
    Serial.println();

    if (!Transmitter::HC12.begin(Transmitter::BAUDRATE, Transmitter::CHANNEL) ||
        !Receiver::HC12.begin(Receiver::BAUDRATE, Receiver::CHANNEL))
    {
        while (true) // Halt operation
            delay(0U);
    }

    Transmitter::serial.stopListening();

    // Turn the LED off, by this point it is proven that everything above has succeeded
    digitalWrite(BUILTIN_LED, LOW);
}

void loop()
{
    randomSeed(analogRead(NOISE_PIN));

    // Enables manual serial command
    if (Serial.available())
    {
        String buffer;

        unsigned long previousMicros = micros();
        while (true)
        {
            if (micros() - previousMicros < 500UL)
            {
                if (Serial.available())
                {
                    buffer += static_cast<char>(Serial.read());
                    previousMicros = micros();
                }
            }
            else
            {
                break;
            }
        }

        if (buffer == F("PRINT_SENSOR_NODES"))
        {
            printSensorNodes();
        }
        else if (buffer == F("PRINT_REPEATER_NODES"))
        {
            printRepeaterNodes();
        }
        else if (buffer == F("PRINT_DISPLAY_NODES"))
        {
            printDisplayNodes();
        }
    }

    // Receive routine, sends ACK on the go, removes a transmission queue after an ACK
    if (Receiver::serial.available())
    {
        static char receiverBuffer[128U]{};
        memset(receiverBuffer, '\0', sizeof(receiverBuffer));

        unsigned int bufferIndex{};
        unsigned long previousMicros = micros();

        while (true)
        {
            if (micros() - previousMicros < 6000UL && bufferIndex < sizeof(receiverBuffer) - 1)
            {
                if (Receiver::serial.available())
                {
                    receiverBuffer[bufferIndex] += static_cast<char>(Receiver::serial.read());
                    ++bufferIndex;

                    previousMicros = micros();
                }
            }
            else
            {
                break;
            }
        }

        static char messageBuffer[32U]{};
        memset(messageBuffer, '\0', sizeof(messageBuffer));

        const char *receiverBufferPointer = receiverBuffer;
        while (receiverBufferPointer < receiverBuffer + strlen(receiverBuffer))
        {
            receiverBufferPointer = Message::load(receiverBufferPointer, messageBuffer, sizeof(messageBuffer));

            if (isArraySet(messageBuffer, sizeof(messageBuffer)))
            {
                if (Message::isSafeForStrtok(messageBuffer, sizeof(messageBuffer)))
                {
                    Serial.print(F("[M] Message received: "));
                    Serial.print(messageBuffer);
                    Serial.println();

                    const char delimiters[]{Message::SEPARATOR, Message::SUBSEPARATOR, '\0'};

                    const unsigned int numberOfTokens = Message::countTokens(messageBuffer, strlen(messageBuffer));
                    if (numberOfTokens == 5U) // Regular message
                    {
                        const String from = strtok(messageBuffer, delimiters);
                        const String to = strtok(nullptr, delimiters);
                        const String node = strtok(nullptr, delimiters);
                        const String key = strtok(nullptr, delimiters);
                        const String value = strtok(nullptr, delimiters);

                        bool isVerified = false;
                        unsigned char keyType = 0;

                        if (to == This::ID.getIDInHexString() &&
                            verifyRepeaterNode(HexConverter::toUShort(from)) &&
                            verifySensorNode(HexConverter::toUShort(node)))
                        {
                            if (key == Node_SensorNode::KEY_CAR)
                            {
                                if (value.toInt() == 0 || value.toInt() == 1)
                                {
                                    isVerified = true;
                                    keyType = Node_SensorNode::Keys::CAR;
                                }
                            }
                            else if (key == Node_SensorNode::KEY_BAT)
                            {
                                if (value.toInt() == 1 || value.toInt() == 2 || value.toInt() == 3 || value.toInt() == 4)
                                {
                                    isVerified = true;
                                    keyType = Node_SensorNode::Keys::BAT;
                                }
                            }
                            else if (key == Node_SensorNode::KEY_RST)
                            {
                                if (value.toInt() == 1)
                                {
                                    isVerified = true;
                                    keyType = Node_SensorNode::Keys::RST;
                                }
                            }
                        }

                        if (isVerified)
                        {
                            String message;
                            message.reserve(18U);

                            message += '<';
                            message += from;
                            message += '/';
                            message += to;
                            message += F("/ACK/1>");

                            Serial.print(F("[M] Sending acknowledge: "));
                            Serial.print(message);
                            Serial.println();

                            Transmitter::serial.print(message);

                            const Query::Queue newQuery{keyType,
                                                        HexConverter::toUShort(node),
                                                        static_cast<unsigned char>(value.toInt())};

                            bool sameQueryFound = false;
                            if (!Query::queue.empty())
                            {
                                for (size_t index = 0U; index < Query::queue.size(); ++index)
                                {
                                    if (Query::queue.at(index) == newQuery)
                                    {
                                        sameQueryFound = true;
                                        break;
                                    }
                                }
                            }

                            if (!sameQueryFound)
                            {
                                Query::queue.push_back(newQuery);
                            }
                        }
                    }
                    else if (numberOfTokens == 4U) // ACK message
                    {
                        const String to = strtok(messageBuffer, delimiters);
                        const String from = strtok(nullptr, delimiters);
                        const String key = strtok(nullptr, delimiters);
                        const String value = strtok(nullptr, delimiters);

                        if (to == This::ID.getIDInHexString() &&
                            verifyRepeaterNode(HexConverter::toUShort(from)))
                        {
                            if (key == F("ACK") && value.toInt() == 1)
                            {
                                if (!Transmitter::queue.empty())
                                {
                                    if (Transmitter::queue.front().isSent &&
                                        Transmitter::queue.front().to.substring(5, 9) /* 5 to 9 is the second subtoken */ == from)
                                    {
                                        setDisplayValue(Transmitter::queue.front().id, Transmitter::queue.front().value);
                                        Transmitter::queue.pop_front();
                                    }
                                }
                            }
                        }
                    }
                }
            }

            memset(messageBuffer, '\0', sizeof(messageBuffer));
        }
    }

    // Transmission queue management routine, adds queues
    iterateDisplayNodes([]() {
        unsigned int currentDisplayValue;

        if (displayNodes[Node_DisplayNode::getPointer()].isMain())
        {
            currentDisplayValue = countMainDisplayValue();
        }
        else
        {
            currentDisplayValue = countDisplayValue(displayNodes[Node_DisplayNode::getPointer()].getDisplayID());
        }

        if (displayNodes[Node_DisplayNode::getPointer()].getDisplayValue() != currentDisplayValue)
        {
            Transmitter::Queue newTransmission{displayNodes[Node_DisplayNode::getPointer()].getDisplayID(),
                                               displayNodes[Node_DisplayNode::getPointer()].getDisplayRoute(),
                                               F("NUM"),
                                               static_cast<unsigned char>(currentDisplayValue)};

            bool sameTransmissionFound = false;
            if (!Transmitter::queue.empty())
            {
                for (size_t index = 0U; index < Transmitter::queue.size(); ++index)
                {
                    // The comparison operator used here does not compare the 'value' integer
                    if (Transmitter::queue.at(index) == newTransmission)
                    {
                        sameTransmissionFound = true;
                        break;
                    }
                }
            }

            if (!sameTransmissionFound)
            {
                Transmitter::queue.push_back(newTransmission);
            }
        }
    });

    // Transmission routine, transmits data from the front queue
    if (!Transmitter::queue.empty())
    {
        if (!Transmitter::queue.front().millisLimit ||
            Transmitter::queue.front().millisLimit < millis())
        {
            String message;
            message.reserve(18U);

            message += '<';
            message += Transmitter::queue.front().to;
            message += '/';
            message += This::ID.getIDInHexString();
            message += '/';
            message += Transmitter::queue.front().key;
            message += '/';
            message += Transmitter::queue.front().value;
            message += '>';

            Serial.print(F("[M] Sending update to LED matrix: "));
            Serial.print(message);
            Serial.println();

            Transmitter::serial.print(message);

            Transmitter::queue.front().millisLimit = millis() + Transmitter::timeLimit();
            Transmitter::queue.front().isSent = true;
        }
    }

    // Query routine, do until the queue is empty
    while (!Query::queue.empty())
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            if (Query::queue.front().type == Node_SensorNode::Keys::CAR)
            {
                post.addRequestData("node_id", HexConverter::toString(Query::queue.front().node_id).c_str());
                post.addRequestData("n_stats", String(Query::queue.front().value).c_str());

                Serial.print(F("[M] Updating "));
                Serial.print(HexConverter::toString(Query::queue.front().node_id));
                Serial.print(F(" in the database to "));
                Serial.print(Query::queue.front().value ? "PARK" : "EMPT");
                Serial.println();

                int httpCode;
                const String payload = post.getStringPayload("insert_node_status.php", &httpCode);

                if (httpCode == t_http_codes::HTTP_CODE_OK)
                {
                    if (payload == F("SUCCESS"))
                    {
                        iterateSensorNodes([]() {
                            if (sensorNodes[Node_SensorNode::getPointer()] == Query::queue.front().node_id)
                            {
                                sensorNodes[Node_SensorNode::getPointer()].setNodeStatus(Query::queue.front().value);
                                return;
                            }
                        });

                        Query::queue.pop_front();
                    }
                }
                else
                {
                    Serial.println(F("[M][E] Failed to retrieve data due to network or HTTP error"));
                    break;
                }
            }
            else if (Query::queue.front().type == Node_SensorNode::Keys::BAT)
            {
                post.addRequestData("node_id", HexConverter::toString(Query::queue.front().node_id).c_str());
                post.addRequestData("battery", String(Query::queue.front().value).c_str());

                int httpCode;
                const String payload = post.getStringPayload("insert_node_battery.php", &httpCode);

                if (httpCode == t_http_codes::HTTP_CODE_OK)
                {
                    if (payload == F("SUCCESS"))
                    {
                        iterateSensorNodes([]() {
                            if (sensorNodes[Node_SensorNode::getPointer()] == Query::queue.front().node_id)
                            {
                                sensorNodes[Node_SensorNode::getPointer()].setNodeBattery(Query::queue.front().value);
                                return;
                            }
                        });

                        Query::queue.pop_front();
                    }
                }
                else
                {
                    Serial.println(F("[M][E] Failed to retrieve data due to network or HTTP error"));
                    break;
                }
            }
            else if (Query::queue.front().type == Node_SensorNode::Keys::RST)
            {
                post.addRequestData("node_id", HexConverter::toString(Query::queue.front().node_id).c_str());

                int httpCode;
                const String payload = post.getStringPayload("insert_node_reset.php", &httpCode);

                if (httpCode == t_http_codes::HTTP_CODE_OK)
                {
                    if (payload == F("SUCCESS"))
                    {
                        Query::queue.pop_front();
                    }
                }
                else
                {
                    Serial.println(F("[M][E] Failed to retrieve data due to network or HTTP error"));
                    break;
                }
            }
        }
        else
        {
            break;
        }
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
        Serial.println(F("[M][E] Gateway was not found in the database"));

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
            HexConverter::toUShort(Query::document[index]["disp_id"]),
            encodeDisplayRoute(HexConverter::toUShort(Query::document[index]["disp_id"]),
                               HexConverter::toUShort(Query::document[index]["recv_rt"]),
                               Message::SUBSEPARATOR),
            Query::document[index]["is_main"]);

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
            HexConverter::toUShort(Query::document[index]["rptr_id"]),
            HexConverter::toUShort(Query::document[index]["send_rt"]));

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
            HexConverter::toUShort(Query::document[index]["node_id"]),
            HexConverter::toUShort(Query::document[index]["disp_rt"]));

        sensorNodes[Node_SensorNode::getPointer()].setNodeStatus(Query::document[index]["n_stats"].as<String>().toInt());  // Using toInt() to prevent conversion error
        sensorNodes[Node_SensorNode::getPointer()].setNodeBattery(Query::document[index]["battery"].as<String>().toInt()); // Using toInt() to prevent conversion error

        Node_SensorNode::preincrementPointer();
    }
}

const char *const Message::load(const char *const from, char *const to, const size_t toSize)
{
    if (!from || !to || !toSize)
    {
        return nullptr;
    }

    const char *start = nullptr;
    const char *end = nullptr;

    unsigned int substrings = 1U;

    const char *p = from;
    while (*p != '\0')
    {
        if (*p == START)
        {
            start = p;
            end = nullptr;

            substrings = 1U;
        }
        else if (*p == SEPARATOR)
        {
            ++substrings;
        }
        else if (*p == END)
        {
            end = p;
            ++p;

            break;
        }

        ++p;
    }

    if (!start)
    {
        // Serial.println(F("[M] No start character found"));
        return p;
    }
    else if (!end)
    {
        // Serial.println(F("[M] No end character found"));
        return p;
    }

    const char *f = start + 1;
    char *t = to;
    for (; f < end && t < to + toSize - 1; ++f, ++t)
    {
        *t = *f;
    }

    return p;
}

const bool Message::isSafeForStrtok(const char *const string, const size_t size)
{
    char previousP1 = SEPARATOR;
    char previousP2 = SUBSEPARATOR;

    if (*(string + size - 1) != '\0')
    {
        return false;
    }

    for (const char *p = string; p < string + size; ++p)
    {
        if ((previousP1 == SEPARATOR && *p == SEPARATOR) || (previousP2 == SUBSEPARATOR && *p == SUBSEPARATOR))
        {
            return false;
        }

        previousP1 = previousP2 = *p;
    }

    return true;
}

const unsigned int Message::countTokens(const char *const string, const size_t length)
{
    unsigned int count = 0;
    char previousP1 = SEPARATOR;
    char previousP2 = SUBSEPARATOR;

    for (const char *p = string; *p != '\0' && p <= string + length; ++p)
    {
        if ((previousP1 == SEPARATOR && *p != SEPARATOR) || (previousP2 == SUBSEPARATOR && *p != SUBSEPARATOR))
        {
            ++count;
        }

        previousP1 = previousP2 = *p;
    }

    return count;
}

const String encodeDisplayRoute(const String &displayID, const String &receiverRouteID, const char separator)
{
    return encodeDisplayRoute(
        HexConverter::toUShort(displayID),
        HexConverter::toUShort(receiverRouteID),
        separator);
}

const String encodeDisplayRoute(const unsigned short displayID, const unsigned short receiverRouteID, const char separator)
{
    std::vector<unsigned short> routes{displayID, receiverRouteID};

    unsigned int watchdogCounter = 0U;
    while (routes.back() != This::ID.getID())
    {
        for (Node_RepeaterNode::setPointerToHome();
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
    for (size_t index = routes.size() - 1; index < routes.size(); --index)
    {
        completeRoute += HexConverter::toString(routes.at(index));

        if (index)
        {
            completeRoute += separator;
        }
    }

    return completeRoute;
}

const unsigned int countDisplayValue(const String &displayID)
{
    return countDisplayValue(HexConverter::toUShort(displayID));
}

const unsigned int countDisplayValue(const unsigned short displayID)
{
    unsigned int count = 0U;

    for (Node_SensorNode::setPointerToHome();
         Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
         Node_SensorNode::preincrementPointer())
    {
        if (sensorNodes[Node_SensorNode::getPointer()].getDisplayID() == displayID &&
            !sensorNodes[Node_SensorNode::getPointer()].getNodeStatus())
        {
            ++count;
        }
    }

    return count;
}

const unsigned int countMainDisplayValue()
{
    unsigned int count = 0U;

    for (Node_SensorNode::setPointerToHome();
         Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
         Node_SensorNode::preincrementPointer())
    {
        if (!sensorNodes[Node_SensorNode::getPointer()].getNodeStatus())
        {
            ++count;
        }
    }

    return count;
}

void setDisplayValue(const String &displayID, const unsigned short displayValue)
{
    setDisplayValue(HexConverter::toUShort(displayID), displayValue);
}

void setDisplayValue(const unsigned short displayID, const unsigned short displayValue)
{
    for (Node_DisplayNode::setPointerToHome();
         Node_DisplayNode::getPointer() < Node_DisplayNode::getTotalNumberOfDisplays();
         Node_DisplayNode::preincrementPointer())
    {
        if (displayNodes[Node_DisplayNode::getPointer()] == displayID)
        {
            displayNodes[Node_DisplayNode::getPointer()].setDisplayValue(displayValue);
            break;
        }
    }
}

template <typename T>
const bool isArraySet(const T *const t, const size_t length)
{
    for (const T *p = t; p < t + length / sizeof(T); ++p)
    {
        if (*p != static_cast<T>(0))
        {
            return true;
        }
    }

    return false;
}

const bool verifySensorNode(const String &ID)
{
    return verifySensorNode(HexConverter::toUShort(ID));
}

const bool verifySensorNode(unsigned short ID)
{
    for (Node_SensorNode::setPointerToHome();
         Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
         Node_SensorNode::preincrementPointer())
    {
        if (sensorNodes[Node_SensorNode::getPointer()] == ID)
        {
            return true;
        }
    }

    return false;
}

const bool verifyRepeaterNode(const String &ID)
{
    return verifyRepeaterNode(HexConverter::toUShort(ID));
}

const bool verifyRepeaterNode(unsigned short ID)
{
    for (Node_RepeaterNode::setPointerToHome();
         Node_RepeaterNode::getPointer() < Node_RepeaterNode::getTotalNumberOfRepeaters();
         Node_RepeaterNode::preincrementPointer())
    {
        if (repeaterNodes[Node_RepeaterNode::getPointer()] == ID)
        {
            return true;
        }
    }

    return false;
}

// Doesn't support captures
void iterateSensorNodes(void (*f)())
{
    for (Node_SensorNode::setPointerToHome();
         Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
         Node_SensorNode::preincrementPointer())
    {
        f();
    }
}

void printSensorNodes()
{
    Node_SensorNode::printTableHeader();
    iterateSensorNodes([]() { sensorNodes[Node_SensorNode::getPointer()].printTable(); });
}

// Doesn't support captures
void iterateRepeaterNodes(void (*f)())
{
    for (Node_RepeaterNode::setPointerToHome();
         Node_RepeaterNode::getPointer() < Node_RepeaterNode::getTotalNumberOfRepeaters();
         Node_RepeaterNode::preincrementPointer())
    {
        f();
    }
}

void printRepeaterNodes()
{
    Node_RepeaterNode::printTableHeader();
    iterateRepeaterNodes([]() { repeaterNodes[Node_RepeaterNode::getPointer()].printTable(); });
}

// Doesn't support captures
void iterateDisplayNodes(void (*f)())
{
    for (Node_DisplayNode::setPointerToHome();
         Node_DisplayNode::getPointer() < Node_DisplayNode::getTotalNumberOfDisplays();
         Node_DisplayNode::preincrementPointer())
    {
        f();
    }
}

void printDisplayNodes()
{
    Node_DisplayNode::printTableHeader();
    iterateDisplayNodes([]() { displayNodes[Node_DisplayNode::getPointer()].printTable(); });
}

void printProcessTime(const char *const processName, void (*processes)())
{
    const unsigned long previousMicros = micros();
    processes();
    const unsigned long timeSpent = micros() - previousMicros;

    Serial.print(F("[M] Spent "));
    if (timeSpent > 1000UL)
    {
        Serial.print(timeSpent / 1000UL);
        Serial.print(F(" ms to process: "));
    }
    else
    {
        Serial.print(timeSpent);
        Serial.print(F(" us to process: "));
    }
    Serial.print(processName);
    Serial.println();
}