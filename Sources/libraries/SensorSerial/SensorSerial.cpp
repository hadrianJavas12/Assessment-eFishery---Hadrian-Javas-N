#include "SensorSerial.h"

SensorSerial::SensorSerial(uint8_t rxPin, int channelNo) : _serial(rxPin)
{
    _channel = channelNo;
    _serial.begin(9600);
}

void SensorSerial::CollectSensorData()
{
    _serial.listen();
    if (_serial.available())
    {
        _countSensorCheck = 0;
        _readComplete = false;
        while (_readComplete == false && _inputString.length() < 200)
        {
            char inChar = _serial.read();
            if (inChar == '\n')
            {
                _sensorInfo = _inputString;
                _inputString = "";
                _readComplete = true;
                break;
            }
            _inputString += inChar;
            delay(2);
        }
    }
    else
    {
        _countSensorCheck++;
        {
            if (_countSensorCheck > 50)
            {
                _sensorInfo = "sensor is not connected";
                _inputString = "";
            }
        }
    }
}

String SensorSerial::GetSensorInfo()
{
    return _sensorInfo;
}

int SensorSerial::GetChannelNumber()
{
    return _channel;
}