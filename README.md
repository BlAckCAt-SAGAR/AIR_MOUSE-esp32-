
## Wiring (summary)
- **MPU6050**
  - VCC -> 3.3V on ESP32
  - GND -> GND
  - SDA -> GPIO21 (default Wire SDA)
  - SCL -> GPIO22 (default Wire SCL)

- **Buttons** (each button other pin -> GND)
  - LEFT_CLICK_PIN  -> GPIO18
  - RIGHT_CLICK_PIN -> GPIO19
  - SCROLL_UP_PIN   -> GPIO14
  - SCROLL_DOWN_PIN -> GPIO27

- **Power**
  - Li-ion battery (3.7V) → USB-C charger module B+/B-
  - Charger OUT+ → ESP32 VIN (or BAT input if your devboard has it)
  - Charger OUT- → GND
  - MPU6050 VCC must be 3.3V (do NOT connect to 5V unless breakout has regulator)

## Libraries required
Install these from Arduino Library Manager:
- `BleMouse` (or ESP32 BLE Mouse library)
- `Adafruit_MPU6050`
- `Adafruit_Sensor`
- `Wire` (built-in)

Also ensure ESP32 board support is installed in Arduino IDE (ESP32 by Espressif Systems).

## Setup / Build / Upload (Arduino)
1. Install Arduino IDE or VSCode + PlatformIO.
2. In Arduino IDE: File → Preferences → Additional Boards Manager URLs add ESP32 URL (https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)
3. Tools → Board → select your ESP32 dev board (e.g., "ESP32 Dev Module").
4. Open `src/air_mouse.ino`.
5. Install the listed libraries via Sketch → Include Library → Manage Libraries.
6. Connect ESP32 via USB, select correct Port in Tools, then Upload.

## Notes about battery
- Use a TP4056 + protection board or a charger module with protection.
- Confirm the devboard accepts the charger OUT voltage at VIN/BAT.
- 300 mAh is small. Add a power switch for safety.

## Files included
- `src/air_mouse.ino` — main source
- `hardware/circuit_diagram.jpg` — wiring image (local)
- `docs/diagrams.pdf` and `docs/poster.png` — diagrams & poster PDFs/images

## Circuit diagram and poster (local)
- Circuit diagram: `/mnt/data/493128881-85f1796a-e780-4030-9029-ef5a011791b4.jpg`
- Poster image (generated): `/mnt/data/A_diagram_in_the_image_provides_a_visual_represent.png`
- Diagrams PDF: `/mnt/data/Air_Mouse_Diagrams_Proper.pdf`

> Replace the above local paths with the files placed into `hardware/` and `docs/` respectively before committing.
////''\\\\
