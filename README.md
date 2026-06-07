# 🏠 Home Assistant Arduino Dashboard

A touch-screen dashboard for **Home Assistant** built on the **Arduino Giga R1 + Giga Display Shield** (800×480px). It connects to your home network via WiFi and receives real-time data through **MQTT**, displaying and controlling a wide range of smart home features.

---

## 📸 Preview

<img width="799" height="599" alt="image" src="https://github.com/user-attachments/assets/17791413-fc8b-4f75-a712-d503ec2e6a3b" />

---

## ✨ Features

| Module | What you can see / do |
|---|---|
| ⚡ **Energy** | Real-time import, production & consumption with scrolling graph (short & long view) |
| 🚗 **Car** | Battery %, charging power, target charge, cruising range + EVCC detail view |
| 💡 **Lights** | Toggle sfeerlichtjes, grote bol, glazen bol, maanlamp, raamversiering — tap to switch, hold for brightness |
| 🎵 **Music** | Control keuken & speelkamer speakers; switch between Radio 1, Radio 2, Vuurland, VRT NWS, Spotify |
| 🌬️ **Ventilation** | CO₂, VOC, humidity, air quality per room; boost control (1h / 3h / stop) |
| 🔥 **Heat pump** | Compressor state, hot water temperature, next DHW cycle time |
| 🌿 **Mower** | Battery %, start / pause / return home |
| 🗑️ **Waste** | Today's waste collection type (Rest, GFT, PMD, Papier) |
| ⚙️ **Settings** | Toggle dark/light mode, last reset time, manual reset button |

The dashboard has two home screens (swipe right) so you can arrange which modules appear on which page.

---

## 🛠️ Hardware

- **Arduino Giga R1 WiFi**
- **Arduino Giga Display Shield** (800×480 touch display)
- MicroSD card (for image assets used by `Dashboard.cpp`)

---

## 📚 Required Libraries

Install these via the Arduino IDE Library Manager:

- `WiFi` (built-in for Giga R1)
- `ArduinoMqttClient`
- `Arduino_GigaDisplayTouch`
- `Arduino_GigaDisplay_GFX` *(or whichever GFX library Dashboard.cpp uses)*

---

## ⚙️ Setup

### 1. Download and open the code

- Go to the [Releases page](../../releases/latest) and download the source code zip
- Unzip it — you'll get a folder called `ha-arduino-dashboard-1.0`
- **Rename that folder to `ha-arduino-dashboard`** (Arduino IDE requires the folder and `.ino` file to have the same name)
- Open `ha-arduino-dashboard.ino` in Arduino IDE

### 2. Download and prepare the SD card

- On the same [Releases page](../../releases/latest), download `sd-assets.zip`
- Unzip it and copy **all files** to the **root** of your MicroSD card
- Insert the SD card into the Giga Display Shield

### 3. Configure your credentials

Rename `arduino_secrets_example.h` to `arduino_secrets.h` and fill in your details:

```cpp
#define SECRET_SSID        "your-wifi-name"
#define SECRET_PASS        "your-wifi-password"
#define SECRET_MQTT_HOST   "192.168.x.x"       // your Home Assistant IP
#define SECRET_MQTT_PORT   1883
#define SECRET_MQTT_USER   "your-mqtt-username"
#define SECRET_MQTT_PASS   "your-mqtt-password"
```

> ⚠️ **Never commit your real `arduino_secrets.h` to Git.** It is listed in `.gitignore` for this reason.

### 4. Set up MQTT in Home Assistant

The dashboard subscribes to these MQTT topics — make sure your HA automations or integrations publish to them:

| Topic prefix | Data |
|---|---|
| `home/time` | Current time as `HH:MM:SS` |
| `home/temp` | Outdoor temperature (integer °C) |
| `energy/currentImport`, `currentProduction`, `currentConsumption` | Watts |
| `energy/newValue` | `Short` or `Long` to trigger graph update |
| `laadpaal/battery`, `chargingPower`, `targetCharge`, `cruisingRange` | Car/charger data |
| `lights/sfeerlichtjes`, `groteBol`, `glazenBol`, `maanlamp`, `raamversiering`, `berging` | `on` / `off` / brightness (0-255) |
| `music/keuken/...` & `music/speelkamer/...` | `title`, `channel`, `state`, `volume` |
| `ventilation/keuken/...` & `ventilation/kelder/...` | `CO2`, `VOC`, `humidity`, `AQI`, `state`, `remaining` |
| `heatPump/compressor`, `hotWaterTemp`, `XDHW`, `nextTime` | Heat pump data |
| `mower/battery`, `mower/state` | `mowing` / `paused` / `docked` |
| `afval/...` | Waste type string containing `GFT`, `Restafval`, `PMD`, `Papier` |

The dashboard also **publishes** to these topics when you interact with it (e.g. `lights/groteBol → "Switch"`, `music/keuken → "toRadio1"`, `mower/set → "Start"`).

### 5. Flash the sketch

Open `Home_assistant.ino` in the Arduino IDE, select **Arduino Giga R1 WiFi** as the board, and upload.

---

## 🗂️ File Structure

```
├── Home_assistant.ino        # Main sketch: WiFi, MQTT, touch handling, page routing
├── Dashboard.cpp             # All drawing logic (screens, icons, graphs, images)
├── Dashboard.h               # Dashboard class declaration
├── Colors.h                  # Colour definitions (RGB565)
├── arduino_secrets_example.h # Credential template — rename to arduino_secrets.h
└── .gitignore                # Excludes arduino_secrets.h from version control
```

---

## 🌙 Automatic Behaviour

- **Night mode**: between `20:00` and `07:00` the screen turns off after 60 seconds of inactivity.
- **Auto return**: outside night hours, the dashboard returns to the home screen after 60 seconds of inactivity.
- **Watchdog reset**: if no energy MQTT message arrives for 15 seconds, or WiFi drops, the board resets automatically.

---

## 🤝 Contributing

Feel free to open issues or pull requests. This project is highly personal/hardware-specific, but the MQTT structure and drawing approach might be useful to others building similar dashboards.

---

## 📄 License

MIT — do whatever you want with it, just don't blame me if your heat pump does something weird. 😄
