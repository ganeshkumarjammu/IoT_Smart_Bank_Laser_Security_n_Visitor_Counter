#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>


const char* ssid = "123456";
const char* password = "11223344";
const char* BOTtoken = "7081364617:AAF6nJs0ihhpl1c8wGUh21xiuk8wlFJU2n8" ;//"6960430957:AAGqGKlpPdf-M9CztjgEK26rqFBUM1lo25E";
#define CHAT_ID "-1002129574130" 

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin1 = 25; // Change to your LED pin
const int ledPin2 = 26; // Change to your LED pin
const int fan1 = 14;    // Change to your fan pin
const int laser = 27;   // Change to your laser pin

bool pinStatus[4] = {false, false, false, true}; // Initialize pin statuses

float humi , tempC ;
int visitors = 0;
bool warning = true;

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    Serial.print("Chat ID:");
    Serial.println(chat_id);
    int atIndex = text.indexOf('@');
    if (atIndex != -1) {
      text = text.substring(0, atIndex);
    }
    if (text == "/start") {
      String welcome = "Welcome!\n";
      welcome += "Use the following commands:\n";
      welcome += "/light1_on to turn Light 1 ON\n";
      welcome += "/light1_off to turn Light 1 OFF\n";
      welcome += "/light2_on to turn Light 2 ON\n";
      welcome += "/light2_off to turn Light 2 OFF\n";
      welcome += "/fan1_on to turn Fan 1 ON\n";
      welcome += "/fan1_off to turn Fan 1 OFF\n";
      welcome += "/laser_on to turn Laser ON\n";
      welcome += "/laser_off to turn Laser OFF\n";
      welcome += "/warn to change warning mode\n";
      welcome += "/all_on to turn all lights and devices ON\n";
      welcome += "/all_off to turn all lights and devices OFF\n";
      welcome += "/status to request current status\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/light1_on") {
      pinStatus[0] = true;
      digitalWrite(ledPin1, HIGH);
      bot.sendMessage(chat_id, "Light 1 state set to ON", "");
    } else if (text == "/light1_off") {
      pinStatus[0] = false;
      digitalWrite(ledPin1, LOW);
      bot.sendMessage(chat_id, "Light 1 state set to OFF", "");
    } else if (text == "/light2_on") {
      pinStatus[1] = true;
      digitalWrite(ledPin2, HIGH);
      bot.sendMessage(chat_id, "Light 2 state set to ON", "");
    } else if (text == "/light2_off") {
      pinStatus[1] = false;
      digitalWrite(ledPin2, LOW);
      bot.sendMessage(chat_id, "Light 2 state set to OFF", "");
    } else if (text == "/fan1_on") {
      pinStatus[2] = true;
      // Turn on fan or perform fan-related operations here
      bot.sendMessage(chat_id, "Fan 1 state set to ON", "");
    } else if (text == "/fan1_off") {
      pinStatus[2] = false;
      // Turn off fan or perform fan-related operations here
      bot.sendMessage(chat_id, "Fan 1 state set to OFF", "");
    } else if (text == "/laser_on") {
      pinStatus[3] = true;
      digitalWrite(laser, HIGH);
      bot.sendMessage(chat_id, "Laser state set to ON", "");
    } else if (text == "/laser_off") {
      pinStatus[3] = false;
      digitalWrite(laser, LOW);
      bot.sendMessage(chat_id, "Laser state set to OFF", "");
    } else if (text == "/warn") {
      warning = !warning;
      bot.sendMessage(chat_id, "Warning Mode Changed", "");
    } else if (text == "/all_on") {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      // Turn on fan and laser or perform related operations here
      digitalWrite(fan1, HIGH);
      digitalWrite(laser, HIGH);
      for (int i = 0; i < 4; i++) {
        pinStatus[i] = true;
      }
      bot.sendMessage(chat_id, "All lights and devices set to ON", "");
    } else if (text == "/all_off") {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      // Turn off fan and laser or perform related operations here
      digitalWrite(fan1, LOW);
      digitalWrite(laser, LOW);
      for (int i = 0; i < 4; i++) {
        pinStatus[i] = false;
      }
      bot.sendMessage(chat_id, "All lights and devices set to OFF", "");
    } else if (text == "/status") {
      String statusMessage = "Status :\n";
      statusMessage += "Light 1: " + String(pinStatus[0] ? "On" : "Off") + "\n";
      statusMessage += "Light 2: " + String(pinStatus[1] ? "On" : "Off") + "\n";
      statusMessage += "Fan 1: " + String(pinStatus[2] ? "On" : "Off") + "\n";
      statusMessage += "Laser: " + String(pinStatus[3] ? "On" : "Off") + "\n";
      //statusMessage += "Visitors: " + String(visitors) + "\n";
      //statusMessage += "Temp: " + String(tempC) + "°C\n";
     // statusMessage += "Humidity: " + String(humi) + "%\n";
      bot.sendMessage(chat_id, statusMessage, "");
    }
  }
}

void setup() {
  Serial.begin(115200);
#ifdef ESP8266
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
#endif

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(laser, OUTPUT);

  // Initialize pin states
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(fan1, LOW);
  digitalWrite(laser, LOW);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef ESP32
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
#endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
