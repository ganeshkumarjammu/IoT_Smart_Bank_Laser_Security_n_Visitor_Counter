#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int IN_IR = 34;
const int OUT_IR = 35;

int visitors = 0;

// Initialize the LCD with I2C address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  //Serial.begin(9600);
  pinMode(IN_IR, INPUT);
  pinMode(OUT_IR, INPUT);

  // Initialize the LCD
  lcd.begin();
  lcd.backlight(); // Turn on the backlight
}

void loop() {
  int inState = digitalRead(IN_IR);
  int outState = digitalRead(OUT_IR);

  if (!inState) {
    visitors++;
    updateLCD();
    delay(1000);
  }

  if ((!outState) && visitors > 0) {
    visitors--;
    updateLCD();
    delay(1000);
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Visitor Counter");
  lcd.setCursor(0, 1);
  lcd.print("Visitors:");
  lcd.print(visitors);
}
