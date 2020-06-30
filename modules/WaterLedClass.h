#include "Arduino.h"
class WaterLed {
    #define WATER_LEVEL_LED_BLUE 3
    #define WATER_LEVEL_LED_GREEN 5
    #define WATER_LEVEL_LED_RED 6
    
    private:
        bool _blinkState = false;

        int blinkState() {
            _blinkState = !_blinkState;
            return _blinkState;
        }

    public:
        WaterLed() {
            pinMode(WATER_LEVEL_LED_BLUE, OUTPUT);
            pinMode(WATER_LEVEL_LED_GREEN, OUTPUT);
            pinMode(WATER_LEVEL_LED_RED, OUTPUT);
        }

        void tooLowWaterBlink() {
            Serial.println(String(F("tooLowWaterBlink")));
            digitalWrite(WATER_LEVEL_LED_RED, HIGH); 
            digitalWrite(WATER_LEVEL_LED_GREEN, LOW);
            digitalWrite(WATER_LEVEL_LED_BLUE, LOW);
        }

        void rightAmountOfWaterBlink() {
            Serial.println(String(F("rightAmountOfWaterBlink")));
            digitalWrite(WATER_LEVEL_LED_BLUE, LOW);
            digitalWrite(WATER_LEVEL_LED_GREEN, LOW);
            digitalWrite(WATER_LEVEL_LED_RED, LOW);
        }

        void soonTooLowWaterBlink() {
            Serial.println(String(F("soonTooLowWaterBlink")));
            if (blinkState()) {
                digitalWrite(WATER_LEVEL_LED_RED, HIGH); 
                digitalWrite(WATER_LEVEL_LED_GREEN, LOW);
                digitalWrite(WATER_LEVEL_LED_BLUE, LOW);
            } else {
                digitalWrite(WATER_LEVEL_LED_RED, LOW);
                digitalWrite(WATER_LEVEL_LED_GREEN, LOW);
                digitalWrite(WATER_LEVEL_LED_BLUE, LOW);
            }
        }

        void tooMuchWaterBlink() {
            Serial.println(String(F("tooMuchWaterBlink")));
            if (blinkState()) {
                analogWrite(WATER_LEVEL_LED_RED, 255); 
                analogWrite(WATER_LEVEL_LED_GREEN, 85);
                digitalWrite(WATER_LEVEL_LED_BLUE, LOW);
            } else {
                digitalWrite(WATER_LEVEL_LED_RED, LOW);
                digitalWrite(WATER_LEVEL_LED_GREEN, LOW);
                digitalWrite(WATER_LEVEL_LED_BLUE, LOW);
            }
        }

        void floodingWaterBlink() {
            Serial.println(String(F("floodingWaterBlink")));
            if (blinkState()) {
                digitalWrite(WATER_LEVEL_LED_RED, LOW); 
                digitalWrite(WATER_LEVEL_LED_GREEN, LOW);
                digitalWrite(WATER_LEVEL_LED_BLUE, HIGH);
            } else {
                digitalWrite(WATER_LEVEL_LED_RED, LOW);
                digitalWrite(WATER_LEVEL_LED_GREEN, LOW);
                digitalWrite(WATER_LEVEL_LED_BLUE, LOW);
            }
        }
        
};
