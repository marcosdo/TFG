const int bridgeDrive = 12;
const int bridgeRevrs = 13;
const int bridgeStart = 15;

const int buttonGears = 14;
const int buttonStart = 2;

bool startST      = false;
bool prevStartST  = false;

bool driveST      = false;
bool prevDriveST  = false;

int motorEnabled    = 0;
int motorSpeed      = 0;
int motorDirection  = 1;

void setup() {
  pinMode(buttonGears, INPUT);
  pinMode(buttonStart, INPUT);

  pinMode(bridgeDrive, OUTPUT);
  pinMode(bridgeRevrs, OUTPUT);
  pinMode(bridgeStart, OUTPUT);

  // Configuración de canal PWM usando la nueva API ledcAttach
  ledcAttach(bridgeStart, 5000, 8);  // Pin, Canal, Frecuencia, Resolución
  // Inicia con el motor desactivado (0% de PWM)
  ledcWrite(0, 0);  
}

void loop() {
  // MOTOR START
  startST = digitalRead(buttonStart);

  motorSpeed = 128;

  if (startST != prevStartST) {
    if (startST == HIGH) {
      Serial.println(" => Start: " + motorEnabled);
      motorEnabled = !motorEnabled;
    }
  }
  prevStartST = startST;

  if (motorEnabled) {
    ledcWrite(bridgeStart, motorSpeed);
  } else {
    ledcWrite(bridgeStart, 0);
  }

  // MOTOR DIRECTION
  driveST = digitalRead(buttonGears);
  
  if (driveST != prevDriveST) {
    if (driveST == HIGH) {
      Serial.println(" => Direction: " + motorDirection);
      motorDirection = !motorDirection;
    }
  }
  prevDriveST = driveST;

  if (motorDirection == 1) {
    digitalWrite(bridgeDrive, HIGH);
    digitalWrite(bridgeRevrs, LOW);
  }
  else {
    digitalWrite(bridgeDrive, LOW);
    digitalWrite(bridgeRevrs, HIGH);
  }
}