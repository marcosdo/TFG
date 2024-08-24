#include <WiFi.h>

#include "credentials.h"

#include "MyWebServer.h"
#include "MyCamera.h"

const char* ssid = NETWORK;
const char* password = PASSWORD;

MyCamera cam;
MyWebServer mws(cam, 80);


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

  cam.startStream();
}

void loop() {
  mws.handleLoop();
}