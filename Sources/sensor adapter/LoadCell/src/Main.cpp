#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

//ADAPTER INFORMATION
String sensorName = "Berat";  //nama sensor
String unit = "kg";    //satuan pengukuran sensor
double m = -0.000134051711535367;   //kemiringan pada perasamaan linear  Y= m*x + C
double C = 38.5357053356453;  //koefisien pada perasamaan linear  Y= m*x + C
 // kalman filter
float err_measure = 10;
float err_estimate = 10;
float q = 0.01;

int deliveryIntervalTime = 100; // interval waktu mengirimkan data dalam milidetik


#include "Sensor.h"

Sensor loadCell(sensorName,unit, m, C, err_measure,err_estimate, q);

#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); // RX, TX

void taskSendSensorData(void *pvParameters);


#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A3;
const int LOADCELL_SCK_PIN = A4;

// untuk mendefinisikan object hx711
HX711 scale;


void setup() {
  Serial.println(F("Initialization"));
  //inisialisasi program
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  mySerial.begin(9600);
  Serial.begin(9600);
  loadCell.SetRawValue(scale.read());
    xTaskCreate(
    taskSendSensorData
    ,  "taskSendSensorData"  
    ,  128 
    ,  NULL
    ,  1  
    ,  NULL );
  Serial.println(F("______________________________________________"));
  Serial.println(F("Sensor Parameter"));
  Serial.println(loadCell.GetSensorFullParam());
  Serial.println(F("______________________________________________"));
}

void loop() {
  //memperbaharui nilai sensor
  loadCell.UpdateRawValue(scale.read());
}

//scheduler untuk mengirim data setiap <deliveryIntervalTime> milli detik
void taskSendSensorData(void *pvParameters) 
{
  (void) pvParameters;
  for (;;)
  {
    mySerial.println(loadCell.GetSensorInfo());
    vTaskDelay(deliveryIntervalTime/portTICK_PERIOD_MS); 
  }
}