#include "modules/WaterSensorClass.h"
#include "modules/MoistureSensorClass.h"

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
  waterSensor->measure_water_level();
  setWaterLevelLed();
  if (flooding == true) {
    checkFloodStatus();
  } else {
    checkWaterTime();
  }
  delay(CORE_LOOP); 
}

const long WATER_LOOP = 10000; //check every half hour

//const long WATER_LOOP = 1800000; //check every half hour
unsigned int currentWaterCheckTime = 0;

void checkWaterTime() {
  //Serial.println(WATER_LOOP);
  //Serial.println(currentWaterCheckTime);
  unsigned int waterCheckTimeLeft = (WATER_LOOP - currentWaterCheckTime) /1000;
  Serial.println(String("Next watering in ") + String(waterCheckTimeLeft) + String("s"));
  if (currentWaterCheckTime >= WATER_LOOP) { //Start flooding every WATER_LOOP
    startFlooding();
    currentWaterCheckTime = 0;
  } else {
    currentWaterCheckTime += CORE_LOOP;
  }
}

unsigned int currentFloodTime = 0;
void startFlooding() {
  bool soilTooDry = moistureSensor->soilTooDry();
  if (soilTooDry && waterSensor->enoughWater()) {
    Serial.println("Start flooding");
    flooding = true;
    currentFloodTime = 0;
    digitalWrite(WATERING_RELAIS, HIGH);   // turn the RELEAIS on (HIGH is the voltage level)
  } else {
    Serial.println("Waterlevel too low!");
  }
}

const unsigned int FLOOD_LENGTH = 5000; //5 seconds of flooding
void checkFloodStatus(){
  if (currentFloodTime >= FLOOD_LENGTH) { //stop flooding after FLOOD_LENGTH
    stopFlooding();
  } else {
    performFlooding();
  }
}

void performFlooding() {
  if (waterSensor->enoughWater()) {
    unsigned int floodingTimeLeft = (FLOOD_LENGTH - currentFloodTime) /1000;
    Serial.println(String("Flooding for ") + String(floodingTimeLeft) + String("s"));
    currentFloodTime += CORE_LOOP;
  } else {
    Serial.println("Waterlevel too low!");
    stopFlooding();
  }
}

void stopFlooding() {
  if (flooding == true) {
    Serial.println("Stop flooding");
    flooding = false;
    digitalWrite(WATERING_RELAIS, LOW);   // turn the RELEAIS off
  }
}

void setWaterLevelLed() {
  if (waterSensor->enoughWater()) {
    digitalWrite(WATER_LEVEL_LED, LOW); 
  } else {
    digitalWrite(WATER_LEVEL_LED, HIGH); 
  }
}