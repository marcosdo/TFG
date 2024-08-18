#include <WiFi.h>
#include <WebServer.h>

#include "credentials.h"

const char* ssid = NETWORK;
const char* password = PASSWORD;

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Bienvenido a la ESP32-CAM</h1>");
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a la red Wi-Fi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}
