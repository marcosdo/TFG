int sensVal;
int sensLow = 1023;
int sensHigh = 0;

const int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, HIGH);

  while (millis() < 5000) {
    sensVal = analogRead(A0);

    if (sensVal > sensHigh) {
      sensHigh = sensVal;
    }

    if (sensVal < sensLow) {
      sensLow = sensVal;
    } 
  }

  digitalWrite(ledPin, LOW);

  Serial.println("Setup finished");
}

void loop() {
  sensVal = analogRead(A0);

  int pitch = map(sensVal, sensLow, sensHigh, 50, 4000);

  tone(8, pitch, 20);

  delay(10);
}
