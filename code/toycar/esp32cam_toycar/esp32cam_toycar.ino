#include <WiFi.h>

#include "credentials.h"

#include "MyWebServer.h"

const char* ssid = NETWORK;
const char* password = PASSWORD;

const int pinForward = 12;
const int pinBackward = 13;
const int pinEnaleMotor = 15;
const int pinServo = 14;

const int port = 80;

MyCamera  cam;
MyServo   srv(pinServo);
MyMotor   mot(pinForward, pinBackward, pinEnaleMotor);

MyWebServer mws(cam, mot, srv, port);

String msg = "";

void setup() {
  Serial.begin(115200);

  // ===========================
  // Setting up the server
  // ===========================
  WiFi.begin(ssid, password);
  Serial.print(" Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Done!\n");

  Serial.println(" => Conectado a la red Wi-Fi: " + String(ssid));
  Serial.println(" => Dirección IP: " + WiFi.localIP().toString());
  mws.setupServer();
  Serial.println(" => WebSocket server started");

  // ===========================
  // Setting up the camera
  // ===========================
  if (!cam.initCamera()) {
    Serial.println(" <ERROR> Initializing camera");
    return;
  }
  msg = "\n => Initiated camera";
  Serial.println(msg);
  mws.logMessage(msg);

  // ===========================
  // Setting up the motor
  // ===========================
  mot.setupMotor();
  msg = " => Initiated motor";
  Serial.println(msg);
  mws.logMessage(msg);

  // ===========================
  // Setting up the servo
  // ===========================
  srv.setupServo();
  msg = " => Initiated servo\n";
  Serial.println(msg);
  mws.logMessage(msg);
}

void loop() {
  mws.handleLoop();
}