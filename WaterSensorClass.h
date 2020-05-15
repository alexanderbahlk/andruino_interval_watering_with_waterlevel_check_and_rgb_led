#include "SR04.h"
class WaterSensor {
    
    private:
        const long TRIG_PIN = 12;
        const long ECHO_PIN = 11;
        SR04 *sr04;
        const long MIN_WATER_LEVEL = 20;

    public:
        WaterSensor() {
            sr04 = new SR04(ECHO_PIN,TRIG_PIN);
        }
        bool can_flood() {
            long current_water_level = measure_water_level();
            return current_water_level >= MIN_WATER_LEVEL;
        }

        long measure_water_level() {
            long cwl = sr04->Distance();
            Serial.print(cwl);    //The difference between "Serial.print" and "Serial.println"
            Serial.println("cm"); //is that "Serial.println" can change lines.
            return cwl;
        }
};
