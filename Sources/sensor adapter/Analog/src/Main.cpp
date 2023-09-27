#include <Arduino.h>

//ADAPTER INFORMATION
String sensorName = "Intesitas Cahaya";
String unit = "lux";
double m = -2.94;
double C = 2400.2;
#define sensorPin A7


 // kalman filter
float err_measure = 10;
float err_estimate = 10;
float q = 0.01;

#include "Sensor.h"

Sensor analogSensor(sensorName,unit, m, C, err_measure, err_estimate, q);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Initialization"));
  //inisialisasi program
  
  Serial.println(F("______________________________________________"));
  Serial.println(F("Sensor Parameter"));
  Serial.println(analogSensor.GetSensorFullParam());
  Serial.println(F("______________________________________________"));
}

void loop() {
  analogSensor.UpdateRawValue(analogRead(sensorPin));
  Serial.println(analogSensor.GetSensorInfo());
}