#ifndef SENSORSERIAL_H
#define SENSORSERIAL_H
#include <Arduino.h>

#include <SoftwareSerial.h>

class SensorSerial
{
private:
  SoftwareSerial _serial;
  String _sensorInfo;
  String _inputString;
  bool _readComplete = false;
  int _countSensorCheck;
  int _channel;

public:
  
  //kosntruktor untuk set pin receive dan nomor channel
  SensorSerial(uint8_t rxPin, int channelNo);

  //untuk mengambil data dari serial komunikasi 
  void CollectSensorData();

  //untuk mendapatan informasi data sensor
  String GetSensorInfo();

  // untuk mendapatkan informasi nomor channel
  int GetChannelNumber();
};


#endif