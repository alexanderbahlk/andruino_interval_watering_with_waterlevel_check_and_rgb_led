#include "SR04.h"
class WaterSensor {
    
    private:
        const long TRIG_PIN = 12;
        const long ECHO_PIN = 11;
        SR04 *sr04;
        const long MIN_WATER_LEVEL = 20;

        long lastMeasurement = false;

    public:
        WaterSensor() {
            sr04 = new SR04(ECHO_PIN,TRIG_PIN);
        }
        bool enoughWater() {
            return lastMeasurement;
        }

        void measure_water_level() {
            long cwl = sr04->Distance();
            Serial.print("Waterlevel: ");
            Serial.print(cwl);    //The difference between "Serial.print" and "Serial.println"
            Serial.println("cm"); //is that "Serial.println" can change lines.
            lastMeasurement = cwl >= MIN_WATER_LEVEL;;
        }
};
