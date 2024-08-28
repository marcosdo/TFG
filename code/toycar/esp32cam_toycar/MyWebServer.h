#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include <functional>

#include "MyCamera.h"
#include "MyMotor.h"
#include "MyServo.h"

#include "webcode.h"

class MyWebServer {
private:
  AsyncWebServer  _server;  // Server (usually :80)

  AsyncWebSocket  _wsCam;   // WebSocket Camera
  AsyncWebSocket  _wsMot;   // WebSocket Motor
  AsyncWebSocket  _wsSer;   // WebSocket Servo
  AsyncWebSocket  _wsLog;

  MyCamera      & _cam;     // Camera
  MyMotor       & _mot;     // Motor
  MyServo       & _ser;     // Servo

  float           _fps;     // FPS
  float           _sum;
  int             _iter;
  unsigned long   _lastFrameTime;

  int             _clientID;  // Conection - Client ID

  String          _msg;
public:
  MyWebServer(MyCamera &cam, MyMotor &mot, MyServo &ser, int port = 80);
  ~MyWebServer() {};

  void setupServer();
  void handleLoop();
  void logMessage(String message);

private:
  void handle_html(AsyncWebServerRequest *request);
  void handle_ico(AsyncWebServerRequest *request);
  void handle_css(AsyncWebServerRequest *request);

  void handleNotFound(AsyncWebServerRequest *request);

  void handleStream();
  void handleMotor();
  void handleServo();
  
  void handleFPS();

  void onEventCamera(
    AsyncWebSocket        * server, 
    AsyncWebSocketClient  * client, 
    AwsEventType            type, 
    void                  * arg, 
    uint8_t               * data, 
    size_t                  len
  );
  void onEventMotor(
    AsyncWebSocket        * server, 
    AsyncWebSocketClient  * client, 
    AwsEventType            type, 
    void                  * arg, 
    uint8_t               * data, 
    size_t                  len
  );
  void onEventServo(
    AsyncWebSocket        * server, 
    AsyncWebSocketClient  * client, 
    AwsEventType            type, 
    void                  * arg, 
    uint8_t               * data, 
    size_t                  len
  );
  void onEventLog(
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
MyWebServer::MyWebServer(MyCamera &cam, MyMotor &mot, MyServo &ser, int port) : 
  _server(port), 
  _wsCam("/wsCam"), 
  _wsMot("/wsMot"),
  _wsSer("/wsSer"), 
  _wsLog("/wsLog"),
  _cam(cam),
  _mot(mot),
  _ser(ser),
  _fps(0.0),
  _lastFrameTime(0), 
  _sum(0), 
  _iter(1),
  _clientID(0),
  _msg("")
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
  _wsMot.onEvent(
    [this](
      AsyncWebSocket        * server,
      AsyncWebSocketClient  * client,
      AwsEventType            type, 
      void                  * arg, 
      uint8_t               * data, 
      size_t                  len
    ) {
      this->onEventMotor(server, client, type, arg, data, len);
    }
  );
  _wsSer.onEvent(
    [this](
      AsyncWebSocket        * server,
      AsyncWebSocketClient  * client,
      AwsEventType            type, 
      void                  * arg, 
      uint8_t               * data, 
      size_t                  len
    ) {
      this->onEventServo(server, client, type, arg, data, len);
    }
  );
  _wsLog.onEvent(
    [this](
      AsyncWebSocket        * server,
      AsyncWebSocketClient  * client, 
      AwsEventType            type, 
      void                  * arg, 
      uint8_t               * data, 
      size_t                  len
    ) {
      this->onEventLog(server, client, type, arg, data, len);
    }
  );

  _server.addHandler(&_wsCam);
  _server.addHandler(&_wsMot);
  _server.addHandler(&_wsSer);
  _server.addHandler(&_wsLog);

  _server.begin();
}

void MyWebServer::handleLoop() {
  handleStream();

  _wsCam.cleanupClients();
  _wsMot.cleanupClients();
  _wsSer.cleanupClients();
  _wsLog.cleanupClients();
}

void MyWebServer::logMessage(String message) {
  if (_wsLog.count() > 0) {
    _wsLog.textAll(message);
  }
}

// ===========================
// Private methods
// ===========================
void MyWebServer::handle_html(AsyncWebServerRequest *request) {
  _msg = " => Access to '/' - handle_html(..)";
  Serial.println(_msg);
  logMessage(_msg);
  request->send(200, "text/html", index_html);
}

void MyWebServer::handle_ico(AsyncWebServerRequest *request) {
  _msg = " => Access to '/favicon.ico' - handle_ico(..)";
  Serial.println(_msg);
  logMessage(_msg);
  request->send_P(200, "image/x-icon", favicon_ico, sizeof(favicon_ico));
}

void MyWebServer::handle_css(AsyncWebServerRequest *request) {
  _msg = " => Access to '/style.css' - handle_css(..)";
  Serial.println(_msg);
  logMessage(_msg);
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
      logMessage(" => WebSocket camera connected");
      Serial.printf(" => WebSocket camera #%u connected from %s\n", 
        _clientID, client->remoteIP().toString().c_str()
      );
    break;

    case WS_EVT_DISCONNECT: 
      _clientID = 0;
      logMessage(" => WebSocket camera disconnected");
      Serial.printf(" => WebSocket camera #%u disconnected\n", client->id());
    break;

    case WS_EVT_DATA:
      // No data expected for the camera stream WebSocket
    break;

    default: break;
  }
}

void MyWebServer::onEventMotor(
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
      logMessage(" => WebSocket motor connected");
      Serial.printf(" => WebSocket motor #%u connected from %s\n", 
        _clientID, client->remoteIP().toString().c_str()
      );
    break;

    case WS_EVT_DISCONNECT: 
      _clientID = 0;
      logMessage(" => WebSocket motor disconnected");
      Serial.printf(" => WebSocket motor #%u disconnected\n", client->id());
    break;

    case WS_EVT_DATA: {
      AwsFrameInfo * info = (AwsFrameInfo*) arg;
      if (info->final 
        && info->index  == 0 
        && info->len    == len 
        && info->opcode == WS_TEXT
      ) {
        String msg = "";

        for (size_t i = 0; i < len; i++) {
          msg += (char)data[i];
        }

        if      (msg == "D") { _mot.forward();  }
        else if (msg == "R") { _mot.backward(); }
        else if (msg == "N") { _mot.breaks();   } 
      }
    } break;

    default: break;
  }
}

void MyWebServer::onEventServo(
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
      logMessage(" => WebSocket servo connected");
      Serial.printf(" => WebSocket servo #%u connected from %s\n", 
        _clientID, client->remoteIP().toString().c_str()
      );
    break;

    case WS_EVT_DISCONNECT: 
      _clientID = 0;
      logMessage(" => WebSocket servo disconnected");
      Serial.printf(" => WebSocket servo #%u disconnected\n", client->id());
    break;

    case WS_EVT_DATA: {
      AwsFrameInfo * info = (AwsFrameInfo*) arg;
      if (info->final 
        && info->index  == 0 
        && info->len    == len 
        && info->opcode == WS_TEXT
      ) {
        String msg = "";

        for (size_t i = 0; i < len; i++) {
          msg += (char)data[i];
        }

        if      (msg == "L") { _ser.left();   }
        else if (msg == "S") { _ser.right();  }
        else if (msg == "M") { _ser.center(); } 
      }
    } break;

    default: break;
  }
}

void MyWebServer::onEventLog(
  AsyncWebSocket        * server, 
  AsyncWebSocketClient  * client, 
  AwsEventType            type, 
  void                  * arg, 
  uint8_t               * data, 
  size_t                  len
) {
  switch (type) {
    case WS_EVT_CONNECT: 
      logMessage(" => WebSocket log connected");
      Serial.printf(" => WebSocket log #%u connected from %s\n", 
        client->id(), client->remoteIP().toString().c_str()
      );
    break;

    case WS_EVT_DISCONNECT: 
    logMessage(" => WebSocket log disconnected");
      Serial.printf(" => WebSocket log #%u disconnected\n", client->id());
    break;

    case WS_EVT_DATA:
      // No data expected for the log WebSocket
    break;

    default: break;
  }
}

void MyWebServer::handleStream() {
  if (_clientID == 0 || _wsCam.client(_clientID)->queueIsFull())
    return;

  camera_fb_t *fb = _cam.captureFrame();
  if (!fb) {
    Serial.println(" <ERROR> Frame capture");
    return;
  }

  _wsCam.binary(_clientID, fb->buf, fb->len);
  esp_camera_fb_return(fb);

  while (true) {
    AsyncWebSocketClient * clientPointer = _wsCam.client(_clientID);
    if (!clientPointer || !(clientPointer->queueIsFull())) {
      break;
    }
    delay(10);
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
