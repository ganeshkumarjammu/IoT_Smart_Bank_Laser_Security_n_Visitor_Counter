#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>  // Include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h>  // Include the LiquidCrystal I2C library for LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize the LCD with I2C address 0x27, 16x2 display

#define DHT11_PIN 5
DHT dht11(DHT11_PIN, DHT11);

// Define pin numbers 
const int IR_IN = 2;  // Digital pin for IR sensor 1
const int IR_OUT = 3;  // Digital pin for IR sensor 2
const int BUZZER_PIN = 4;
const int FAN_INPUT = 6; //P4 to esp32
const int LDR_PIN = 7;  // Analog pin for LDR or P16 of ESP32
const int FAN_OUTPUT = 9; 
const int ENA = 10;
  // Digital pin for buzzer

// Initialize variables for counting visitors and LDR value
int visitorCount = 0;  // Initialize visitor count
int ldrValue = 0;  // Initialize LDR value
int fanState = 0 ; 
int speed = 150; //fan speed
//DHT11
int humi , tempC ;
int prevTemp = 0;
int prevState = 0;
void setup() {
  Serial.begin(9600);
  pinMode(IR_IN, INPUT);
  pinMode(IR_OUT, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(FAN_INPUT,INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(FAN_OUTPUT, OUTPUT); 
  pinMode(BUZZER_PIN, OUTPUT);  // Set buzzer pin as output
  dht11.begin();     // initialize the sensor
  lcd.begin();  // Initialize the LCD
  lcd.backlight();  // Turn on the backlight
  lcd.setCursor(0, 0);  // Set cursor to the first column of the first row
  lcd.print("IoT Security in");  // Print text to the LCD
  lcd.setCursor(0, 1);  // Set cursor to the first column of the second row
  lcd.print("    Banks ");  // Print text to the LCD
  delay(1000);
  displayTemp();
  updateLCD();
}

void loop() {
  // Read LDR value
  ldrValue = digitalRead(LDR_PIN);
  Serial.print("LDR Value:");
  Serial.println(ldrValue);
  //read DHT11
  humi  = int(dht11.readHumidity());    // read humidity
  tempC = int(dht11.readTemperature()); // read temperature
  Serial.print("Temp:");
  Serial.println(tempC);
  //check the fan 
  fanState = digitalRead(FAN_INPUT) ;
  Serial.print("Fan State:");
  Serial.println(fanState);

  if (ldrValue) {
    // Ring the buzzer
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  if(fanState != prevState){
    prevState = fanState;
    setFan();
  }
 
  //DHT11 based fan speed condition
  if ( (isnan(humi) || isnan(tempC))) {
   //Do Nothing
  } 
  else if(tempC == prevTemp){
    //Do Nothing
  }
  else if(tempC  < 30 ){
    prevTemp = tempC;
    speed = 150;
    setFan();
    displayTemp();
    updateLCD();
  }
  else if((tempC >= 30) && (tempC < 45 )){
    prevTemp = tempC;
    speed = 200 ;
    setFan();
    displayTemp();
    updateLCD();
  }
  else if(tempC > 45){
    prevTemp = tempC;
    speed = 250;
    setFan();
    displayTemp();
    updateLCD();
  }

  // Check IR sensors
  if (digitalRead(IR_IN) == LOW) {
    // IR sensor 1 detects movement, increment visitor count
    visitorCount++;
    updateLCD();
  }
  
  if (digitalRead(IR_OUT) == LOW) {
    // IR sensor 2 detects movement, decrement visitor count
    visitorCount--;
    updateLCD();
  }
}
void displayTemp(){
    lcd.clear();
    lcd.setCursor(0, 0);  // start to print at the first row
    lcd.print("Temp: ");
    lcd.print(tempC);     // print the temperature
    lcd.print((char)223); // print ° character
    lcd.print("C");
    lcd.setCursor(0, 1);  // start to print at the second row
    lcd.print("Humi: ");
    lcd.print(humi);      // print the humidity
    lcd.print("%");
    delay(1000);
}

void updateLCD(){
  // Update LCD
  lcd.clear();  // Clear the LCD display
  lcd.setCursor(0, 0);  // Set cursor to the first column of the first row
  lcd.print("IoT Bank of ACE");  // Print text to the LCD
  lcd.setCursor(0, 1);  // Set cursor to the first column of the second row
  lcd.print("Visitors: ");  // Print text to the LCD
  lcd.print(visitorCount);  // Print visitor count to the LCD
  Serial.print("Visitors:");
  Serial.println(visitorCount);
  delay(500); // Update LCD every 1 second
}

void setFan() {
  // Set fan speed using L298N motor driver
  analogWrite(ENA, speed);  // Set speed using PWM
  digitalWrite(FAN_OUTPUT, fanState);  // Set direction (clockwise)
}
