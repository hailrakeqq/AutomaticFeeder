#include "StepperMotor.h"

const byte steps[] = {0b1010, 0b0110, 0b0101, 0b1001};
const byte NUMBER_OF_PHASE = 4;

StepperMotor::StepperMotor(int8_t motorPins[], int feedSpeed, int stepsBack, int stepsForward, int feedAmount)
    : motorPins(motorPins), feedSpeed(feedSpeed), stepsBack(stepsBack), stepsForward(stepsForward), feedAmount(feedAmount) {
        for (byte i = 0; i < NUMBER_OF_PHASE; i++)
            pinMode(motorPins[i], OUTPUT);
    }

void StepperMotor::disableMotor() {
    for (byte i = 0; i < NUMBER_OF_PHASE; i++)
        digitalWrite(motorPins[i], LOW);
}

void StepperMotor::runMotor(int8_t dir) {
    static byte step = 0;
    for (byte i = 0; i < NUMBER_OF_PHASE; i++)
        digitalWrite(motorPins[i], bitRead(steps[step & 0b11], i));
    delayMicroseconds(feedSpeed);
    step += dir;
}

void StepperMotor::oneRev() {
    for (int i = 0; i < stepsBack; i++)
        runMotor(-1);
    for (int i = 0; i < stepsForward; i++)
        runMotor(1);
}

void StepperMotor::feed() {
    EEPROM.get(1, feedAmount);
    for (int i = 0; i < feedAmount; i++)
        oneRev();
    disableMotor();
}