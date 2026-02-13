#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

class TelegramBot {
public:
    void begin();
    void checkMessages();
    void sendMessage(const String& chatId, const String& text);
    bool isReady();
    
private:
    WiFiClientSecure securedClient;
    UniversalTelegramBot* bot = nullptr;
    String botToken;
    int lastCheck = 0;
    
    void handleCommand(const String& chatId, const String& cmd);
    void handleGPIOCommand(const String& chatId, const String& cmd);
    String getStatusMessage();
};

#endif // TELEGRAM_BOT_H
