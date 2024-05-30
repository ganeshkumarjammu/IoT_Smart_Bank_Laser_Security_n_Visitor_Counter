#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#define DHT11_PIN  23 // ESP32 pin GPIO23 connected to DHT11 sensor

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27 (from DIYables LCD), 16 column and 2 rows
DHT dht11(DHT11_PIN, DHT11);

void setup() {
  dht11.begin(); // initialize the DHT11 sensor
  lcd.begin();         // initialize the lcd
  lcd.backlight();    // open the backlight
  lcd.setCursor(0, 0);
  lcd.print("Dharmic");
  lcd.setCursor(0, 1);
  lcd.print("Programmer");
  delay(3000);
}

void loop() {
  float humi  = dht11.readHumidity();    // read humidity
  float tempC = dht11.readTemperature(); // read temperature

  lcd.clear();
  // check whether the reading is successful or not
  if (isnan(tempC) || isnan(humi)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 0);  // display position
    lcd.print("Temp: ");
    lcd.print(tempC);     // display the temperature
    lcd.print("°C");

    lcd.setCursor(0, 1);  // display position
    lcd.print("Humi: ");
    lcd.print(humi);      // display the humidity
    lcd.print("%");
  }

  // wait a 2 seconds between readings
  delay(2000);
}
