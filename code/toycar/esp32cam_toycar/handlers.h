#ifndef HANDLERS_H
#define HANDLERS_H

#include <WebServer>

void handleRoot(WebServer ws) {
  ws.send(200, "text/html", html_index);
}

#endif