#ifndef MYMOTOR_H
#define MYMOTOR_H

#include "definitions.h"

class MyMotor {
private:
  int _start;   // 1 = ON | 0 = OFF
  int _speed;   // From 0 to 255
  int _accel;   // From 1 to 3
  int _direc;   // 1 = forward | 0 = backward

  const int _freqz; // 1000 = 1 KHz
  const int _rsltn; // Base = 8

  const int _bridgeDrive; // Pin <--> Bridge: Go forward 
  const int _bridgeRevrs; // Pin <--> Bridge: Go backward
  const int _bridgeStart; // Pin <--> Bridge: Start-Stop

  bool _startST;      // Motor ON-OFF
  bool _prevStartST;  // Motor ON-OFF (previous)

  bool _driveST;      // Motor forward-backward
  bool _prevDriveST;  // Motor forward-backward (previous)


public:
  MyMotor(int GPIO_BridgeDrive, int GPIO_BridgeReverse, int GPIO_BridgeStart);
  ~MyMotor() {};

  void setupMotor();

  void forward(int speed = MAX_SPEED);
  void backward(int speed = MAX_SPEED);
  void lock(int speed = MIN_SPEED);
  void breaks(int speed = MIN_SPEED);

  void rampTo(int targetSpeed = 0, int delay = 1000);

  void setSpeed(int newSpeed);

private:

};

// ===========================
// Constructor and destructor
// ===========================
MyMotor::MyMotor(int GPIO_BridgeDrive, int GPIO_BridgeReverse, int GPIO_BridgeStart) :
  _bridgeDrive(GPIO_BridgeDrive),
  _bridgeRevrs(GPIO_BridgeReverse),
  _bridgeStart(GPIO_BridgeStart),
  _start(MOTOR_STOP),
  _speed(MIN_SPEED),
  _accel(MIN_ACCELERATION),
  _direc(GO_FORWARD),
  _freqz(1000),
  _rsltn(8)
 {}

// ===========================
// Public methods
// ===========================
void MyMotor::setupMotor() {
  pinMode(_bridgeDrive, OUTPUT);
  pinMode(_bridgeRevrs, OUTPUT);
  pinMode(_bridgeStart, OUTPUT);

  // Configuración de canal PWM usando la nueva API ledcAttach
  ledcAttach(_bridgeStart, _freqz, _rsltn);  // Pin, Frecuencia, Resolución
  // Inicia con el motor desactivado (0% de PWM)
  ledcWrite(_bridgeStart, _speed);

  lock();
}

void MyMotor::forward(int speed) {
  digitalWrite(_bridgeDrive, HIGH);
  digitalWrite(_bridgeRevrs, LOW);
  rampTo(speed);
}

void MyMotor::backward(int speed) {
  digitalWrite(_bridgeDrive, LOW);
  digitalWrite(_bridgeRevrs, HIGH);
  rampTo(speed);
}

void MyMotor::breaks(int speed) {
  digitalWrite(_bridgeDrive, LOW);
  digitalWrite(_bridgeRevrs, LOW);
  rampTo(speed);
}

void MyMotor::lock(int speed) {
  digitalWrite(_bridgeDrive, LOW);
  digitalWrite(_bridgeRevrs, LOW);
  setSpeed(speed);
}

void MyMotor::rampTo(int targetSpeed, int delayTotal) {
  int startSpeed = _speed;
  int increment = (targetSpeed > startSpeed) ? _accel : -_accel;
  int steps = abs(targetSpeed - startSpeed);
  int delayPerStep = (steps > 0) ? delayTotal / steps : delayTotal;

  for (int i = 0; i <= steps; i++) {
    setSpeed(startSpeed + i * increment);
    delay(delayPerStep);
  }
  setSpeed(targetSpeed);
}

void MyMotor::setSpeed(int newSpeed) {
  _speed = constrain(newSpeed, -255, 255);
  ledcWrite(_bridgeStart, abs(_speed));
}

#endif