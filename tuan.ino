void setup() {
  pinMode(buzzerPin, OUTPUT);

  pinMode(motorEN, OUTPUT);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);

}

void loop() {

  // ---- Read Temperature ----
  raw = analogRead(tempPin);
  voltage = raw * (5.0 / 1023.0);
  tempC = (voltage - 0.5) * 100.0;   // TMP36 style

  // ---- Check Fire Condition ----
  if (tempC > threshold) {
    runFan();
    runBuzzer();
  } else {
    stopFan();
    noTone(buzzerPin);
  }
}

// -------- FAN FUNCTIONS --------
void runFan() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  analogWrite(motorEN, 200);  // medium speed
}

void stopFan() {
  analogWrite(motorEN, 0);
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
}

// -------- BUZZER--------
void runBuzzer() {

  unsigned long currentTime = millis();

  if (currentTime - lastBeep > beepInterval) {
    lastBeep = currentTime;
    beepState = !beepState;

    if (beepState) {
      tone(buzzerPin, 1000);
    } else {
      tone(buzzerPin, 600);
    }
  }
}