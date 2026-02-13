#include "kimi_client.h"
#include "config.h"
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

void KimiClient::begin() {
    apiKey = KIMI_API_KEY;
    Serial.println("Kimi Client initialized");
}

bool KimiClient::isReady() {
    return apiKey.length() > 10;
}

String KimiClient::chat(const String& message) {
    if (!isReady()) {
        return "❌ API Key not configured";
    }
    
    WiFiClientSecure client;
    client.setInsecure();  // For simplicity - in production use proper cert
    
    HTTPClient http;
    http.begin(client, KIMI_API_ENDPOINT);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + apiKey);
    http.setTimeout(KIMI_TIMEOUT_MS);
    
    // Build JSON request
    StaticJsonDocument<512> doc;
    doc["model"] = KIMI_MODEL;
    doc["max_tokens"] = KIMI_MAX_TOKENS;
    doc["stream"] = false;  // Non-streaming for ESP32 simplicity
    
    JsonArray messages = doc.createNestedArray("messages");
    JsonObject msg = messages.createNestedObject();
    msg["role"] = "user";
    msg["content"] = message;
    
    String requestBody;
    serializeJson(doc, requestBody);
    
    Serial.println("Sending to Kimi: " + message);
    int httpCode = http.POST(requestBody);
    
    String response = "";
    if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Raw response: " + payload.substring(0, 200));
        
        DynamicJsonDocument respDoc(2048);
        DeserializationError error = deserializeJson(respDoc, payload);
        
        if (!error) {
            const char* content = respDoc["choices"][0]["message"]["content"];
            if (content) {
                response = String(content);
                response.trim();
            } else {
                response = "⚠️ Empty response from AI";
            }
        } else {
            response = "⚠️ Failed to parse response";
            Serial.println("JSON parse error: " + String(error.c_str()));
        }
    } else if (httpCode == 401) {
        response = "❌ Invalid API Key";
    } else if (httpCode == 429) {
        response = "⏳ Rate limited. Please wait.";
    } else {
        response = "❌ Error: HTTP " + String(httpCode);
        Serial.println("HTTP Error: " + String(httpCode));
    }
    
    http.end();
    return response.length() > 0 ? response : "🤔 No response";
}
