#include "Arduino.h"

class MoistureSensor {
    private:
        #define WATER_MOISTURE_SENSOR A4
        const unsigned int dry = 832;
        const unsigned int wet = 456;
        const unsigned int idealMoisture = 50;

    public:
        MoistureSensor() {
        }
        bool soilTooDry() {
            int sensorVal = analogRead(WATER_MOISTURE_SENSOR);
            Serial.print(F("Moisture val: "));
            Serial.println(sensorVal);
            int percentageHumidity = map(sensorVal, wet, dry, 100, 0);
            Serial.print(F("Humidity: "));
            Serial.print(percentageHumidity);
            Serial.println(F("%"));
            bool tooDry = percentageHumidity < idealMoisture;
            Serial.print(F("Should water: "));
            Serial.println(tooDry);
            return tooDry;
        }
};
