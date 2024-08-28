#ifndef MYSERVO_H
#define MYSERVO_H

#include <ESP32Servo.h>

#include "definitions.h"

class MyServo {
private:
  Servo _servo;

  int _pin;

  int _angle;     // Current angle 

  int _maxAngle;  // Angle = 180
  int _midAngle;  // Angle = 90
  int _minAngle;  // Angle = 0

public:
  MyServo(const int pinServo);
  ~MyServo() {};

  void setupServo();

  void right(int newAnglee = MAX_ANGLE);
  void center(int newAngle = MID_ANGLE);
  void left(int newAngle = MIN_ANGLE);

private:
  void angle(int newAngle);
  void sweep(int delayTime = 10);        // Delay in ms
};

// ===========================
// Constructor and destructor
// ===========================
MyServo::MyServo(const int pinServo) :
  _pin(pinServo),
  _angle(MID_ANGLE),
  _maxAngle(MAX_ANGLE),
  _midAngle(MID_ANGLE),
  _minAngle(MIN_ANGLE)
{ }

// ===========================
// Public methods
// ===========================
void MyServo::setupServo() {
  _servo.attach(_pin);
  sweep();
}

void MyServo::right(int newAngle) {
  angle(newAngle);
}

void MyServo::center(int newAngle) {
  angle(newAngle);
}

void MyServo::left(int newAngle) {
  angle(newAngle);
}

// ===========================
// Private methods
// ===========================
void MyServo::angle(int newAngle) {
  _angle = constrain(newAngle, _minAngle, _maxAngle);
  _servo.write(_angle);
}

void MyServo::sweep(int delayTime) {
  for (int pos = _minAngle; pos <= _maxAngle; pos++) {
    angle(pos);
    delay(delayTime);
  }
  for (int pos = _maxAngle; pos >= _minAngle; pos--) {
    angle(pos);
    delay(delayTime);
  }
  left();
  right();
  center();
}

#endif