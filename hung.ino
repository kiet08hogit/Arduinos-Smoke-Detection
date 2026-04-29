#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 3


const int buttonPin = 2;


// Button Debounce Variables
int buttonState = LOW;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;


MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo gateServo;


// Authorized UID
byte allowedUID[4] = {0xD0, 0x4F, 0xB7, 0x5F};
SoftwareSerial mySerial(6,7); // RX, TX


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);


  Serial.begin(9600);
  mySerial.begin(9600);


  SPI.begin();
  mfrc522.PCD_Init();


  gateServo.attach(SERVO_PIN);
  gateServo.write(0); // Gate starts CLOSED


  Serial.println("\nScan card to open gate...");
}


void loop() {


  unsigned long now = millis();
  // Button Debounce
  int reading = digitalRead(buttonPin);


  if (reading != lastButtonState) {
    lastDebounceTime = now;
  }


  if ((now - lastDebounceTime) > debounceDelay) {


    if (reading != buttonState) {
      buttonState = reading;


      if (buttonState == LOW) {   // pressed (using INPUT_PULLUP)
      }
    }
  }


  lastButtonState = reading;
 
  // Wait for card
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;


  Serial.print("");
  bool accessGranted = true;


  if (mySerial.available()) {
      String msg = mySerial.readStringUntil('\n');
      msg.trim();


      Serial.print("Received: [");
      Serial.print(msg);
      Serial.println("]");
  }
  else{
    // Check UID
    for (byte i = 0; i < 4; i++) {
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      Serial.print(" ");


      if (mfrc522.uid.uidByte[i] != allowedUID[i]) {
        accessGranted = false;
      }
    }
    Serial.println();
  }


  if (accessGranted && !mySerial.available()) {
    Serial.println("Access Granted - Opening Gate");


    gateServo.write(90); // OPEN
    delay(3000);


    gateServo.write(0);  // CLOSE
    Serial.println("Gate Closed");
  } else if (accessGranted && mySerial.available()) {
    gateServo.write(90); // OPEN
  } else {
    Serial.println("Access Denied");
  }


  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();


  delay(1000);
}
