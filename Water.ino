#include "modules/WaterSensorClass.h"
#include "modules/WaterLedClass.h"
#include "Arduino.h"

#define WATERING_RELAIS 2

WaterSensor *waterSensor;
WaterLed *waterLed;
bool flooding;
bool flooding_aborted;

unsigned long WATER_INTERVAL = 10000; //kinda 2.5h
//unsigned long WATER_INTERVAL = 90; //test every 90 sec

unsigned int currentWaterCheckTime;
unsigned int currentFloodTime;
const unsigned int MILLI_2_SEC = 1000;

void setup() {
  Serial.begin(9600);//Initialization of Serial Port
  pinMode(WATERING_RELAIS, OUTPUT);
  waterLed = new WaterLed();
  waterSensor = new WaterSensor();
  flooding = false;
  flooding_aborted = false;
  currentWaterCheckTime = WATER_INTERVAL - 10; //start the first flooding 10 sec after the start
  currentFloodTime = 0;
  delay(1000);
}

const unsigned int CORE_LOOP = 1000;
void loop() {
  waterSensor->measure_water_level();
  setWaterLevelLed();
  delay(CORE_LOOP);
  checkWaterTime();
  if (flooding) {
    checkFloodStatus();
  }
}

void checkWaterTime() {
  currentWaterCheckTime += CORE_LOOP / MILLI_2_SEC;
  Serial.print(String(F("Next watering in ")));
  Serial.print(WATER_INTERVAL - currentWaterCheckTime);
  Serial.println(String(F("s")));
  Serial.println(currentWaterCheckTime);
  if (currentWaterCheckTime >= WATER_INTERVAL) { //Start flooding every x hours
    currentWaterCheckTime = 0;
    startFlooding();
  } else if (flooding_aborted) {
    if (!waterSensor->soonNotEnoughWater()) { //need some minimum water to start
      startFlooding();
    }
  } 
}

void startFlooding() {
  if (flooding == true) { return; }
  bool enoughWaterToFlood = waterSensor->enoughWaterToFlood();
  if (enoughWaterToFlood) {
    Serial.println(F("Start flooding"));
    flooding = true;
    flooding_aborted = false;
    currentFloodTime = 0;
    digitalWrite(WATERING_RELAIS, HIGH);   // turn the RELEAIS on (HIGH is the voltage level)
  } else {
    flooding_aborted = true;
    Serial.println(F("Waterlevel too low. Abort start flooding"));
  }
}

const unsigned int FLOOD_LENGTH = 30; //30 seconds of flooding
void checkFloodStatus(){
  bool enoughWaterToFlood = waterSensor->enoughWaterToFlood();
  currentFloodTime += CORE_LOOP / MILLI_2_SEC;
  if (!enoughWaterToFlood || currentFloodTime >= FLOOD_LENGTH) { //stop flooding after FLOOD_LENGTH
    stopFlooding();
  } else {
    Serial.print(String(F("Flooding for ")));
    Serial.print(FLOOD_LENGTH - currentFloodTime);
    Serial.println(String(F("s")));
  }
}

void stopFlooding() {
  Serial.println(F("Stop flooding"));
  flooding = false;
  digitalWrite(WATERING_RELAIS, LOW);   // turn the RELEAIS off
}

void setWaterLevelLed() {
  if (flooding) {
    waterLed->floodingWaterBlink();
  } else {
      if (waterSensor->tooMuchWater()) {
      waterLed->tooMuchWaterBlink();
    } else if (waterSensor->enoughWater()){
      if (waterSensor->soonNotEnoughWater()) {
        waterLed->soonTooLowWaterBlink();
      } else {
        waterLed->rightAmountOfWaterBlink();
      }
    } else {
      waterLed->tooLowWaterBlink();
    }
  } 
}