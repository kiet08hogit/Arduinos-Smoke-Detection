#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);


char fireLine[17]   = "GOOD DAY SO FAR";
char peopleLine[17] = "No one in room";


long duration;
int distance;
int fire = 0;


const int trigPin = 11;
const int echoPin = 10;


unsigned long previousTime = 0;
const unsigned long interval = 60;   // 60 ms


void setup() {
 Serial.begin(9600);
 Serial1.begin(9600);


 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);


 lcd.init();
 lcd.backlight();
 updateLCD();
}


void updateLCD() {
 lcd.setCursor(0, 0);
 lcd.print("                ");
 lcd.setCursor(0, 0);
 lcd.print(fireLine);


 lcd.setCursor(0, 1);
 lcd.print("                ");
 lcd.setCursor(0, 1);
 lcd.print(peopleLine);
}


void printPeople(int occupied) {
 snprintf(peopleLine, 17, occupied ? "People in room" : "No one in room");
}


void printFire(int fireState) {
 snprintf(fireLine, 17, fireState ? "EMERGENCY: FIRE" : "GOOD DAY SO FAR");
}


void loop() {
 unsigned long currentTime = millis();


 // Read signal first and update LCD immediately
 while (Serial1.available()) {
   String msg = Serial1.readStringUntil('\n');
   msg.trim();


   Serial.print("Received: ");
   Serial.println(msg);


   if (msg == "Fire") {
     fire = 1;
   } else if (msg == "Done") {
     fire = 0;
   }


   printFire(fire);
   updateLCD();
 }


 // Update ultrasonic part on interval
 if (currentTime - previousTime >= interval) {
   previousTime = currentTime;


   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);


   duration = pulseIn(echoPin, HIGH, 30000);   // timeout added


   if (duration > 0) {
     distance = duration * 0.034 / 2;


     Serial.print("Distance: ");
     Serial.println(distance);


     if (distance > 0 && distance < 20) {
       printPeople(1);
     } else {
       printPeople(0);
     }
   } else {
     printPeople(0);
   }


   printFire(fire);
   updateLCD();
 }
}
