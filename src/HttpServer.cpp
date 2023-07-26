#include "../include/HttpServer.h"

HttpServer::HttpServer(): server(80){}

void HttpServer::begin(const char* ssid, const char* password){
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.print('.');
    }
    Serial.println("\nWiFi connected");
    Serial.println("\nServer Address: " + WiFi.localIP().toString());

    server.on("/", [this](){
        this->handleRoot();
    });
    //server.on("url", callback) - to add new endpoint
    server.begin();
    Serial.println("HTTP server has been started");
}


void HttpServer::handleRequest(){
    server.handleClient();
}

void HttpServer::handleRoot(){
    server.send(200, "text/plain", "Hello from NodeMCU!!!");
}