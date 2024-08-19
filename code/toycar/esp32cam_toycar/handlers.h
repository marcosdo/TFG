#ifndef HANDLERS_H
#define HANDLERS_H

#include <WebServer.h>

#include "html.h"

extern WebServer server;

void handleRoot() {
  server.send(200, "text/html", html_index);
}

#endif