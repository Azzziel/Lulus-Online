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
#include <Node_ID.h>

const unsigned long MONITOR_SPEED = 115200UL;

const char WIFI_SSID[] PROGMEM = "HOME-NETWORK";
const char WIFI_PASS[] PROGMEM = ":Waffle/192/Licious:";
const char SERVER_URL[] PROGMEM = "http://mejakalori.xyz/NodeTest/";
const char SERVER_KEY[] PROGMEM = "2fad7ae4be04ce093c8a9e48511fdf6c";

namespace ThisGateway
{
namespace ID
{
const unsigned int SAVE_ADDRESS = 0U;
const char SIGNATURE = 'G';

Node_ID ID;
} // namespace ID
namespace Query
{
const unsigned int limit = 4U;
}
} // namespace ThisGateway

Divider divider;

HTTPClient client;
POSTHandler post(&client, SERVER_URL, SERVER_KEY);

void setup()
{
    Serial.begin(MONITOR_SPEED);

    EEPROM.begin(sizeof(Node_ID::ID_t));
    ThisGateway::ID::ID.loadIDFromEEPROM(ThisGateway::ID::SAVE_ADDRESS, ThisGateway::ID::SIGNATURE);
    EEPROM.end();

    if (!ThisGateway::ID::ID.getID())
    {
        Serial.println(F("[M][E] EEPROM corrupt or not set"));

        while (1) // Abort operation
            delay(0);
    }

    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
        delay(0);

    int httpCode;
    const String payload = post.getStringPayload("count_node_devices.php", &httpCode);

    if (httpCode == t_http_codes::HTTP_CODE_OK)
    {
        divider.loadDivider(payload.toInt(), ThisGateway::Query::limit);
    }
    else
    {
        Serial.println(F("[M][E] Failed to get total number of devices"));

        while (1) // Abort operation
            delay(0);
    }

    // JSON decode here

    divider.clearArray();

    digitalWrite(BUILTIN_LED, LOW);
}

void loop()
{
}