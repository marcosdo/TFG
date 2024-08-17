const int sensorPin = A0;
const float baseTmp = 25.0;

void setup() {
  Serial.begin(9600);
  for (int pin = 2; pin < 5; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  Serial.print(" Serial value: ");
  Serial.print(sensorVal);

  float voltage = sensorVal / 1024.0 * 5.0;
  Serial.print(" Volts value: ");
  Serial.print(voltage);

  float tmp = (voltage - .5) * 100;
  Serial.print(" Tmp value: ");
  Serial.println(tmp);

  if (tmp < baseTmp + 2) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }
  else if (tmp >= baseTmp + 2 && tmp < baseTmp + 4) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }
  else if (tmp >= baseTmp + 4 && tmp < baseTmp + 6) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }
  else if (tmp >= baseTmp + 6 && tmp < baseTmp + 8) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
  }

  delay(1);
}
