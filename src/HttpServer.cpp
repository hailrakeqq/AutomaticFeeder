#include "../include/HttpServer.h"
#include "../include/StepperMotor.h"

HttpServer::HttpServer(): server(80){}

void HttpServer::begin(const char* ssid, const char* password, StepperMotor stepperMotor){
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
    
    server.on("/feed", [this, stepperMotor](){
        this->handleFeed(stepperMotor);
    });

     server.on("/change_feedamount", [this](){
        this->handleChangeFeedAmount();
    });

    server.on("/get_feedamount", [this](){
        this->handleGetFeedAmount();
    });

    server.begin();
    Serial.println("HTTP server has been started");
}

void HttpServer::handleFeed(StepperMotor stepperMotor){
    stepperMotor.feed();
    server.send(200, "text/plain", "Feed working!!!");
}

void HttpServer::handleChangeFeedAmount(){
    Serial.print("test");
}
void HttpServer::handleGetFeedAmount(){
    Serial.print("test");
}

void HttpServer::handleRequest(){
    server.handleClient();
}

void HttpServer::handleRoot(){
    server.send(200, "text/plain", "Hello from NodeMCU!!!");
}