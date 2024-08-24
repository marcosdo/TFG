#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include <functional>

#include "MyCamera.h"

#include "webcode.h"

class MyWebServer {
private:
  AsyncWebServer  _server;
  AsyncWebSocket  _wsCam;
  AsyncWebSocket  _wsFPS;
  MyCamera      & _cam;
  float           _fps;
  float           _sum;
  int             _iter;
  unsigned long   _lastFrameTime;
  int             _clientID;

public:
  MyWebServer(MyCamera &cam, int port = 80);
  ~MyWebServer() {};

  void setupServer();
  void handleLoop();

private:
  void handleNotFound(AsyncWebServerRequest *request);
  void handle_html(AsyncWebServerRequest *request);
  void handle_ico(AsyncWebServerRequest *request);
  void handle_css(AsyncWebServerRequest *request);
  void handleStream();
  void handleFPS();

  void onEventCamera(
    AsyncWebSocket        * server, 
    AsyncWebSocketClient  * client, 
    AwsEventType            type, 
    void                  * arg, 
    uint8_t               * data, 
    size_t                  len
  );
};

// ===========================
// Constructor and destructor
// ===========================
MyWebServer::MyWebServer(MyCamera &cam, int port) : 
  _server(port), 
  _wsCam("/wsCam"), 
  _wsFPS("/wsFPS"), 
  _cam(cam), 
  _fps(0.0),
  _lastFrameTime(0), 
  _sum(0), 
  _iter(1),
  _clientID(0)
{ }

// ===========================
// Public methods
// ===========================
void MyWebServer::setupServer() {
  _server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) { this->handle_html(request); });
  _server.on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){ this->handle_ico(request); });
  _server.on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request){ this->handle_css(request); });

  _server.onNotFound([this](AsyncWebServerRequest *request){ this->handleNotFound(request); });

  _wsCam.onEvent(
    [this](
      AsyncWebSocket        * server,
      AsyncWebSocketClient  * client,
      AwsEventType            type, 
      void                  * arg, 
      uint8_t               * data, 
      size_t                  len
    ) {
      this->onEventCamera(server, client, type, arg, data, len);
    }
  );

  _server.addHandler(&_wsCam);
  _server.addHandler(&_wsFPS);

  _server.begin();
}

void MyWebServer::handleLoop() {
  handleStream();
  //handleFPS();

  _wsCam.cleanupClients();
  _wsFPS.cleanupClients();
}

// ===========================
// Private methods
// ===========================
void MyWebServer::handle_html(AsyncWebServerRequest *request) {
  Serial.println(" => Access to '/' - handle_html(..)");
  request->send(200, "text/html", index_html);
}

void MyWebServer::handle_ico(AsyncWebServerRequest *request) {
  Serial.println(" => Access to '/favicon.ico' - handle_ico(..)");
  request->send_P(200, "image/x-icon", favicon_ico, sizeof(favicon_ico));
}

void MyWebServer::handle_css(AsyncWebServerRequest *request) {
  Serial.println(" => Access to '/style.css' - handle_css(..)");
  request->send_P(200, "text/css", style_css);
}

void MyWebServer::handleNotFound(AsyncWebServerRequest *request) {
  Serial.println(" <ERROR> Access to handleNotFound(..)");

  String message = "File Not Found \n\n URI: ";
    message += request->url();
    message += "\n Method: ";
    message += (request->method() == HTTP_GET) ? "GET\n" : "POST\n";
  request->send(404, "text/plain", message);
}

void MyWebServer::onEventCamera(
  AsyncWebSocket        * server, 
  AsyncWebSocketClient  * client, 
  AwsEventType            type, 
  void                  * arg, 
  uint8_t               * data, 
  size_t                  len
) {
  switch (type) {
    case WS_EVT_CONNECT: 
      _clientID = client->id();
      Serial.printf(" => WebSocket client #%u connected from %s\n", 
        _clientID, client->remoteIP().toString().c_str()
      );
    break;

    case WS_EVT_DISCONNECT: 
      _clientID = 0;
      Serial.printf(" => WebSocket client #%u disconnected\n", client->id());
    break;

    case WS_EVT_DATA: 
    break;

    default: break;
  }
}


void MyWebServer::handleStream() {
  if (_clientID == 0)
    return;

  camera_fb_t *fb = _cam.captureFrame();
  if (!fb) {
    Serial.println(" <ERROR> Frame capture");
    //_server.send(503, "text/plain", "<ERROR> Frame capture");
    return;
  }

  _wsCam.binary(_clientID, fb->buf, fb->len);
  esp_camera_fb_return(fb);

  while (true) {
    AsyncWebSocketClient * clientPointer = _wsCam.client(_clientID);
    if (!clientPointer || !(clientPointer->queueIsFull())) {
      break;
    }
    delay(1);
  }
}

void MyWebServer::handleFPS() {
  unsigned long currentTime = millis();
  _fps = 1000.0 / (currentTime - _lastFrameTime);
  _lastFrameTime = currentTime;

  _sum += _fps;

  float avg = _sum / _iter++;

  if (_iter % 1000000 == 0) // Do not flood the serial monitor
    Serial.println(" => FPS: Avg = " + String(avg) + " | Act = " + String(_fps));
}

#endif
