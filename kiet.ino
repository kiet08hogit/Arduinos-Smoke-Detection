const int ThermistorPin = A0;

const int led1 = 2;
const int led2 = 3;
const int led3 = 4;

float R1 = 10000.0;
float c1 = 1.009249522e-03;
float c2 = 2.378405444e-04;
float c3 = 2.019202697e-07;

float threshold = 80;   // Fahrenheit

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

}

void loop() {

  int Vo = analogRead(ThermistorPin);
  if (Vo == 0) return;   // safety

  float R2 = R1 * (1023.0 / Vo - 1.0);
  float logR2 = log(R2);

  float T = 1.0 / (c1 + c2*logR2 + c3*pow(logR2, 3));
  T = T - 273.15; // Celsius

  float Tf = (T * 9.0)/5.0 + 32.0;

  Serial.print("Temperature: ");
  Serial.println(Tf);

  if (Tf > threshold) {
    Serial.println("🔥 FIRE DETECTED");
    Serial1.println("Fire");
    delay(500);

    // blink all LEDs
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(300);

    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(300);
  } 
  else {
    // all off
    Serial1.println("Done");
    delay(500);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }
}