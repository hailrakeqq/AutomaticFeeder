#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <Arduino.h>
#include <EEPROM.h>

class StepperMotor {
private:
    int8_t* motorPins;
    int feedSpeed;
    int stepsBack;
    int stepsForward;
    int feedAmount;

public:
    StepperMotor(int8_t motorPins[], int feedSpeed, int stepsBack, int stepsForward, int feedAmount);

    void disableMotor();
    void runMotor(int8_t dir);
    void oneRev();
    void feed();
};

#endif // STEPPER_MOTOR_H
