#include "telegram_bot.h"
#include "config.h"
#include "secrets.h"
#include "kimi_client.h"
#include <Arduino.h>

extern KimiClient kimi;

void TelegramBot::begin() {
    botToken = TELEGRAM_BOT_TOKEN;
    securedClient.setInsecure();  // For Telegram API
    
    if (botToken.length() > 10) {
        bot = new UniversalTelegramBot(botToken, securedClient);
        Serial.println("Telegram Bot initialized");
    } else {
        Serial.println("❌ Bot token not configured");
    }
}

bool TelegramBot::isReady() {
    return bot != nullptr;
}

void TelegramBot::checkMessages() {
    if (!isReady()) return;
    
    int currentTime = millis() / 1000;
    if (currentTime - lastCheck < 1) return;  // Check every 1 second
    lastCheck = currentTime;
    
    int numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    
    while (numNewMessages) {
        Serial.println("Got Telegram message");
        
        for (int i = 0; i < numNewMessages; i++) {
            String chatId = String(bot->messages[i].chat_id);
            String text = bot->messages[i].text;
            String fromName = bot->messages[i].from_name;
            
            Serial.println("From: " + fromName + " | Message: " + text);
            
            // Handle commands
            if (text.startsWith("/")) {
                handleCommand(chatId, text);
            } else {
                // Regular message - ask Kimi
                sendMessage(chatId, "🤔 Thinking...");
                String response = kimi.chat(text);
                sendMessage(chatId, response);
            }
        }
        
        numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    }
}

void TelegramBot::handleCommand(const String& chatId, const String& cmd) {
    if (cmd == "/start") {
        String welcome = "🤖 *MiniClaw ESP32* is ready!\n\n";
        welcome += "Available commands:\n";
        welcome += "`/status` - Device info\n";
        welcome += "`/gpio [pin] [0/1]` - Control GPIO\n";
        welcome += "\nOr just chat with me!";
        bot->sendMessage(chatId, welcome, "Markdown");
    }
    else if (cmd == "/status") {
        bot->sendMessage(chatId, getStatusMessage());
    }
    else if (cmd.startsWith("/gpio")) {
        handleGPIOCommand(chatId, cmd);
    }
    else {
        bot->sendMessage(chatId, "❓ Unknown command. Type /start for help.");
    }
}

void TelegramBot::handleGPIOCommand(const String& chatId, const String& cmd) {
    // Parse: /gpio 2 1
    int firstSpace = cmd.indexOf(' ');
    int secondSpace = cmd.indexOf(' ', firstSpace + 1);
    
    if (firstSpace == -1 || secondSpace == -1) {
        bot->sendMessage(chatId, "⚠️ Usage: `/gpio [pin] [0/1]`\nExample: `/gpio 2 1`");
        return;
    }
    
    String pinStr = cmd.substring(firstSpace + 1, secondSpace);
    String valStr = cmd.substring(secondSpace + 1);
    
    int pin = pinStr.toInt();
    int val = valStr.toInt();
    
    if (pin < 0 || pin > 48) {
        bot->sendMessage(chatId, "❌ Invalid pin number (0-48)");
        return;
    }
    
    pinMode(pin, OUTPUT);
    digitalWrite(pin, val ? HIGH : LOW);
    
    String msg = "✅ GPIO " + String(pin) + " set to " + String(val ? "HIGH 🔴" : "LOW ⚫");
    bot->sendMessage(chatId, msg);
}

String TelegramBot::getStatusMessage() {
    String msg = "📊 *MiniClaw Status*\n\n";
    msg += "🔋 Uptime: " + String(millis() / 1000) + "s\n";
    msg += "💾 Free Heap: " + String(ESP.getFreeHeap() / 1024) + " KB\n";
    msg += "📶 WiFi RSSI: " + String(WiFi.RSSI()) + " dBm\n";
    msg += "🌐 IP: " + WiFi.localIP().toString() + "\n";
    msg += "🤖 Version: " + String(DEVICE_VERSION);
    return msg;
}

void TelegramBot::sendMessage(const String& chatId, const String& text) {
    if (isReady()) {
        bot->sendMessage(chatId, text);
    }
}
