#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "StepperMotor.h"

class HttpServer{
public:
    HttpServer();
    void begin(const char* ssid, const char* password, StepperMotor stepperMotor);
    void handleRequest();
    void handleFeed(StepperMotor stepperMotor);
    void handleChangeFeedAmount();
    void handleGetFeedAmount();

private:
    ESP8266WebServer server;
    void handleRoot();
};

#endif