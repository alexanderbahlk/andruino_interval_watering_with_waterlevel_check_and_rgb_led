class MoistureSensor {
    private:
        #define WATER_MOISTURE_SENSOR A5
        const int dry = 827;
        const int wet = 460;
        const int idealMoisture = 50;

    public:
        MoistureSensor() {
        }
        bool soilTooDry() {
            int sensorVal = analogRead(WATER_MOISTURE_SENSOR);
            Serial.print("Moisture val: ");
            Serial.println(sensorVal);
            int percentageHumidity = map(sensorVal, wet, dry, 100, 0);
            Serial.print("Humidity: ");
            Serial.print(percentageHumidity);
            Serial.println("%");
            bool tooDry = percentageHumidity < idealMoisture;
            Serial.print("Should water: ");
            Serial.println(tooDry);
            return tooDry;
        }
};
