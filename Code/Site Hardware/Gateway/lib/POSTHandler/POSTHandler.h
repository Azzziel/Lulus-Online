#ifndef BERAS_POST_HANDLER_h
#define BERAS_POST_HANDLER_h

#include <Arduino.h>
#include <HTTPClient.h>

class POSTHandler
{
public:
    POSTHandler(HTTPClient *const httpClient, const char *const hostingUrl, const char *const phpAuthenticationKey);
    ~POSTHandler() { clearRequestData(); }

    void resetRequestData();
    void addRequestData(const char *const arrayKeyName, const char *const arrayValue);

    const String getStringPayload(const char *const phpFilename, int *const httpResponseCode);

private:
    void clearRequestData();

    HTTPClient *const http;
    const String url;
    const String authKey;

    String *requestData = nullptr;
};

#endif