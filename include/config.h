#ifndef CONFIG_H
#define CONFIG_H

// Device Info
#define DEVICE_NAME "MiniClaw"
#define DEVICE_VERSION "1.0.0"

// WiFi Configuration (fallback if not in secrets.h)
#ifndef WIFI_SSID
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASS"
#endif

#define WIFI_TIMEOUT_MS 20000
#define WIFI_RECONNECT_INTERVAL_MS 5000

// Telegram
#ifndef TELEGRAM_BOT_TOKEN
#define TELEGRAM_BOT_TOKEN "YOUR_BOT_TOKEN"
#endif
#define TELEGRAM_CHECK_INTERVAL_MS 1000  // Check messages every 1 second

// Kimi LLM (z.ai)
#ifndef KIMI_API_KEY
#define KIMI_API_KEY "YOUR_KIMI_API_KEY"
#endif
#define KIMI_API_ENDPOINT "https://api.z.ai/v1/chat/completions"
#define KIMI_MODEL "glm-4"
#define KIMI_MAX_TOKENS 500
#define KIMI_TIMEOUT_MS 30000

// GPIO
#define LED_BUILTIN_PIN 2
#define GPIO_POLL_INTERVAL_MS 100

// System
#define STATUS_REPORT_INTERVAL_MS 60000  // Report every 1 minute
#define WATCHDOG_TIMEOUT_MS 30000

#endif // CONFIG_H
