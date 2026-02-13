# MiniClaw ESP32 🤖

ESP32-S3 based AI Assistant with Telegram integration and Kimi LLM support.

## ✨ Features

- **🤖 Telegram Bot** - Chat with your ESP32 via Telegram
- **🧠 Kimi LLM** - Powered by z.ai (GLM-4.5)
- **📶 WiFi Manager** - Auto-connect with AP fallback
- **🔌 GPIO Control** - Control pins via chat commands
- **💾 PSRAM Support** - Utilize 8MB PSRAM on ESP32-S3

## 🛠️ Hardware

- **Board:** ESP32-S3 N16R8 (16MB Flash + 8MB PSRAM)
- **Connectivity:** WiFi 2.4GHz
- **USB:** USB-C (Serial + OTG)

## 🚀 Quick Start

### 1. Prerequisites

Install **PlatformIO** (VS Code Extension or CLI):
```bash
pip install platformio
```

Install USB Driver:
- [CP210x Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) or
- [CH340 Driver](https://sparks.gogo.co.nz/ch340.html)

### 2. Clone & Configure

```bash
git clone https://github.com/YOUR_USERNAME/miniclaw-esp32.git
cd miniclaw-esp32
```

Copy config template:
```bash
cp include/secrets.h.example include/secrets.h
```

Edit `include/secrets.h`:
```cpp
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASS"
#define TELEGRAM_BOT_TOKEN "YOUR_BOT_TOKEN"  // From @BotFather
#define KIMI_API_KEY "YOUR_ZAI_API_KEY"       // From z.ai
```

### 3. Build & Flash

```bash
pio run --target upload
pio device monitor
```

## 🤖 Telegram Commands

| Command | Description |
|---------|-------------|
| `/start` | Start conversation |
| `/status` | Show device status (uptime, heap, WiFi) |
| `/gpio [pin] [0/1]` | Control GPIO pin (e.g., `/gpio 2 1`) |
| Any text | Ask Kimi AI |

## 🧠 How It Works

```
User (Telegram) → ESP32 → Kimi API (z.ai) → Response → Telegram
```

1. User sends message to Telegram Bot
2. ESP32 receives message via Telegram API
3. If command: execute directly (GPIO, status)
4. If text: forward to Kimi LLM API
5. Send response back to Telegram

## 📁 Project Structure

```
miniclaw-esp32/
├── include/
│   ├── config.h          # General configuration
│   ├── secrets.h         # WiFi & API keys (not in git)
│   └── secrets.h.example # Template
├── src/
│   ├── main.cpp          # Entry point
│   ├── telegram_bot.cpp  # Telegram handling
│   ├── telegram_bot.h
│   ├── kimi_client.cpp   # Kimi LLM API
│   ├── kimi_client.h
│   ├── wifi_manager.cpp  # WiFi connection
│   └── wifi_manager.h
├── platformio.ini        # PlatformIO config
└── README.md
```

## ⚠️ Important

- **Never commit `secrets.h`** - It contains your API keys!
- ESP32 has limited RAM (~512KB), long responses may be truncated
- For best results, keep questions concise

## 📝 License

MIT License - Feel free to use and modify!

## 🙏 Credits

- [UniversalTelegramBot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot) by witnessmenow
- [ArduinoJson](https://arduinojson.org/) by bblanchon
- Kimi LLM by [z.ai](https://z.ai)
