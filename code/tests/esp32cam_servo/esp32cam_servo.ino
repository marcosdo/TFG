#include <ESP32Servo.h>

Servo myServo;

int const potPin = 14;

int potentiometer;
int angle;

void setup() {
  Serial.begin(115200);

  myServo.attach(13);
}

void loop() {
  potentiometer = analogRead(potPin);
  Serial.printf(" potentiometer: %d | ", potentiometer);

  angle = map(potentiometer, 0, 4095, 0, 179);
  Serial.printf(" angle: %d\n", angle);

  myServo.write(angle);

  delay(15);
}