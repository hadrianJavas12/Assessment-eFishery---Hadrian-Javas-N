#include <Arduino.h>

//ADAPTER INFORMATION
String sensorName = "Intesitas Cahaya";  //nama sensor
String unit = "lux";    //satuan pengukuran sensor
double m = -2.94;   //kemiringan pada perasamaan linear  Y= m*x + C
double C = 2400.2;  //koefisien pada perasamaan linear  Y= m*x + C
#define sensorPin A7  // pin analog yang digunakan
long int lastTimeSent=0;
int deliveryIntervalTime = 100; // interval waktu mengirimkan data dalam milidetik

 // kalman filter
float err_measure = 10;
float err_estimate = 10;
float q = 0.01;

#include "Sensor.h"

Sensor analogSensor(sensorName,unit, m, C, err_measure, err_estimate, q);


#include <SoftwareSerial.h>
SoftwareSerial mySerial(11, 12); // RX, TX





void setup() {
  // put your setup code here, to run once:
 
  Serial.println(F("Initialization"));
  //inisialisasi program
  mySerial.begin(9600);
  Serial.begin(9600);
  analogSensor.SetRawValue(analogRead(sensorPin));

  Serial.println(F("______________________________________________"));
  Serial.println(F("Sensor Parameter"));
  Serial.println(analogSensor.GetSensorFullParam());
  Serial.println(F("______________________________________________"));
}

void loop() {
  //memperbaharui nilai sensor
  analogSensor.UpdateRawValue(analogRead(sensorPin));
  
  //mengirimkan nilai sensor information setiap interval waktu
  if(millis()-lastTimeSent>deliveryIntervalTime){
    String _sensorInfo = analogSensor.GetSensorInfo();
    Serial.println(_sensorInfo);
    mySerial.println(_sensorInfo);
    lastTimeSent = millis();
  }  
}