#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class HttpServer{
public:
    HttpServer();
    void begin(const char* ssid, const char* password);
    void handleRequest();

private:
    ESP8266WebServer server;
    void handleRoot();
};

#endif