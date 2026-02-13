#ifndef KIMI_CLIENT_H
#define KIMI_CLIENT_H

#include <Arduino.h>
#include <ArduinoJson.h>

class KimiClient {
public:
    void begin();
    String chat(const String& message);
    bool isReady();
    
private:
    String apiKey;
    bool parseStreamResponse(WiFiClient& client, String& output);
};

#endif // KIMI_CLIENT_H
