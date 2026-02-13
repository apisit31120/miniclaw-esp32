/*
 * MiniClaw ESP32 - Main
 * Telegram Bot + Kimi LLM (z.ai) + GPIO Control
 * 
 * Hardware: ESP32-S3 N16R8 (16MB Flash + 8MB PSRAM)
 */

#include <Arduino.h>
#include "config.h"
#include "secrets.h"
#include "wifi_manager.h"
#include "kimi_client.h"
#include "telegram_bot.h"

// Global instances
WiFiManager wifiManager;
KimiClient kimi;
TelegramBot telegramBot;

unsigned long lastStatusReport = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n");
    Serial.println("╔════════════════════════════════════╗");
    Serial.println("║      🤖 MiniClaw ESP32-S3          ║");
    Serial.println("║   Telegram + Kimi AI Assistant     ║");
    Serial.println("╚════════════════════════════════════╝");
    Serial.println("\n");
    
    // Initialize LED
    pinMode(LED_BUILTIN_PIN, OUTPUT);
    digitalWrite(LED_BUILTIN_PIN, HIGH);
    
    // Initialize Kimi Client
    Serial.println("🧠 Initializing Kimi Client...");
    kimi.begin();
    
    // Connect WiFi
    Serial.println("📶 Connecting to WiFi...");
    wifiManager.begin();
    
    if (wifiManager.isConnected()) {
        Serial.println("✅ WiFi Connected!");
        Serial.print("   IP: ");
        Serial.println(wifiManager.getIP());
        
        // Initialize Telegram Bot
        Serial.println("🤖 Starting Telegram Bot...");
        telegramBot.begin();
        
        if (telegramBot.isReady()) {
            Serial.println("✅ Bot is ready!");
            telegramBot.sendMessage("", "🤖 MiniClaw is online!\nIP: " + wifiManager.getIP());
        } else {
            Serial.println("⚠️ Bot not configured - check secrets.h");
        }
        
        // Test Kimi
        Serial.println("🧠 Testing Kimi API...");
        if (kimi.isReady()) {
            Serial.println("✅ Kimi API Ready");
        } else {
            Serial.println("⚠️ Kimi not configured - check secrets.h");
        }
        
        // Blink LED to show ready
        for (int i = 0; i < 3; i++) {
            digitalWrite(LED_BUILTIN_PIN, LOW);
            delay(100);
            digitalWrite(LED_BUILTIN_PIN, HIGH);
            delay(100);
        }
    } else {
        Serial.println("⚠️ AP Mode active - connect to 'MiniClaw-Setup' to configure");
    }
    
    Serial.println("\n✨ Setup complete!\n");
}

void loop() {
    // Handle WiFi
    if (!wifiManager.isConnected()) {
        wifiManager.reconnect();
        wifiManager.handleAPMode();
        delay(1000);
        return;
    }
    
    // Check Telegram messages
    telegramBot.checkMessages();
    
    // Status report every minute
    unsigned long now = millis();
    if (now - lastStatusReport >= STATUS_REPORT_INTERVAL_MS) {
        Serial.println("📊 Status - Heap: " + String(ESP.getFreeHeap() / 1024) + "KB, RSSI: " + String(WiFi.RSSI()) + "dBm");
        lastStatusReport = now;
    }
    
    // Small delay to prevent watchdog
    delay(100);
}
