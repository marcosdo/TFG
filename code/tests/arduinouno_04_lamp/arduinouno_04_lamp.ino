const int gLEDp = 9;
const int rLEDp = 10;
const int bLEDp = 11;

const int rSensor = A0;
const int gSensor = A1;
const int bSensor = A2;

int rValue = 0;
int gValue = 0;
int bValue = 0;

int rSensVal = 0;
int gSensVal = 0;
int bSensVal = 0;

void setup() {
  Serial.begin(9600);

  pinMode(gLEDp, OUTPUT);
  pinMode(rLEDp, OUTPUT);
  pinMode(bLEDp, OUTPUT);
}

void loop() {
  rSensVal = analogRead(rSensor);
  delay(5);
  gSensVal = analogRead(gSensor);
  delay(5);
  bSensVal = analogRead(bSensor);

  Serial.print("Raw Sensor Values \t red: ");
  Serial.print(rSensVal);
  Serial.print("\t green: ");
  Serial.print(gSensVal);
  Serial.print("\t blue: ");
  Serial.println(bSensVal);

  rValue = rSensVal / 4;
  gValue = gSensVal / 4;
  bValue = bSensVal / 4;

  Serial.print("Mapped Sensor Values \t red: ");
  Serial.print(rValue);
  Serial.print("\t green: ");
  Serial.print(gValue);
  Serial.print("\t blue: ");
  Serial.println(bValue);

  analogWrite(rLEDp, rValue);
  analogWrite(gLEDp, gValue);
  analogWrite(bLEDp, bValue);
}
