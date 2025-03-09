/*******************************************************************
   A simple example for reading time from Oracle APEX RESTful API

    Written by Ata Can Yaymacı
    Medium: https://medium.com/@atacanymc
    Github: https://github.com/AtaCanYmc
 *******************************************************************/

#include "ApexBridge.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "<your-ssid>";
const char* password = "<your-password>";

X509List cert(ORACLE_APEX_CERTIFICATE);
WiFiClientSecure clientApex;
String schema = "<your-schema>";
String base_path = "/pls/apex";

void setup() {
    Serial.begin(115200);
    clientApex.setTrustAnchors(&cert); // activate certificate for connection
    connectWifi(ssid, password);
    ApexBridge apex = ApexBridge(schema, base_path, clientApex);

    // /pls/apex/<your-schema>/<your-module>/<your-resource>
    apex.prepareURL("time","now");
    DynamicJsonDocument doc(1024);
    doc = apex.sendRequest();
    Serial.println(doc["full_timestamp"].as<String>());
    Serial.println(doc["year"].as<int>());
}

void loop() {

}

IPAddress connectWifi(const char* ssid, const char* pwd){
    IPAddress ip;
    Serial.print("[Wifi] Connection found on: ");
    Serial.print(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);
    Serial.print("\n[Wifi] Connecting to WiFi ..");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }
    Serial.println("\n[Wifi] Wifi Connected");

    Serial.print("[Wifi] RRSI: ");
    Serial.println(WiFi.RSSI());
    ip = WiFi.localIP();
    Serial.print("[Wifi] IP ADDRESS: ");
    Serial.println(ip);
    return ip;
}
