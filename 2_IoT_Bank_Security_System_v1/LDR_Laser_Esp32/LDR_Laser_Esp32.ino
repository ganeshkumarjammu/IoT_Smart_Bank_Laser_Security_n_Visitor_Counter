const int LDR = 32;  // LDR pin connected to ESP32 analog pin
const int laser = 14; // Pin connected to laser diode

void setup() {
  Serial.begin(9600);
  pinMode(LDR, INPUT);
  pinMode(laser, OUTPUT);
}

void loop() {
  // Turn on the laser
  digitalWrite(laser, HIGH);
  
  // Read the LDR value
  int ldrValue = analogRead(LDR);
  
  // Print the LDR value
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Check if the laser beam is interrupted
  if (ldrValue < 2700) {
    Serial.println("Intruder detected!");
    // You can add additional actions here, such as sending an alert message
  }

  delay(1000); // Adjust delay as needed
}
