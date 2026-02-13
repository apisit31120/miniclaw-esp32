#include "wifi_manager.h"
#include "config.h"
#include "secrets.h"
#include <AsyncTCP.h>

void WiFiManager::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40) {
        delay(500);
        Serial.print(".");
        attempts++;
        if (attempts % 10 == 0) {
            Serial.println("\nRetrying...");
            WiFi.reconnect();
        }
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✓ WiFi Connected!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        apMode = false;
    } else {
        Serial.println("\n✗ WiFi Failed! Starting AP Mode...");
        setupAP();
    }
}

void WiFiManager::setupAP() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("MiniClaw-Setup", "miniclaw123");
    apMode = true;
    
    Serial.println("AP Mode Started");
    Serial.print("SSID: MiniClaw-Setup\nPassword: miniclaw123\n");
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
    
    setupWebServer();
}

void WiFiManager::setupWebServer() {
    server = new AsyncWebServer(80);
    
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        String html = "<!DOCTYPE html><html><head>";
        html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        html += "<title>MiniClaw Setup</title>";
        html += "<style>body{font-family:Arial;margin:20px;background:#1a1a1a;color:#fff}";
        html += ".container{max-width:400px;margin:0 auto;background:#2a2a2a;padding:20px;border-radius:10px}";
        html += "input{width:100%;padding:10px;margin:5px 0;border-radius:5px;border:none;box-sizing:border-box}";
        html += "button{width:100%;padding:12px;background:#00d26a;color:#000;border:none;border-radius:5px;cursor:pointer;font-weight:bold}";
        html += "h2{color:#00d26a}</style></head><body>";
        html += "<div class='container'><h2>🤖 MiniClaw Setup</h2>";
        html += "<form action='/save' method='POST'>";
        html += "<label>WiFi SSID:</label><input type='text' name='ssid' required>";
        html += "<label>WiFi Password:</label><input type='password' name='pass' required>";
        html += "<label>Telegram Bot Token:</label><input type='text' name='token' required>";
        html += "<label>Kimi API Key:</label><input type='text' name='kimi' required>";
        html += "<button type='submit'>Save & Restart</button></form></div></body></html>";
        request->send(200, "text/html", html);
    });
    
    server->on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
        String ssid, pass, token, kimi;
        if (request->hasParam("ssid", true)) ssid = request->getParam("ssid", true)->value();
        if (request->hasParam("pass", true)) pass = request->getParam("pass", true)->value();
        if (request->hasParam("token", true)) token = request->getParam("token", true)->value();
        if (request->hasParam("kimi", true)) kimi = request->getParam("kimi", true)->value();
        
        // Save to preferences (simplified - in real app save to NVS)
        request->send(200, "text/html", "<h1>Saved! Restarting...</h1>");
        delay(2000);
        ESP.restart();
    });
    
    server->begin();
    Serial.println("Web server started on port 80");
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::reconnect() {
    if (!isConnected()) {
        WiFi.reconnect();
    }
}

void WiFiManager::handleAPMode() {
    if (apMode && server) {
        // AP mode is handled by AsyncWebServer
    }
}

String WiFiManager::getIP() {
    if (apMode) return WiFi.softAPIP().toString();
    return WiFi.localIP().toString();
}

int WiFiManager::getRSSI() {
    return WiFi.RSSI();
}
