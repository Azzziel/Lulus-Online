#include <Arduino.h>
#include <SoftwareSerial.h>

const unsigned long MONITOR_SPEED = 115200UL;

namespace HC12
{
const unsigned int RX = 9U;
const unsigned int TX = 8U;

const unsigned int SET = 7U;

const bool AT_COMMAND_MODE = LOW;
const bool TRANSMISSION_MODE = HIGH;

// Notice: this library might fail to detect responses in high baudrates due to the nature of SoftwareSerial
const unsigned long BAUDRATES[]{1200UL, 2400UL, 4800UL, 9600UL, 19200UL, 38400UL, 57600UL, 115200UL};

void begin(const unsigned long baudrate, const unsigned int channel);

void setToATCommandMode();
void setToTransmissionMode();

void resetToATCommandMode();
void resetToTransmissionMode();

const String getResponse(const unsigned long timeout = 40UL /* Timeout's safe for 8 MHz CPU */);

const bool testAT();

const unsigned long checkBaudrate();
const bool changeBaudrate(const unsigned long from, const unsigned long to);

const unsigned int checkChannel();
const bool changeChannel(const unsigned int channel);
} // namespace HC12

SoftwareSerial sorial(HC12::TX, HC12::RX);

void setup()
{
    Serial.begin(MONITOR_SPEED);
    HC12::begin(2400UL, 10U);
}

void loop()
{
}

void HC12::begin(const unsigned long baudrate, const unsigned int channel)
{
    pinMode(HC12::SET, OUTPUT);
    setToATCommandMode();

    changeBaudrate(checkBaudrate(), baudrate);

    sorial.begin(baudrate);
    if (channel != 0 && channel != checkChannel())
    {
        if (changeChannel(channel))
        {
            resetToTransmissionMode();
        }
    }
}

void HC12::setToATCommandMode()
{
    digitalWrite(HC12::SET, LOW);
    delay(40);
}

void HC12::setToTransmissionMode()
{
    digitalWrite(HC12::SET, HIGH);
    delay(80);
}

void HC12::resetToATCommandMode()
{
    setToTransmissionMode();
    setToATCommandMode();
}

void HC12::resetToTransmissionMode()
{
    setToATCommandMode();
    setToTransmissionMode();
}

const String HC12::getResponse(const unsigned long timeout)
{
    String response = "";

    const unsigned long previousMillis = millis();
    while (millis() - previousMillis < timeout)
    {
        if (sorial.available())
        {
            do
            {
                response += static_cast<char>(sorial.read());
                delay(1);
            } while (sorial.available());
            response.trim();

            Serial.print("[R] ");
            Serial.print(response);
            Serial.println();

            break;
        }
    }

    return response;
}

const unsigned long HC12::checkBaudrate()
{
    sorial.end();

    for (size_t index = 0; index < sizeof(BAUDRATES) / sizeof(const unsigned long); ++index)
    {
        sorial.begin(BAUDRATES[index]);

        if (testAT())
        {
            Serial.print("[M] Current baudrate found at ");
            Serial.print(BAUDRATES[index]);
            Serial.println();

            sorial.end();

            return BAUDRATES[index];
        }

        sorial.end();
    }

    Serial.println("[M][E] OK response was not detected from the device");
    return 0;
}

const bool HC12::testAT()
{
    sorial.print("AT");
    return getResponse() == "OK";
}

const bool HC12::changeBaudrate(const unsigned long from, const unsigned long to)
{
    if (from == to)
        return true;

    sorial.end();
    sorial.begin(from);

    sorial.print("AT+B");
    sorial.print(to);

    if (getResponse() == String("OK+B") + to)
    {
        sorial.end();
        resetToATCommandMode();

        sorial.begin(to);

        if (testAT())
        {
            sorial.end();

            Serial.print("[M] Changing baudrate from ");
            Serial.print(from);
            Serial.print(" to ");
            Serial.print(to);
            Serial.print(" was successful");
            Serial.println();

            return true;
        }
    }

    sorial.end();
    return false;
}

const unsigned int HC12::checkChannel()
{
    sorial.print("AT+RC");

    String response = getResponse();
    if (response.startsWith("OK+RC"))
    {
        response.remove(0, 5);

        Serial.print("[M] Channel is currently at ");
        Serial.print(response);
        Serial.println();

        return static_cast<unsigned int>(response.toInt());
    }
    else
    {
        return 0;
    }
}

const bool HC12::changeChannel(const unsigned int channel)
{
    if (channel > 127) // More than the number of channels available
    {
        return false;
    }

    String channelInString;
    if (channel > 99) // Three digits
    {
        channelInString += channel;
    }
    else if (channel > 9) // Two digits
    {
        channelInString += "0";
        channelInString += channel;
    }
    else // One digit
    {
        channelInString += "00";
        channelInString += channel;
    }

    sorial.print("AT+C");
    sorial.print(channelInString);

    if (getResponse() == String("OK+C") + channelInString)
    {
        Serial.print("[M] Changing channel to ");
        Serial.print(channelInString);
        Serial.print(" was successful");
        Serial.println();

        return true;
    }

    return false;
}