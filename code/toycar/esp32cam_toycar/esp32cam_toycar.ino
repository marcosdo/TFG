#include <WiFi.h>

#include "credentials.h"

#include "MyWebServer.h"

const char* ssid = NETWORK;
const char* password = PASSWORD;

const int pinForward = 12;
const int pinBackward = 13;
const int pinEnaleMotor = 15;

const int port = 80;

MyCamera cam;
MyMotor mot(pinForward, pinBackward, pinEnaleMotor);
MyWebServer mws(cam, mot, port);


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
  Serial.println("\n => Initiated camera");

  // ===========================
  // Setting up the motors
  // ===========================
  mot.setupMotor();
  Serial.println(" => Initiated motor\n");
}

void loop() {
  mws.handleLoop();
}