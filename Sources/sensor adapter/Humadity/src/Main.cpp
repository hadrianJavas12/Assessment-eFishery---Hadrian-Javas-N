#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

//ADAPTER INFORMATION
String sensorName = "Kelembaban";  //nama sensor
String unit = "%";    //satuan pengukuran sensor
double m = 1;   //kemiringan pada perasamaan linear  Y= m*x + C
double C = 0;  //koefisien pada perasamaan linear  Y= m*x + C
 // kalman filter
float err_measure = 10;
float err_estimate = 10;
float q = 0.01;

#define sensorPin A3  // pin digital yang digunakan
int deliveryIntervalTime = 100; // interval waktu mengirimkan data dalam milidetik



#include "Sensor.h"

Sensor dhtSensor(sensorName,unit, m, C, err_measure,err_estimate, q);

#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); // RX, TX

void taskSendSensorData(void *pvParameters);

// untuk mendefinisikan object dht11
#include <DHT11.h>
DHT11 dht11(A3);


void setup() {
  Serial.println(F("Initialization"));
  //inisialisasi program
  mySerial.begin(9600);
  Serial.begin(9600);
  dhtSensor.SetRawValue(dht11.readHumidity());
    xTaskCreate(
    taskSendSensorData
    ,  "taskSendSensorData"  
    ,  128 
    ,  NULL
    ,  1  
    ,  NULL );
  Serial.println(F("______________________________________________"));
  Serial.println(F("Sensor Parameter"));
  Serial.println(dhtSensor.GetSensorFullParam());
  Serial.println(F("______________________________________________"));
}

void loop() {
  //memperbaharui nilai sensor
  dhtSensor.UpdateRawValue(dht11.readHumidity());
}

//scheduler untuk mengirim data setiap <deliveryIntervalTime> milli detik
void taskSendSensorData(void *pvParameters) 
{
  (void) pvParameters;
  for (;;)
  {
    mySerial.println(dhtSensor.GetSensorInfo());
    vTaskDelay(deliveryIntervalTime/portTICK_PERIOD_MS); 
  }
}