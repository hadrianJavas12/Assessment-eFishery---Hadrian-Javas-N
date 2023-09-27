#include "Sensor.h"

//konstruktor basic sensor information
Sensor::Sensor(String sensorName, String unit)
{
  SetSensorParam(sensorName, unit);
}

//konstruktor informasi sensor + parameter kalibrasi
Sensor::Sensor(String sensorName, String unit, double mValue, double CValue) : Sensor(sensorName, unit)
{
  SetCalibrationParam(mValue, CValue);
}

//konstruktor informasi sensor + parameter kalibrasi + parameter filer
Sensor::Sensor(String sensorName, String unit, double mValue, double CValue,float mea_e, float est_e, float q) : Sensor(sensorName, unit, mValue, CValue) 
{
  SetFilter(mea_e, est_e, q);
}

//untuk memfilter data mentah sensor agar tidak noise
float Sensor::UpdateEstimate(float mea)
{
  _kalman_gain = _err_estimate/(_err_estimate + _err_measure);
  _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
  _err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
  _last_estimate=_current_estimate;

  return _current_estimate;
}

//untuk menghitung nilai terkalibrasi dengan satuan yang diharapkan
void Sensor::calibrate() 
{
  Value = (_rawValue * _m) + _C;
}

//set parameter informasi sensor
void Sensor::SetSensorParam(String sensorName, String unit)
{
  _sensorName = sensorName;
  _unit = unit;  
}

//set parameter kalibrasi menggunakan linear regresi
void Sensor::SetCalibrationParam(double mValue, double CValue)
{
  _m = mValue;
  _C = CValue;
}

//untuk mendapatkan nilai raw yang sudah terfilter
double Sensor::GetRawValue() 
{
  return _rawValue;
}

//assign nilai baru
void Sensor::SetRawValue(double newRawValue) 
{
  _rawValue = newRawValue;
  calibrate();
}

//update nilai baru
void Sensor::UpdateRawValue(double newRawValue) 
{
  _rawValue = UpdateEstimate(newRawValue);
  calibrate();
}

//set parameter filter
void Sensor::SetFilter(float mea_e, float est_e, float q)
{
  _err_measure=mea_e;
  _err_estimate=est_e;
  _q = q;
}

//untuk mendapatkan informasi sensor
String Sensor::GetSensorInfo()
{
  return _sensorName + ": " + String(Value) + " " + _unit;
}

//untuk mendapatkan full paramater sensor
String Sensor::GetSensorFullParam()
{
  String fullParam;

  fullParam = "Sensor Name: " + _sensorName + "\n"; 
  fullParam += "Sensor Unit: " + _unit + "\n";
  fullParam += "Calibration Parameter (m): " + String(_m) + "\n";
  fullParam += "Calibration Parameter (C): " + String(_C) + "\n";
  fullParam += "Filter Parameter (mea_e): " + String(_err_measure) + "\n";
  fullParam += "Filter Parameter (est_e): " + String(_err_estimate) + "\n";
  fullParam += "Filter Parameter (q): " + String(_q) + "\n";
  fullParam += "Filtered Raw Value: " + String(_rawValue) + "\n";
  fullParam += "Sensor Value: " + String(Value) + "\n";
  
  return fullParam;
}