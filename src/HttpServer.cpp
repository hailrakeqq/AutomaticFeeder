#include "../include/HttpServer.h"
#include "../include/StepperMotor.h"

HttpServer::HttpServer(): server(80){}

void HttpServer::begin(const char* ssid, const char* password, StepperMotor *stepperMotor){
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

    server.on("/change_feedamount", HTTP_POST, [this](){
        this->handleChangeFeedAmount();
    });

    server.on("/get_feedamount", [this](){
        this->handleGetFeedAmount();
    });

    server.begin();
    Serial.println("HTTP server has been started");
}

void HttpServer::handleFeed(StepperMotor* stepperMotor){
    stepperMotor->feed();
    server.send(200, "text/plain", "feed working.");
}

void HttpServer::handleChangeFeedAmount(){
      if (server.method() == HTTP_POST) {
        if (server.hasArg("plain")) {
            int value = server.arg("plain").toInt();
            
            EEPROM.put(1, value);
            EEPROM.commit();
                  
            server.send(200, "text/plain", "Feed Amount was successfully updated");
        } else {
            server.send(400, "text/plain", "Bad Request - Missing JSON data");
        }
    } else {
        server.send(405, "text/plain", "Method Not Allowed");
    }
}

void HttpServer::handleGetFeedAmount(){
    int feedAmount;

    EEPROM.get(1, feedAmount);
    String result = String(feedAmount);
    server.send(200, "text/plain", result);
}

void HttpServer::handleRequest(){
    server.handleClient();
}

void HttpServer::handleRoot(){
    server.send(200, "text/plain", "Hello from NodeMCU!!!");
}