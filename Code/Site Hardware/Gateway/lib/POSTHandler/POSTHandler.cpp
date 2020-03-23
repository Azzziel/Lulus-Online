#include <Arduino.h>
#include <HTTPClient.h>

#include "POSTHandler.h"

POSTHandler::POSTHandler(HTTPClient *const httpClient, const char *const hostingUrl, const char *const phpAuthenticationKey) : http{httpClient}, url{hostingUrl}, authKey{phpAuthenticationKey}
{
    resetRequestData();
}

POSTHandler::~POSTHandler()
{
    clearRequestDataFromHeap();
}

void POSTHandler::resetRequestData()
{
    clearRequestDataFromHeap();
    requestData = new String();

    *requestData += "auth_key=";
    *requestData += authKey;
}

void POSTHandler::addRequestData(const char *const arrayKeyName, const char *const arrayValue)
{
    *requestData += '&';
    *requestData += arrayKeyName;
    *requestData += '=';
    *requestData += arrayValue;
}

const String POSTHandler::getStringPayload(const char *phpFilename, int *httpResponseCode)
{
    http->begin(url + phpFilename);
    http->addHeader("Content-Type", "application/x-www-form-urlencoded");

    *httpResponseCode = http->POST(*requestData);
    const String payload = http->getString();

    http->end();

    resetRequestData();

    return payload;
}

void POSTHandler::clearRequestDataFromHeap()
{
    delete requestData;
    requestData = nullptr;
}