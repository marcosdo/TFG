#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "credentials.h"
#include "definitions.h"

#include "handlers.h"
#include "html.h"

const char* ssid = NETWORK;
const char* password = PASSWORD;

WebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Done!\n");

  Serial.print(" => Conectado a la red Wi-Fi: ");
  Serial.println(ssid);
  Serial.print(" => Dirección IP: ");
  Serial.println(WiFi.localIP());

  MDNS.begin("esp32tfg") ? 
    Serial.println(" => MDNS responder started") :
    Serial.println(" <ERROR> Could not set up mDNS");

  server.on("/", HTTP_GET, handleRoot);
  server.begin();

  Serial.println(" => Server HTTP started");
}

void loop() {
  server.handleClient();
}