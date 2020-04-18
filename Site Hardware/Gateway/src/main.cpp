#include <deque>
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

struct Queue
{
    Queue(const unsigned short node_id, const String &t, const String &k, const unsigned char v) : id{node_id}, to{t}, key{k}, value{v} {}

    unsigned short id;   // Cannot be const due to erase member function in std::deque
    String to;           // Cannot be const due to erase member function in std::deque
    String key;          // Cannot be const due to erase member function in std::deque
    unsigned char value; // Cannot be const due to erase member function in std::deque

    bool isSent = false;
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
void printSensorNodes();

const bool verifySensorNode(const String &ID);
const bool verifySensorNode(unsigned short ID);

// Access this array only by the class-provided pointer
Node_RepeaterNode repeaterNodes[32U];
void printRepeaterNodes();

const bool verifyRepeaterNode(const String &ID);
const bool verifyRepeaterNode(unsigned short ID);

// Access this array only by the class-provided pointer
Node_DisplayNode displayNodes[32U];
void printDisplayNodes();

const String encodeDisplayRoute(const String &displayID, const String &receiverRouteID, const char separator);
const String encodeDisplayRoute(const unsigned short displayID, const unsigned short receiverRouteID, const char separator);

const unsigned int countDisplayValue(const String &displayID);
const unsigned int countDisplayValue(const unsigned short displayID);

void setDisplayValue(const String &displayID, const unsigned short displayValue);
void setDisplayValue(const unsigned short displayID, const unsigned short displayValue);

// Untested for user-defined types, use with fundamental data types only
template <typename T>
const bool isArraySet(const T *t, const size_t length);

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
        delay(0U);

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

    for (Node_DisplayNode::setPointerToHome();
         Node_DisplayNode::getPointer() < Node_DisplayNode::getTotalNumberOfDisplays();
         Node_DisplayNode::preincrementPointer())
    {
        displayNodes[Node_DisplayNode::getPointer()].setDisplayValue(countDisplayValue(displayNodes[Node_DisplayNode::getPointer()].getDisplayID()));
    }
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
                    const char delimiters[]{Message::SEPARATOR, '\0'};

                    const unsigned int numberOfTokens = Message::countTokens(messageBuffer, strlen(messageBuffer));
                    if (numberOfTokens == 5U) // Regular message
                    {
                        const String to = strtok(messageBuffer, delimiters);
                        const String from = strtok(nullptr, delimiters);
                        const String node = strtok(nullptr, delimiters);
                        const String key = strtok(nullptr, delimiters);
                        const String value = strtok(nullptr, delimiters);

                        if (to == This::ID.getIDInHexString() &&
                            verifyRepeaterNode(HexConverter::hexStringToUShort(from)) &&
                            verifySensorNode(HexConverter::hexStringToUShort(node)))
                        {
                            if (key == Node_SensorNode::KEY_CAR)
                            {
                                if (value.toInt() == 0 || value.toInt() == 1)
                                {
                                    String message;
                                    message.reserve(18U);

                                    message += '<';
                                    message += from;
                                    message += '/';
                                    message += to;
                                    message += F("/ACK/1>");

                                    Transmitter::serial.print(message);

                                    Query::Queue newQuery{Node_SensorNode::Keys::CAR,
                                                          HexConverter::hexStringToUShort(node),
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
                            else if (key == Node_SensorNode::KEY_BAT)
                            {
                                if (value.toInt() == 1 || value.toInt() == 2 || value.toInt() == 3 || value.toInt() == 4)
                                {
                                    String message;
                                    message.reserve(18U);

                                    message += '<';
                                    message += from;
                                    message += '/';
                                    message += to;
                                    message += F("/ACK/1>");

                                    Transmitter::serial.print(message);

                                    Query::Queue newQuery{Node_SensorNode::Keys::BAT,
                                                          HexConverter::hexStringToUShort(node),
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
                        }
                    }
                    // else if (numberOfTokens == 4U) // ACK message
                    // {
                    //     const String to = strtok(messageBuffer, delimiters);
                    //     const String from = strtok(nullptr, delimiters);
                    //     const String key = strtok(nullptr, delimiters);
                    //     const String value = strtok(nullptr, delimiters);

                    //     if (to == This::ID.getIDInHexString() &&
                    //         verifyRepeaterNode(HexConverter::hexStringToUShort(from)))
                    //     {
                    //         if (key == F("ACK") && value.toInt() == 1)
                    //         {
                    //             if (!Transmitter::queue.empty())
                    //             {
                    //                 for (size_t index = 0U; index < Transmitter::queue.size(); ++index)
                    //                 {
                    //                     if (Transmitter::queue.at(index).isSent && Transmitter::queue.at(index).to.startsWith(from))
                    //                     {
                    //                         setDisplayValue(Transmitter::queue.at(index).id, Transmitter::queue.at(index).value);
                    //                         Transmitter::queue.erase(Transmitter::queue.begin() + index);
                    //                     }
                    //                 }
                    //             }
                    //         }
                    //     }
                    // }
                }
            }

            memset(messageBuffer, '\0', sizeof(messageBuffer));
        }
    }

    // for (Node_DisplayNode::setPointerToHome();
    //      Node_DisplayNode::getPointer() < Node_DisplayNode::getTotalNumberOfDisplays();
    //      Node_DisplayNode::preincrementPointer())
    // {
    //     const unsigned int currentDisplayValue = countDisplayValue(displayNodes[Node_DisplayNode::getPointer()].getDisplayID());
    //     if (displayNodes[Node_DisplayNode::getPointer()].getDisplayValue() != currentDisplayValue)
    //     {
    //         Transmitter::Queue newTransmission{displayNodes[Node_DisplayNode::getPointer()].getDisplayID(),
    //                                            displayNodes[Node_DisplayNode::getPointer()].getDisplayRoute(),
    //                                            F("NUM"),
    //                                            static_cast<unsigned char>(currentDisplayValue)};

    //         bool sameTransmissionFound = false;
    //         for (size_t index = 0U; index < Transmitter::queue.size(); ++index)
    //         {
    //             if (Transmitter::queue.at(index).id == newTransmission.id)
    //             {
    //                 sameTransmissionFound = true;
    //                 break;
    //             }
    //         }

    //         if (!sameTransmissionFound)
    //         {
    //             Transmitter::queue.push_back(newTransmission);
    //         }
    //     }
    // }

    // for (size_t index = 0U; index < Transmitter::queue.size(); ++index)
    // {
    //     if (!Transmitter::queue.at(index).isSent)
    //     {
    //         String message;
    //         message.reserve(18U);

    //         message += '<';
    //         message += Transmitter::queue.at(index).to;
    //         message += '/';
    //         message += This::ID.getIDInHexString();
    //         message += '/';
    //         message += Transmitter::queue.at(index).key;
    //         message += '/';
    //         message += Transmitter::queue.at(index).value;
    //         message += '>';

    //         Transmitter::serial.print(message);

    //         Transmitter::queue.at(index).isSent = true;
    //     }
    // }

    if (WiFi.status() == WL_CONNECTED)
    {
        while (!Query::queue.empty())
        {
            if (Query::queue.front().type == Node_SensorNode::Keys::CAR)
            {
                post.addRequestData("node_id", HexConverter::UIntToHexString(Query::queue.front().node_id).c_str());
                post.addRequestData("n_stats", String(Query::queue.front().value).c_str());

                int httpCode;
                const String payload = post.getStringPayload("insert_node_status.php", &httpCode);

                if (httpCode == t_http_codes::HTTP_CODE_OK)
                {
                    if (payload == F("SUCCESS"))
                    {
                        for (Node_SensorNode::setPointerToHome();
                             Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
                             Node_SensorNode::preincrementPointer())
                        {
                            if (sensorNodes[Node_SensorNode::getPointer()] == Query::queue.front().node_id)
                            {
                                sensorNodes[Node_SensorNode::getPointer()].setNodeStatus(Query::queue.front().value);
                                break;
                            }
                        }

                        Query::queue.pop_front();
                    }
                }
                else
                {
                    Serial.println(F("[M][E] Failed to retrieve data due to network or HTTP error"));
                }
            }
            else if (Query::queue.front().type == Node_SensorNode::Keys::BAT)
            {
                post.addRequestData("node_id", HexConverter::UIntToHexString(Query::queue.front().node_id).c_str());
                post.addRequestData("battery", String(Query::queue.front().value).c_str());

                int httpCode;
                const String payload = post.getStringPayload("insert_node_battery.php", &httpCode);

                if (httpCode == t_http_codes::HTTP_CODE_OK)
                {
                    if (payload == F("SUCCESS"))
                    {
                        for (Node_SensorNode::setPointerToHome();
                             Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
                             Node_SensorNode::preincrementPointer())
                        {
                            if (sensorNodes[Node_SensorNode::getPointer()] == Query::queue.front().node_id)
                            {
                                sensorNodes[Node_SensorNode::getPointer()].setNodeBattery(Query::queue.front().value);
                                break;
                            }
                        }

                        Query::queue.pop_front();
                    }
                }
                else
                {
                    Serial.println(F("[M][E] Failed to retrieve data due to network or HTTP error"));
                }
            }
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

        sensorNodes[Node_SensorNode::getPointer()].setNodeStatus(Query::document[index]["n_stats"]);
        sensorNodes[Node_SensorNode::getPointer()].setNodeBattery(Query::document[index]["battery"]);

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
    char previousP = SEPARATOR;

    if (*(string + size - 1) != '\0')
    {
        return false;
    }

    for (const char *p = string; p < string + size; ++p)
    {
        if (previousP == SEPARATOR && *p == SEPARATOR)
        {
            return false;
        }

        previousP = *p;
    }

    return true;
}

const unsigned int Message::countTokens(const char *const string, const size_t length)
{
    unsigned int count = 0;
    char previousP = SEPARATOR;

    for (const char *p = string; *p != '\0' && p <= string + length; ++p)
    {
        if (previousP == SEPARATOR && *p != SEPARATOR)
        {
            ++count;
        }

        previousP = *p;
    }

    return count;
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
    for (size_t index = routes.size() - 2 /* Ignore the last data in the vector */; index < routes.size(); --index)
    {
        completeRoute += HexConverter::UIntToHexString(routes.at(index));

        if (index)
        {
            completeRoute += separator;
        }
    }

    return completeRoute;
}

const unsigned int countDisplayValue(const String &displayID)
{
    return countDisplayValue(HexConverter::hexStringToUShort(displayID));
}

const unsigned int countDisplayValue(const unsigned short displayID)
{
    unsigned int count = 0U;

    for (Node_DisplayNode::setPointerToHome();
         Node_DisplayNode::getPointer() < Node_DisplayNode::getTotalNumberOfDisplays();
         Node_DisplayNode::preincrementPointer())
    {
        if (displayNodes[Node_DisplayNode::getPointer()] == displayID)
        {
            for (Node_SensorNode::setPointerToHome();
                 Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
                 Node_SensorNode::preincrementPointer())
            {
                if (sensorNodes[Node_SensorNode::getPointer()].getDisplayID() == displayID)
                {
                    ++count;
                }
            }

            break;
        }
    }

    return count;
}

void setDisplayValue(const String &displayID, const unsigned short displayValue)
{
    setDisplayValue(HexConverter::hexStringToUShort(displayID), displayValue);
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
    return verifySensorNode(HexConverter::hexStringToUShort(ID));
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
    return verifyRepeaterNode(HexConverter::hexStringToUShort(ID));
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

void printSensorNodes()
{
    Node_SensorNode::printTableHeader();
    for (Node_SensorNode::setPointerToHome();
         Node_SensorNode::getPointer() < Node_SensorNode::getTotalNumberOfNodes();
         Node_SensorNode::preincrementPointer())
    {
        sensorNodes[Node_SensorNode::getPointer()].printTable();
    }
}

void printRepeaterNodes()
{
    Node_RepeaterNode::printTableHeader();
    for (Node_RepeaterNode::setPointerToHome();
         Node_RepeaterNode::getPointer() < Node_RepeaterNode::getTotalNumberOfRepeaters();
         Node_RepeaterNode::preincrementPointer())
    {
        repeaterNodes[Node_RepeaterNode::getPointer()].printTable();
    }
}

void printDisplayNodes()
{
    Node_DisplayNode::printTableHeader();
    for (Node_DisplayNode::setPointerToHome();
         Node_DisplayNode::getPointer() < Node_DisplayNode::getTotalNumberOfDisplays();
         Node_DisplayNode::preincrementPointer())
    {
        displayNodes[Node_DisplayNode::getPointer()].printTable();
    }
}