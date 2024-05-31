# IoT Security System for Banks

## Introduction
This project implements an IoT-based security system specifically designed for banks. It utilizes an ESP32 microcontroller along with various sensors to monitor environmental conditions, detect intrusions, and control devices remotely via a Telegram bot. The system features temperature and humidity monitoring, visitor counting, light detection, and fan control, and can send alerts and respond to commands through the Telegram bot.

## Components
- **ESP32 Microcontroller**
- **DHT11 Sensor** (Temperature and Humidity)
- **IR Sensors** (for visitor counting)
- **LDR Sensor** (Light Detection)
- **Liquid Crystal Display (LCD)** 16x2 with I2C interface
- **L298N Motor Driver** (for fan control)
- **Buzzer**
- **LEDs**
- **Telegram Bot** for remote control

## Libraries Used
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- [LiquidCrystal I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
- [Universal Telegram Bot Library](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

## Circuit Diagram
![Circuit Diagram](path_to_circuit_diagram_image)

## Setup

### Hardware Setup
1. **Connect the DHT11 Sensor:**
   - VCC to 5V
   - GND to GND
   - Data pin to digital pin 5

2. **Connect the IR Sensors:**
   - IR Sensor 1: Output pin to digital pin 2
   - IR Sensor 2: Output pin to digital pin 3

3. **Connect the LDR Sensor:**
   - LDR pin to analog pin A0 (or digital pin 7 if using a digital interface)

4. **Connect the Buzzer:**
   - Positive pin to digital pin 4
   - Negative pin to GND

5. **Connect the LCD:**
   - SDA to SDA
   - SCL to SCL
   - VCC to 5V
   - GND to GND

6. **Connect the Fan and Motor Driver:**
   - ENA to digital pin 10 (PWM pin)
   - IN1 and IN2 to digital pins 9 and 8 respectively
   - VCC and GND to power supply

### Software Setup
1. **Install the Arduino IDE** if you haven't already.

2. **Install the required libraries**:
   - Go to Sketch > Include Library > Manage Libraries
   - Search for and install the following:
     - DHT Sensor Library
     - LiquidCrystal I2C
     - Universal Telegram Bot
     - ArduinoJson

3. **Set up the Telegram Bot**:
   - Create a new bot using BotFather on Telegram and obtain the bot token.
   - Find your chat ID using the @myidbot on Telegram.

4. **Configure WiFi credentials and Telegram settings**:
   - Replace `ssid` and `password` with your WiFi network credentials.
   - Replace `BOTtoken` with your Telegram bot token.
   - Replace `CHAT_ID` with your Telegram chat ID.

## Usage
1. **Upload the main Arduino code** to your ESP32 using the Arduino IDE.
2. **Upload the Telegram bot code** to your ESP32 using the Arduino IDE.
3. **Open the Serial Monitor** to observe the sensor readings and control states.
4. **Use the Telegram bot** to control the devices remotely by sending commands like `/light1_on`, `/fan1_off`, etc.
5. **Monitor the system** through the LCD display and the Serial Monitor.

## License
This project is licensed under the MIT License - see the LICENSE file for details.


