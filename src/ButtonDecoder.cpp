#include <Arduino.h>

class ButtonDecoder{
public:
    ButtonDecoder(int pin, unsigned long debouncedDelayMillis = 50){
        buttonPin = pin;
        debounceDelay = debouncedDelayMillis;
        buttonState = HIGH;
        lastButtonState = HIGH;
        lastDebounceTime = 0;
        pinMode(buttonPin, INPUT_PULLUP);
    }

    bool click(){
        if(digitalRead(buttonPin) == HIGH)
        return false;
    }

    bool hold(){
        return false;
    }


private:
    int buttonPin;
    int buttonState;
    int lastButtonState;
    unsigned long buttonPressedTime = 0;
    bool isButtonHeld = false;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;
};