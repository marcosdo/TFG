#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <WebServer.h>

#include <functional>

#include "MyCamera.h"
#include "webcode.h"

class MyWebServer {
private:
  WebServer       _server;
  MyCamera      & _cam;
  float           _fps;
  float           _sum;
  int             _iter;
  unsigned long   _lastFrameTime;
public:
  MyWebServer(MyCamera &cam, int port = 80);
  ~MyWebServer();

  void setupServer();
  void handleLoop();

private:
  void handleRoot();
  void handleStream();
  void handleFPS();
};

// ===========================
// Constructor and destructor
// ===========================
MyWebServer::MyWebServer(MyCamera &cam, int port) 
  : _server(port), _cam(cam), _fps(0.0), _lastFrameTime(0), _sum(0), _iter(1) { }
MyWebServer::~MyWebServer() { }

// ===========================
// Public methods
// ===========================
void MyWebServer::setupServer() {
  _server.on("/", HTTP_GET, std::bind(&MyWebServer::handleRoot, this));
  _server.on("/capture", HTTP_GET, std::bind(&MyWebServer::handleStream, this));
  _server.on("/fps", HTTP_GET, std::bind(&MyWebServer::handleFPS, this));
  _server.begin();
}

void MyWebServer::handleLoop() {
  _server.handleClient();
}

// ===========================
// Private methods
// ===========================
void MyWebServer::handleRoot() {
  Serial.println(" => Access to '/' - handleRoot()");
  _server.send(200, "text/html", index_html);
}

void MyWebServer::handleStream() {
  camera_fb_t *fb = _cam.captureFrame();

  if (!fb) {
    Serial.println(" <ERROR> Frame capture");
    _server.send(503, "text/plain", "<ERROR> Frame capture");
    return;
  }

  _server.sendHeader("Content-Type", "image/jpeg");
  _server.sendHeader("Content-Length", String(fb->len));
  _server.send_P(200, "image/jpeg", reinterpret_cast<const char*>(fb->buf), fb->len);
  esp_camera_fb_return(fb);
}

void MyWebServer::handleFPS() {
  unsigned long currentTime = millis();
  _fps = 1000.0 / (currentTime - _lastFrameTime);
  _lastFrameTime = currentTime;

  _sum += _fps;

  float avg = _sum / _iter++;

  if (_iter % 30 == 0) // Do not flood the serial monitor
    Serial.println(" => FPS: Avg = " + String(avg) + " | Act = " + String(_fps));
  
  _server.send(200, "text/plain", String(_fps));
}

#endif
