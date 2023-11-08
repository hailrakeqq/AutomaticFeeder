#include "../include/HttpServer.h"
#include "../include/StepperMotor.h"
#include "../include/secret.h"
#include "ButtonDecoder.cpp"
#include <Arduino.h>
#include <EEPROM.h>

#define EE_RESET 12
#define STEPS_FORWARD 25
#define STEPS_BACK 20
#define FEED_SPEED 3000 // delay between motor steps(ms)
#define NUMBER_OF_PHASE 4
#define BUTTON_PIN D5

int8_t motorPins[] = {D0, D1, D2, D3};

const int mechChangeBtn = D7;
const int led = D6;

int sensorValue = 0;
int outputValue = 0;
int lastOutputValue = 0;
boolean mechChangeBtnState = false;
boolean mechChangeBtnLastState = false;
boolean isMechChangeFeedAmountRunning = false;

int feedAmount = 100;

StepperMotor stepperMotor(motorPins, FEED_SPEED, STEPS_BACK, STEPS_FORWARD,
                          feedAmount);
HttpServer httpServer;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(mechChangeBtn, INPUT_PULLUP);

  EEPROM.begin(4096); // it's should be here if you use esp8266 or esp32
  if (EEPROM.read(0) != EE_RESET) {
    EEPROM.write(0, EE_RESET);
    EEPROM.put(1, feedAmount);
    EEPROM.commit(); // EEPROM.commit() uses only for esp8266 or esp32
  }

  httpServer.begin(SSID, PASSWORD, &stepperMotor);
}

void loop() {
  httpServer.handleRequest();

  if (digitalRead(BUTTON_PIN) == LOW && isMechChangeFeedAmountRunning == false)
    stepperMotor.feed();

  mechChangeBtnState = digitalRead(mechChangeBtn);

  if (mechChangeBtnState == LOW && mechChangeBtnLastState == HIGH) {
    if (isMechChangeFeedAmountRunning) {
      isMechChangeFeedAmountRunning = false;
      analogWrite(led, 0);
      Serial.print("Last Output Value = ");
      Serial.println(lastOutputValue);
      EEPROM.put(1, lastOutputValue);
      EEPROM.commit();
    } else {
      isMechChangeFeedAmountRunning = true;
    }
  }

  if (isMechChangeFeedAmountRunning) {
    sensorValue = analogRead(A0);
    outputValue = map(sensorValue, 0, 1023, 0, 255);
    analogWrite(led, outputValue);
    Serial.print("Output Value = ");
    Serial.println(outputValue);
    lastOutputValue = outputValue;
  }

  mechChangeBtnLastState = mechChangeBtnState;
}