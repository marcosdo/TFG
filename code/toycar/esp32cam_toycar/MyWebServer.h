#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <WebServer.h>

#include <functional>

#include "MyCamera.h"
#include "webcode.h"

class MyWebServer {
private:
  WebServer _server;
  MyCamera &_cam;

public:
  MyWebServer(MyCamera &cam, int port = 80);
  ~MyWebServer();

  void setupServer();
  void handleLoop();

private:
  void handleRoot();
  void handleStream();
};

// ===========================
// Constructor and destructor
// ===========================
MyWebServer::MyWebServer(MyCamera &cam, int port) : _server(port), _cam(cam) { }
MyWebServer::~MyWebServer() { }

// ===========================
// Public methods
// ===========================
void MyWebServer::setupServer() {
  _server.on("/", HTTP_GET, std::bind(&MyWebServer::handleRoot, this));
  _server.on("/capture", HTTP_GET, std::bind(&MyWebServer::handleStream, this));
  _server.begin();
}

void MyWebServer::handleLoop() {
  _server.handleClient();
}

// ===========================
// Private methods
// ===========================
void MyWebServer::handleRoot() {
  Serial.println(" => Access to handleRoot()");
  _server.send(200, "text/html", index_html);
}

void MyWebServer::handleStream() {
  camera_fb_t *fb = _cam.captureFrame();
  if (!fb) {
    Serial.println(" <ERROR> Frame capture");
    _server.send(503, "text/plain", "Error capturando el frame");
    return;
  }
  _server.sendHeader("Content-Type", "image/jpeg");
  _server.sendHeader("Content-Length", String(fb->len));
  _server.send_P(200, "image/jpeg", reinterpret_cast<const char*>(fb->buf), fb->len);
  esp_camera_fb_return(fb);
}

#endif
