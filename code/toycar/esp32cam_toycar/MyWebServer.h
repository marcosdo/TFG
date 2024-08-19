#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <WebServer.h>

#include <functional>

#include "html.h"

class MyWebServer {
private:
  WebServer _server;

public:
  MyWebServer(int port = 80);
  ~MyWebServer();

  void setupServer();
  void handleLoop();

private:
  void handleRoot();
};

// ===========================
// Constructor and destructor
// ===========================
MyWebServer::MyWebServer(int port) : _server(port) { }
MyWebServer::~MyWebServer() { }

// ===========================
// Public methods
// ===========================
void MyWebServer::setupServer() {
  _server.on("/", HTTP_GET, std::bind(&MyWebServer::handleRoot, this));
  _server.begin();
}

void MyWebServer::handleLoop() {
  _server.handleClient();
}

// ===========================
// Private methods
// ===========================
void MyWebServer::handleRoot() {
  _server.send(200, "text/html", html_index);
}

#endif
