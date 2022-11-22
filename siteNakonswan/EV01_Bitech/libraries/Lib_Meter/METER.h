#ifndef METER_H
#define METER_H
#include <MODBUS.h>

class METER
{
public:
  void init(int8_t slave, Stream &serial);
  bool readParameter1Phase(float &voltage, float &current, float &power);
  bool readParameter3Phase(float  &voltageA, float  &voltageB, float &voltageC, float &currentA, float &currentB, float &currentC, float &powerA, float &powerB, float &powerC);
  bool readParameter3Phase(float &voltageAVG, float &currentA, float &currentB, float &currentC, float &powerTotal, float &energy);
  bool readEnergy1Phase(float &energy);
  bool readEnergy3Phase(float &energy);

private:
  bool _return;
  uint8_t _buffers[100];
  uint32_t _value;
  uint8_t _slave;
  Stream *_serial;
  float _result;
};
#endif
