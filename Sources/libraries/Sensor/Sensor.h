#ifndef SENSOR_H
#define SENSOR_H
#include <Arduino.h>
class Sensor {
  private:
    //parameter sensor
    String _sensorName = "Sensor";
    String _unit;
    double _rawValue;
    double _m = 1;
    double _C = 0;
    String Info;

    // kalman filter
    float _err_measure = 3;
    float _err_estimate = 3;
    float _q = 0.01;
    float _current_estimate;
    float _last_estimate;
    float _kalman_gain;

    //untuk memfilter data mentah sensor agar tidak noise
    float UpdateEstimate(float mea);

    //untuk menghitung nilai terkalibrasi dengan satuan yang diharapkan
    void calibrate();

  public:
    double Value;
    
    //konstruktor basic sensor information
    Sensor(String sensorName, String unit);

    //konstruktor informasi sensor + parameter kalibrasi
    Sensor(String sensorName, String unit, double mValue, double CValue);
    
    //konstruktor informasi sensor + parameter kalibrasi + parameter filer
    Sensor(String sensorName, String unit, double mValue, double CValue,float mea_e, float est_e, float q);
    
    //set parameter informasi sensor
    void SetSensorParam(String sensorName, String unit);
    
    //set parameter kalibrasi menggunakan linear regresi
    void SetCalibrationParam(double mValue, double CValue);

    //untuk mendapatkan nilai raw yang sudah terfilter
    double GetRawValue();

    //assign nilai baru
    void SetRawValue(double newRawValue);

    //update nilai baru
    void UpdateRawValue(double newRawValue);

    //set parameter filter
    void SetFilter(float mea_e, float est_e, float q);

    //untuk mendapatkan informasi sensor
    String GetSensorInfo();
    
    //untuk mendapatkan full paramater sensor
    String GetSensorFullParam();

};

#endif