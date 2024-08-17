const int switchPin = 8;

unsigned long prevTime = 0;

int switchState = 0;
int prevSwitchState;

int led = 2;
long interval = 1000;

void setup() {
  for(int x = 2; x < 8; x++) {
    pinMode(x, OUTPUT);
  }

  pinMode(switchPin, INPUT);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - prevTime > interval) {
    prevTime = currentTime;

    digitalWrite(led, HIGH);

    led++;
  }

  if (led == 7) {
    // Apagar
  }

  switchState = digitalRead(switchPin);

  if (switchState != prevSwitchState) {
    for(int x = 2; x < 8; x++) {
      digitalWrite(led, LOW);
    }

    led = 2;
    prevTime = currentTime;
  }

  prevSwitchState = switchState;
}
