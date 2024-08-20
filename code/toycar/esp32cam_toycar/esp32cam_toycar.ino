#include <WiFi.h>
#include <ESPmDNS.h>

#include "credentials.h"
#include "definitions.h"

#include "MyWebServer.h"

const char* ssid = NETWORK;
const char* password = PASSWORD;

MyWebServer mws(80);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Done!\n");

  Serial.println(" => Conectado a la red Wi-Fi: " + String(ssid));
  Serial.println(" => Dirección IP: " + WiFi.localIP().toString());

  MDNS.begin("esp32tfg") ? 
    Serial.println(" => MDNS responder started") :
    Serial.println(" <ERROR> Could not set up mDNS");

  mws.setupServer();

  Serial.println(" => Server HTTP started");
}

void loop() {
  mws.handleLoop();
}