#include "SR04.h"
#include "Arduino.h"
class WaterSensor {
    #define TRIG_PIN 8
    #define ECHO_PIN 7
    
    private:
        SR04 *sr04;
        const unsigned int WARN_WATER_LEVEL = 14;
        const unsigned int MIN_WATER_LEVEL = 16;
        const unsigned int MAX_WATER_LEVEL = 5;

        long waterLevel = 0;

    public:
        WaterSensor() {
            pinMode(TRIG_PIN, OUTPUT);
            pinMode(ECHO_PIN, INPUT);
            sr04 = new SR04(ECHO_PIN,TRIG_PIN);
        }

        bool enoughWaterToFlood() {
            return enoughWater();
        }

        bool enoughWater() {
            return waterLevel <= MIN_WATER_LEVEL; //16 cm and higer
        }

        bool soonNotEnoughWater() {
            return waterLevel >= WARN_WATER_LEVEL; //14 cm and lower
        }

        bool tooMuchWater() {
            return waterLevel < MAX_WATER_LEVEL;
        }

        void measure_water_level() {
            waterLevel = sr04->Distance();
            //waterLevel = 17;
            Serial.print(F("Waterlevel: "));
            Serial.print(waterLevel);    //The difference between "Serial.print" and "Serial.println"
            Serial.println(F("cm")); //is that "Serial.println" can change lines.
        }
};
