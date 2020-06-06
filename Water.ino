#include "modules/WaterSensorClass.h"
#include "modules/MoistureSensorClass.h"
#include "Arduino.h"

#define WATERING_RELAIS 2
#define WATER_LEVEL_LED 4

WaterSensor *waterSensor;
MoistureSensor *moistureSensor;

void setup() {
  Serial.begin(9600);//Initialization of Serial Port
  pinMode(WATERING_RELAIS, OUTPUT);
  pinMode(WATER_LEVEL_LED, OUTPUT);
  waterSensor = new WaterSensor();
  moistureSensor = new MoistureSensor();
  delay(1000);
}

const unsigned int CORE_LOOP = 1000;
bool flooding = false;
void loop() {
  setWaterLevelLed();
  delay(CORE_LOOP); 
  if (flooding == true) {
    checkFloodStatus();
  } else {
    checkWaterTime();
  }
}

const long WATER_LOOP = 10000; //check every half hour
//const long WATER_LOOP = 1800000; //check every half hour

unsigned int currentWaterCheckTime = 0;

void checkWaterTime() {
  Serial.print(String(F("Next watering in ")));
  Serial.print((WATER_LOOP - currentWaterCheckTime) /1000);
  Serial.println(String(F("s")));
  if (currentWaterCheckTime >= WATER_LOOP) { //Start flooding every WATER_LOOP
    currentWaterCheckTime = 0;
    startFlooding();
  } else {
    currentWaterCheckTime += CORE_LOOP;
  }
}

unsigned int currentFloodTime = 0;
void startFlooding() {
  bool soilTooDry = moistureSensor->soilTooDry();
  bool enoughWater = waterSensor->enoughWater();
  if (soilTooDry && enoughWater) {
    Serial.println(F("Start flooding"));
    flooding = true;
    currentFloodTime = 0;
    digitalWrite(WATERING_RELAIS, HIGH);   // turn the RELEAIS on (HIGH is the voltage level)
  } else {
    Serial.println(F("Waterlevel too low or not dry enough!"));
  }
}

const unsigned int FLOOD_LENGTH = 5000; //5 seconds of flooding
void checkFloodStatus(){
  if (currentFloodTime >= FLOOD_LENGTH) { //stop flooding after FLOOD_LENGTH
    stopFlooding();
  } else {
    currentFloodTime += CORE_LOOP;
    performFlooding();
  }
}

void performFlooding() {
  bool enoughWater = waterSensor->enoughWater();
  if (enoughWater) {
    Serial.print(String(F("Flooding for ")));
    Serial.print((FLOOD_LENGTH - currentFloodTime) /1000);
    Serial.println(String(F("s")));
  } else {
    Serial.println(F("Waterlevel too low!"));
    stopFlooding();
  }
}

void stopFlooding() {
  Serial.println(F("Stop flooding"));
  flooding = false;
  digitalWrite(WATERING_RELAIS, LOW);   // turn the RELEAIS off
}

void setWaterLevelLed() {
  waterSensor->measure_water_level();
  if (waterSensor->tooMuchWater()) {
    digitalWrite(WATER_LEVEL_LED, waterSensor->tooMuchWaterLedBlinkState()); 
  } else if (waterSensor->enoughWater()) {
    digitalWrite(WATER_LEVEL_LED, LOW); 
  } else {
    digitalWrite(WATER_LEVEL_LED, HIGH); 
  }
}