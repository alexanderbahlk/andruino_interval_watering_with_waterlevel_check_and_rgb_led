#include "SR04.h"
#include "Arduino.h"
class WaterSensor {
    
    private:
        const long TRIG_PIN = 12;
        const long ECHO_PIN = 11;
        SR04 *sr04;
        const long BOTTOM_WATER_LEVEL = 16;
        const long TOP_WATER_LEVEL = 5;

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
            Serial.println(_tooMuchWaterLedBlinkState);
            return _tooMuchWaterLedBlinkState == true ? HIGH : LOW;
        }

        void measure_water_level() {
            waterLevel = sr04->Distance();
            Serial.print("Waterlevel: ");
            Serial.print(waterLevel);    //The difference between "Serial.print" and "Serial.println"
            Serial.println("cm"); //is that "Serial.println" can change lines.
        }
};
