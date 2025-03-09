/*******************************************************************
A simple example for reading time from Oracle APEX RESTful API

   Written by Ata Can Yaymacı
   Medium: https://medium.com/@atacanymc
   Github: https://github.com/AtaCanYmc
 *******************************************************************/

#include "ApexBridge.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "<your-ssid>";
const char* password = "<your-password>";

WiFiClientSecure clientApex;
String schema = "<your-schema>";
String base_path = "/pls/apex";

void setup() {
    Serial.begin(115200);
    connectWifi(ssid, password);

    clientApex.setCACert(apex_cert); // activate certificate for connection
    ApexBridge apex = ApexBridge(schema, base_path, clientApex);

    // /pls/apex/<your-schema>/<your-module>/<your-resource>
    apex.prepareURL("time", "now");
    DynamicJsonDocument doc(1024);
    doc = apex.sendRequest();

    Serial.println(doc["full_timestamp"].as<String>());
    Serial.println(doc["year"].as<int>());
}

void loop() {
    // No need that part for this example
}

IPAddress connectWifi(const char* ssid, const char* pwd) {
    IPAddress ip;
    Serial.print("[WiFi] Connecting to: ");
    Serial.println(ssid);

    WiFi.begin(ssid, pwd);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 20) {
        Serial.print(".");
        delay(500);
        attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFi] Connected!");
        Serial.print("[WiFi] IP Address: ");
        ip = WiFi.localIP();
        Serial.println(ip);
    } else {
        Serial.println("\n[WiFi] Connection Failed!");
    }

    return ip;
}
