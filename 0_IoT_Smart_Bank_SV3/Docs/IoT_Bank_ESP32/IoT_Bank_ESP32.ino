#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

const char* ssid = "123456";
const char* password = "11223344";

// Initialize Telegram BOT
#define BOTtoken "7081364617:AAETXp1_7aQImI57AaxBkIxZE0U7j6qtPNg"  //"6960430957:AAGqGKlpPdf-M9CztjgEK26rqFBUM1lo25E";

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "-1002129574130"
#define chat_id1 "-1002129574130"//"-1002085707801"
#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin1 = 12;
const int ledPin2 = 13;
const int fan1 = 4;
const int laser = 14;
const int ldr = 18;

//// Counter variable for total visitors
//float humi , tempC ;
//
//// Setting PWM properties
//const int freq = 30000;
//const int pwmChannel = 0;
//const int resolution = 8;
//int dutyCycle = 200;

bool pinStatus[4] = {false, false, false, true};
bool warning = true ;

// //========> DHT11 AND LCD I2C
// #include <DHT.h>
// #include <LiquidCrystal_I2C.h>
// #define DHT11_PIN  23 // ESP32 pin GPIO23 connected to DHT11 sensor

// LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27 (from DIYables LCD), 16 column and 2 rows
// DHT dht11(DHT11_PIN, DHT11);


// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);   
    if (chat_id != CHAT_ID){
//      bot.sendMessage(chat_id, "Unauthorized user", "");
//      continue;
     chat_id = "-1002129574130";
    }   
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;
//    int atIndex = text.indexOf('@');
//    if (atIndex != -1) {
//      text = text.substring(0, atIndex);
//    }
    Serial.print("text:");
    Serial.println(text);
    if (text == "/start") {
      String welcome = "Welcome to Smart Home, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/light1_on to turn Light1 ON \n";
      welcome += "/light1_off to turn Light1 OFF \n";
      welcome += "/light2_on to turn Light2 ON \n";
      welcome += "/light2_off to turn Light2 OFF \n";
      welcome += "/fan1_on to turn Fan1 ON \n";
      welcome += "/fan1_off to turn Fan1 OFF \n";
      welcome += "/laser_on to turn Laser1 ON \n";
      welcome += "/laser_off to turn Laser1 OFF \n";
      welcome += "/warn to change warning mode \n";
      welcome += "/all_on to turn all \n";
      welcome += "/all_off to turn all  \n";
      welcome += "/status to request current pins states \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/light1_on") {
      bot.sendMessage(chat_id, "Light 1 state set to ON", "");
      pinStatus[0] = true;
      digitalWrite(ledPin1, HIGH);
    }
    else if (text == "/light1_off") {
       bot.sendMessage(chat_id, "Light1 state set to OFF", "");
      pinStatus[0] = false;
      digitalWrite(ledPin1, LOW);
    }
    else if (text == "/light2_on") {
      bot.sendMessage(chat_id, "Light2 state set to ON", "");
      pinStatus[1] = true;
      digitalWrite(ledPin2, HIGH);
    } 
    else if (text == "/light2_off") {
      bot.sendMessage(chat_id, "Light2 state set to OFF", "");
      pinStatus[1] = false;
      digitalWrite(ledPin2, LOW);
    }
    else if (text == "/fan1_on") {
      bot.sendMessage(chat_id, "Fan state set to ON", "");
      pinStatus[2] = true;
      digitalWrite(fan1,HIGH);
    }
    else if (text == "/fan1_off") {
      bot.sendMessage(chat_id, "Fan state set to OFF", "");
      pinStatus[2] = false;
      digitalWrite(fan1,LOW);
    }
    else if (text == "/laser_on") {
      bot.sendMessage(chat_id, "Laser1 state set to ON", "");
      pinStatus[3] = true;
      digitalWrite(laser, HIGH);
      
    }
    else if (text == "/laser_off") {
      bot.sendMessage(chat_id, "Laser1 state set to OFF", "");
      pinStatus[3] = false;
      digitalWrite(laser, LOW);
    } 
    else if(text == "/warn"){
      bot.sendMessage(chat_id, "Warning Mode Changed", "");
      warning = ! warning;
    }
    else if (text == "/all_on") {
      bot.sendMessage(chat_id, " All  set to ON", "");
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(fan1, HIGH);
      digitalWrite(laser, HIGH);
      for(int i = 0; i < 4 ; i++){
        pinStatus[i]=true;
      }
    }
    else if (text == "/all_off") {
       bot.sendMessage(chat_id, "All set to OFF", "");
      digitalWrite(ledPin1, LOW);      
      digitalWrite(ledPin2, LOW);      
      digitalWrite(fan1, LOW);      
      digitalWrite(laser, LOW);
      for(int i = 0; i < 4 ; i++){
        pinStatus[i]=false;
      }
    }
    if (text == "/status") {
    // Serial.println("Status success");
    //runFan();
    String statusMessage = "Status:\n";
    statusMessage += "Light 1: " + String(pinStatus[0] ? "On" : "Off") + "\n" ;
    statusMessage += "Light 2: " + String(pinStatus[1] ? "On" : "Off")+"\n" ;
    statusMessage += "Fan 1  : " + String(pinStatus[2] ? "On" : "Off") + "\n";
    statusMessage += "Laser  : " + String(pinStatus[3] ? "On" : "Off") + "\n";
    bot.sendMessage(chat_id, statusMessage,"");
    }
  }
}


// void runFan() {
//   // humi  = dht11.readHumidity();    // read humidity
//   // tempC = dht11.readTemperature(); // read temperature
//   // //lcd.clear();
//   // // check whether the reading is successful or not
//   // if ((isnan(tempC) || isnan(humi)) && pinStatus[2]) {
//   //     Serial.println("Connect DHT11");
//   //     ledcWrite(pwmChannel, 150);  
//   // } else if (tempC < 40 && pinStatus[2]) {
//   //   Serial.println("Temp:");
//   //   Serial.print(tempC);
//   //   // lcd.setCursor(0, 0);  // display position
//   //   // lcd.print("Temp: ");
//   //   // lcd.print(tempC);     // display the temperature
//   //   // lcd.print("°C");
//   //   // lcd.setCursor(0, 1);  // display position
//   //   // lcd.print("Humi: ");
//   //   // lcd.print(humi);      // display the humidity
//   //   // lcd.print("%");
//   //   ledcWrite(pwmChannel, 150);   

//   // }
//   // else if (pinStatus[2]){
//   // //int fanSpeed = map(temperature, 0, 50, 0, 255); // Adjust temperature range as needed
//   // ledcWrite(pwmChannel, 150);   
//   // // Control fan speed using PWM
//   // }
//   // else if(!pinStatus[2]){
//   //   ledcWrite(pwmChannel, 0);  
//   // }
//   // // wait a 2 seconds between readings
//   // delay(500);
// }


void setup() {
  Serial.begin(115200);
//Telegram 
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(laser, OUTPUT); 
  pinMode(ldr,INPUT);

//    // configure LED PWM functionalitites
//  ledcSetup(pwmChannel, freq, resolution);
//  
//  // attach the channel to the GPIO to be controlled
//  ledcAttachPin(fan1, pwmChannel);

  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("connecting to WiFi");
    delay(50);
  }
  Serial.println();
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(laser, HIGH);
  //displayTemp();
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
     // Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  bool intruder = digitalRead(ldr);
  if(warning && (intruder && pinStatus[3])){
     bot.sendMessage(chat_id1, "Warning : Intruder Detected", "");
  }
}
