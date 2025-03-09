//
// Created by Ata Can Yaymacı on 2.03.2025.
//

#include "ApexBridge.h"

// This is the constructor of the ApexBridge class.
ApexBridge::ApexBridge(String schema, String base_path, Client &client) {
    this->app.schema = schema;
    this->app.base_path = base_path;
    this->client = &client;
}

// This function is used to set the token of the app.
void ApexBridge::setToken(String token) {
    this->app.token = token;
}

// This function is used to prepare the URL for the request.
// It takes a schema, a module and a resource name and returns the full URL.
// The URL is stored in the last_endpoint variable.
String ApexBridge::prepareURL(String schema, String module, String resource) {
    String url = app.base_path + "/" + schema + "/" + module + "/" + resource;
    this->last_endpoint = url;
    return url;
}

// This function is used to prepare the URL for the request.
// It takes a resource name and returns the full URL.
// The URL is stored in the last_endpoint variable.
String ApexBridge::prepareURL(String module, String resource) {
    return prepareURL(app.schema, module, resource);
}

// This function is used to add a parameter to the last_endpoint variable.
// It takes a parameter name and a value.
void ApexBridge::addParameter(String param, String value) {
    if (last_endpoint.indexOf("?") == -1) {
        last_endpoint += "?";
    } else {
        last_endpoint += "&";
    }
    last_endpoint += param + "=" + value;
}

// This function is used to add a parameter to a given URL.
// It takes a URL, a parameter name and a value.
void ApexBridge::addParameter(String &url, String param, String value) {
    if (url.indexOf("?") == -1) {
        url += "?";
    } else {
        url += "&";
    }
    url += param + "=" + value;
}

// This function is used to add a path to the last_endpoint variable.
// It takes a path and adds it to the last_endpoint variable.
void ApexBridge::addPath(String path) {
    last_endpoint += "/" + path;
}

// This function is used to add a path to a given URL.
void ApexBridge::addPath(String &url, String path) {
    url += "/" + path;
}

// This function is used to send a request to the last_endpoint.
DynamicJsonDocument ApexBridge::sendRequest(DynamicJsonDocument payload, const char* method) {
    return sendRequest(last_endpoint.c_str(), payload, method);
}

// This function is used to send a request to a given URL.
DynamicJsonDocument ApexBridge::sendRequest(const char* url, DynamicJsonDocument payload, const char* method) {
	DynamicJsonDocument doc(1024);
    if (!connectApexClient()) {
        return doc;
    }

    if (strcmp(method, "GET") == 0) {
        String response = sendGet(url);
        deserializeJson(doc, response);
    } else if (strcmp(method, "POST") == 0) {
        String response = sendPost(url, payload);
        deserializeJson(doc, response);
    }

    closeApexClient();
    return doc;
}

DynamicJsonDocument ApexBridge::sendRequest(const char* method) {
  	DynamicJsonDocument emptyPayload(1024);
    return sendRequest(last_endpoint.c_str(), emptyPayload, method);
}

// This function is used to send a GET request to a given URL.
String ApexBridge::sendGet(const char* url) {
    client->print("GET ");
    client->print(url);
    client->println(" HTTP/1.1");
    client->print("Host: ");
    client->println(APEX_HOST);
	if (app.token.length() > 1) {
        client->print("Authorization: Bearer ");
        client->println(app.token);
    }
    client->println("Connection: close");
    client->println();

    String body, headers;
    if (!readHTTP(body, headers)) {
        return "";
    }

    return body;
}

// This function is used to send a POST request to a given URL.
String ApexBridge::sendPost(const char* url, DynamicJsonDocument payload) {
    String json;
    serializeJson(payload, json);

    client->print("POST ");
    client->print(url);
    client->println(" HTTP/1.1");
    client->print("Host: ");
    client->println(APEX_HOST);
   	if (app.token.length() > 1) {
        client->print("Authorization: Bearer ");
        client->println(app.token);
    }
    client->println("Connection: close");
    client->println("Content-Type: application/json");
    client->print("Content-Length: ");
    client->println(json.length());
    client->println();
    client->println(json);

    String body, headers;
    if (!readHTTP(body, headers)) {
        return "";
    }

    return body;
}

// This function is used to read the HTTP response.
bool ApexBridge::readHTTP(String &body, String &headers) {
    int ch_count = 0;
    unsigned long now = millis();
    bool finishedHeaders = false;
    bool currentLineIsBlank = true;
    bool responseReceived = false;

    while (millis() - now < MAX_RESPONSE_TIME) {
        while (client->available()) {
            char c = client->read();
            responseReceived = true;

            if (!finishedHeaders) {
                if (currentLineIsBlank && c == '\n') {
                    finishedHeaders = true;
                } else {
                    headers += c;
                }
            } else {
                if (ch_count < MAX_BUFFER_SIZE) {
                    body += c;
                    ch_count++;
                }
            }

            if (c == '\n') currentLineIsBlank = true;
            else if (c != '\r') currentLineIsBlank = false;
        }

        if (responseReceived) {
            Serial.println(body);
            break;
        }
    }

    if (!responseReceived) {
        log(F("[APEX BRIDGE] - No response received"));
    }
    return responseReceived;
}

// This function is used to connect to the Apex client.
bool ApexBridge::connectApexClient() {
    if (!client->connect(APEX_HOST, APEX_PORT)) {
        log(F("[APEX BRIDGE] - Connection failed"));
        return false;
    }
    return true;
}

// This function is used to close the Apex client.
void ApexBridge::closeApexClient() {
    if (client->connected()) {
        client->stop();
    }
}

// This function is used to log a message.
void ApexBridge::log(String message) {
    #ifdef BRIDGE_DEBUG
    Serial.println(message);
    #endif
}

