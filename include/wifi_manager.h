#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

class WiFiManager {
public:
    void begin();
    bool isConnected();
    void reconnect();
    void handleAPMode();
    String getIP();
    int getRSSI();
    
private:
    bool apMode = false;
    AsyncWebServer* server = nullptr;
    void setupAP();
    void setupWebServer();
};

#endif // WIFI_MANAGER_H
