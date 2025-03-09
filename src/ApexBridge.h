//
// Created by Ata Can Yaymacı on 2.03.2025.
//

#ifndef APEXBRIDGE_H
#define APEXBRIDGE_H

#include <Arduino.h>
#include <Client.h>
#include <ArduinoJson.h>
#include <Client.h>
#include "OracleApexCertificate.h"

#define MAX_BUFFER_SIZE 102400000
#define MAX_RESPONSE_TIME 5000
#define APEX_HOST "apex.oracle.com"
#define APEX_PORT 443
// #define BRIDGE_DEBUG 1 // Uncomment this line to enable debug messages

struct ApexApp {
    String schema;
    String token = "";
    String base_path = "/pls/apex";
};

class ApexBridge {
public:
    ApexBridge(String schema, String base_path, Client &client);
    void setToken(String token);
    String prepareURL(String schema, String module, String resource);
    String prepareURL(String module, String resource);
    void addParameter(String param, String value);
    void addParameter(String &url, String param, String value);
    void addPath(String path);
    void addPath(String &url, String path);
    DynamicJsonDocument sendRequest(const char* method="GET");
    DynamicJsonDocument sendRequest(DynamicJsonDocument payload, const char* method="GET");
    DynamicJsonDocument sendRequest(const char* url, DynamicJsonDocument payload, const char* method="GET");
private:
    Client *client;
    ApexApp app;
    String last_endpoint;

    String sendGet(const char* url);
    String sendPost(const char* url, DynamicJsonDocument payload);
    bool readHTTP(String &body, String &headers);
    bool connectApexClient();
    void closeApexClient();
    void log(String message);
};


#endif //APEXBRIDGE_H
