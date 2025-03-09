# ApexBridge for ESP Devices

ApexBridge is a lightweight and efficient library designed for ESP8266 and ESP32 devices to seamlessly interact with Oracle APEX RESTful services. It simplifies sending authenticated HTTP requests and managing API endpoints securely.

## ✨ Features
- 🔗 **Seamless API Communication**: Easily interact with Oracle APEX RESTful services.
- 🔒 **Secure Connections**: Supports HTTPS with certificate validation.
- 🔑 **Token-Based Authentication**: Handles Bearer token authentication.
- 🛠 **Built-in URL Management**: Automatically constructs request URLs based on schema, module, and resources.
- ⚡ **Lightweight & Efficient**: Designed for low-power IoT devices.

## 📦 Installation
1. Copy the `ApexBridge.h` and `ApexBridge.cpp` files into your Arduino project folder.
2. Include them in your sketch.

## 🔧 Dependencies
This library relies on the following:
- [`WiFiClientSecure`](https://www.arduino.cc/reference/en/libraries/wificlientsecure/) for HTTPS requests.
- [`ArduinoJson`](https://arduinojson.org/) for parsing JSON responses.

## 🚀 Getting Started
### 1️⃣ Include the Library
```cpp
#include "ApexBridge.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
```

### 2️⃣ Define WiFi Credentials
```cpp
String ssid = "<your-ssid>";
String password = "<your-password>";
```

### 3️⃣ Set Up the Secure Client
```cpp
X509List cert(ORACLE_APEX_CERTIFICATE);
WiFiClientSecure clientApex;
```

### 4️⃣ Initialize ApexBridge
```cpp
String schema = "<your-schema>";
String base_path = "/pls/apex";
ApexBridge apex = ApexBridge(schema, base_path, clientApex);
```

### 5️⃣ Connect to WiFi
```cpp
void connectWifi(const char* ssid, const char* pwd) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("WiFi Connected");
}
```

### 6️⃣ Send a Request
```cpp
apex.prepareURL("time", "now");
DynamicJsonDocument doc = apex.sendRequest("GET");
Serial.println(doc["full_timestamp"].as<String>());
```

## 📖 API Reference

### `void setToken(String token)`
Sets the authentication token for API requests.

### `String prepareURL(String module, String resource)`
Builds the full URL using the schema and provided module/resource.

### `DynamicJsonDocument sendRequest(const char* method)`
Sends a request using the specified HTTP method (`GET` or `POST`).

### `void addParameter(String param, String value)`
Adds query parameters to the request URL.

## 🛠 Example Use Case
```cpp
apex.setToken("your-jwt-token");
apex.prepareURL("sensor", "data");
apex.addParameter("id", "123");
DynamicJsonDocument response = apex.sendRequest("GET");
Serial.println(response["status"].as<String>());
```

## 📜 License
This project is licensed under the MIT License. See `LICENSE` for details.

## 📬 Contact & Support
- **Author**: Ata Can Yaymacı
- **Medium**: [@atacanymc](https://medium.com/@atacanymc)
- **GitHub**: [AtaCanYmc](https://github.com/AtaCanYmc)

If you find this library useful, consider giving it a ⭐ on GitHub!

---

🚀 *Happy coding with ApexBridge!*

