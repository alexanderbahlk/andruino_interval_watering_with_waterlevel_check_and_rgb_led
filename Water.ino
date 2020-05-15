//www.elegoo.com
//2016.12.08
#include "SR04.h"

#define TRIG_PIN 12
#define ECHO_PIN 11
#define WATERING_LED 4
#define WATERING_RELAIS 7  

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

void setup() {
   Serial.begin(9600);//Initialization of Serial Port
   pinMode(WATERING_LED, OUTPUT);
   pinMode(WATERING_RELAIS, OUTPUT);
   delay(1000);
}

const int CORE_LOOP = 1000;
bool flooding = false;
void loop() {
   if (flooding == true) {
    check_flood_status();
   } else {
    check_water_time();
   }
   delay(CORE_LOOP);
}

const int WATER_LOOP = 10000; //every 10 seconds try to water (ToDo Should be 30 minutes or so)
int current_water_check_time = 0;

void check_water_time() {
  int water_check_time_left = (WATER_LOOP - current_water_check_time) /1000;
  Serial.println(String("Next watering in ") + String(water_check_time_left) + String("s"));
  if (current_water_check_time >= WATER_LOOP) { //Start flooding every WATER_LOOP
    start_flooding();
    current_water_check_time = 0;
  } else {
    current_water_check_time += CORE_LOOP;
  }
}

int current_flood_time = 0;
void start_flooding() {
  if (can_flood()) {
    Serial.println("Start flooding");
    flooding = true;
    current_flood_time = 0;
    digitalWrite(WATERING_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(WATERING_RELAIS, HIGH);   // turn the RELEAIS on (HIGH is the voltage level)
  } else {
    Serial.println("Waterlevel too low!");
  }
}

const int FLOOD_LENGTH = 5000; //5 seconds of flooding
void check_flood_status(){
  if (current_flood_time >= FLOOD_LENGTH) { //stop flooding after FLOOD_LENGTH
    stop_flooding();
  } else {
    perform_flooding();
  }
}

void perform_flooding() {
  if (can_flood()) {
    int flooding_time_left = (FLOOD_LENGTH - current_flood_time) /1000;
    Serial.println(String("Flooding for ") + String(flooding_time_left) + String("s"));
    current_flood_time += CORE_LOOP;
  } else {
    Serial.println("Waterlevel too low!");
    stop_flooding();
  }
}

void stop_flooding() {
  if (flooding == true) {
    Serial.println("Stop flooding");
    flooding = false;
    digitalWrite(WATERING_LED, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(WATERING_RELAIS, LOW);   // turn the RELEAIS off
  } 
}

const long MIN_WATER_LEVEL = 20;
bool can_flood() {
  long current_water_level = measure_water_level();
  return current_water_level >= MIN_WATER_LEVEL;
}

long measure_water_level() {
  long cwl = sr04.Distance();
  Serial.print(cwl);    //The difference between "Serial.print" and "Serial.println" 
  Serial.println("cm"); //is that "Serial.println" can change lines.                  
  return cwl;
}
