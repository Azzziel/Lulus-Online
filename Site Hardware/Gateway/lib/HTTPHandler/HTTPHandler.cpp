#include <vector>
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "HTTPHandler.h"

void HTTPHandler::resetRequestData()
{
    requestData.clear();
    requestData.shrink_to_fit();
}

const String HTTPHandler::getStringPayloadViaGET(const char *route, int *httpCode)
{
    httpClient->begin(baseUrl + route + buildGETRequestData());
    httpClient->addHeader("Content-Type", "application/json");
    httpClient->addHeader("x-api-key", apiKey);

    *httpCode = httpClient->GET();
    const String response = httpClient->getString();

    httpClient->end();

    resetRequestData();
    return response;
}

const String HTTPHandler::getStringPayloadViaPOST(const char *route, int *httpCode)
{
    httpClient->begin(baseUrl + route);
    httpClient->addHeader("Content-Type", "application/json");
    httpClient->addHeader("x-api-key", apiKey);

    *httpCode = httpClient->POST(buildPOSTRequestData());
    const String response = httpClient->getString();

    httpClient->end();

    resetRequestData();
    return response;
}

const String HTTPHandler::buildGETRequestData()
{
    String requestString;

    for (size_t i = 0; i < requestData.size(); ++i)
    {
        if (i > 0)
        {
            requestString += '&';
        }
        else
        {
            requestString += '?';
        }

        requestString += requestData.operator[](i).getKey();
        requestString += '=';
        requestString += requestData.operator[](i).getValue();
    }

    return requestString;
}

const String HTTPHandler::buildPOSTRequestData()
{
    String requestString;
    StaticJsonDocument<256> doc;

    for (size_t i = 0; i < requestData.size(); ++i)
    {
        doc[requestData.operator[](i).getKey()] = requestData.operator[](i).getValue();
    }

    serializeJson(doc, requestString);
    return requestString;
}
