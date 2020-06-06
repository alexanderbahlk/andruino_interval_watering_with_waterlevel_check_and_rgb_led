#include "SR04.h"
#include "Arduino.h"
class WaterSensor {
    
    private:
        const unsigned int TRIG_PIN = 12;
        const unsigned int ECHO_PIN = 11;
        SR04 *sr04;
        const unsigned int BOTTOM_WATER_LEVEL = 16;
        const unsigned int TOP_WATER_LEVEL = 5;

        long waterLevel = 0;
        bool _tooMuchWaterLedBlinkState = false;

    public:
        WaterSensor() {
            sr04 = new SR04(ECHO_PIN,TRIG_PIN);
        }
        bool enoughWater() {
            return waterLevel <= BOTTOM_WATER_LEVEL;
        }

        bool tooMuchWater() {
            return waterLevel < TOP_WATER_LEVEL;
        }

        int tooMuchWaterLedBlinkState() {
            _tooMuchWaterLedBlinkState = !_tooMuchWaterLedBlinkState;
            return _tooMuchWaterLedBlinkState == true ? HIGH : LOW;
        }

        void measure_water_level() {
            waterLevel = sr04->Distance();
            Serial.print(F("Waterlevel: "));
            Serial.print(waterLevel);    //The difference between "Serial.print" and "Serial.println"
            Serial.println(F("cm")); //is that "Serial.println" can change lines.
        }
};
