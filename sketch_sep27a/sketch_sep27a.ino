#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address may vary; use an I2C scanner to find the correct address.

// Define the correct UID as a string
char correctUID[] = "01-23-45-67";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();

  servo.attach(8);  // Connect servo to digital pin 8

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place RFID card");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (compareUID(mfrc522.uid, correctUID)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access granted");
      
      // Move servo for 10 seconds
      servo.write(90);  // Assuming 90 degrees is the middle position
      delay(10000);     // 10 seconds delay
      servo.write(0);   // Return servo to initial position
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access denied");
    }
    mfrc522.PICC_HaltA();
  }
}

bool compareUID(MFRC522::Uid uid, const char* correctUID) {
  String uidStr = "";
  for (byte i = 0; i < uid.size; i++) {
    uidStr += String(uid.uidByte[i] < 0x10 ? "0" : "");
    uidStr += String(uid.uidByte[i], HEX);
    uidStr += "-";
  }
  uidStr.remove(uidStr.length() - 1); // Remove the trailing '-'

  return uidStr == correctUID;
}
