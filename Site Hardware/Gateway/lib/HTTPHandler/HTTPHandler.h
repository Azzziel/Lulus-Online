#ifndef HTTP_HANDLER_h
#define HTTP_HANDLER_h

#include <vector>
#include <Arduino.h>
#include <HTTPClient.h>

class HTTPHandler
{
    class HTTPRequestData
    {
    public:
        HTTPRequestData(const char *const key, const char *const value) : key{key}, value{value} {}
        HTTPRequestData(const char *const key, const String &value) : key{key}, value{value} {}
        HTTPRequestData(const String &key, const char *const value) : key{key}, value{value} {}
        HTTPRequestData(const String &key, const String &value) : key{key}, value{value} {}

        const String getKey() { return key; }
        const String getValue() { return value; }

    private:
        const String key;
        const String value;
    };

public:
    HTTPHandler(HTTPClient *const httpClient, const char *const baseUrl, const char *const apiKey)
        : httpClient{httpClient}, baseUrl{baseUrl}, apiKey{apiKey} {}

    void resetRequestData();
    void addRequestData(const char *const key, const char *const value) { requestData.push_back(HTTPRequestData(key, value)); }
    void addRequestData(const char *const key, const String &value) { requestData.push_back(HTTPRequestData(key, value)); }
    void addRequestData(const String &key, const char *const value) { requestData.push_back(HTTPRequestData(key, value)); }
    void addRequestData(const String &key, const String &value) { requestData.push_back(HTTPRequestData(key, value)); }

    const String getStringPayloadViaGET(const char *const route, int *httpCode);
    const String getStringPayloadViaPOST(const char *const route, int *httpCode);

private:
    const String buildGETRequestData();
    const String buildPOSTRequestData();

    HTTPClient *const httpClient;
    const String baseUrl;
    const String apiKey;

    std::vector<HTTPRequestData> requestData;
};

#endif