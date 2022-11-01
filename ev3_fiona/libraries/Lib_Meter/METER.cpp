#include "METER.H"

MODBUS modbus;

void METER::init(int8_t slave, Stream &serial)
{
  _serial = &serial;
  _slave = slave;
  modbus.init(_slave, *_serial);
}

bool METER::readParameter1Phase(float &voltage, float &current, float &power)
{
  _return = false;
  memset(_buffers, 0, sizeof(_buffers));
  if (modbus.readMultipleParameter(0x0000, 14, _buffers))
  {
    _value = _buffers[3] << 24 | _buffers[4] << 16 | _buffers[5] << 8 | _buffers[6];
    _result = (*(float *)&_value);
    // voltage = String(_result);
    voltage = _result;
    // voltage = String(_buffers[3],HEX);

    _value = _buffers[15] << 24 | _buffers[16] << 16 | _buffers[17] << 8 | _buffers[18];
    _result = (*(float *)&_value);
    current = _result;
    // current = String(_result);

    _value = _buffers[27] << 24 | _buffers[28] << 16 | _buffers[29] << 8 | _buffers[30];
    _result = (*(float *)&_value);
    // power = String(_result);
    power = _result / 1000;
    _return = true;
  }

  return _return;
}

bool METER::readEnergy1Phase(float &energy)
{
  _return = false;
  memset(_buffers, 0, sizeof(_buffers));
  if (modbus.readMultipleParameter(0x0156, 2, _buffers))
  {
    _value = _buffers[3] << 24 | _buffers[4] << 16 | _buffers[5] << 8 | _buffers[6];
    _result = (*(float *)&_value);
    // energy = String(_result);
    energy = _result;
    _return = true;
  }
  return _return;
}

bool METER::readParameter3Phase(float &voltageA, float &voltageB, float &voltageC, float &currentA, float &currentB, float &currentC, float &powerA, float &powerB, float &powerC)
{
  // bool METER::readParameter3Phase(float *voltage, float *current, float *power){
  _return = false;
  memset(_buffers, 0, sizeof(_buffers));
  if (modbus.readMultipleParameter(0x0000, 18, _buffers))
  {
    _value = _buffers[3] << 24 | _buffers[4] << 16 | _buffers[5] << 8 | _buffers[6];
    _result = (*(float *)&_value);
    // voltage[0] = String(_result);
    voltageA = _result;

    _value = _buffers[7] << 24 | _buffers[8] << 16 | _buffers[9] << 8 | _buffers[10];
    _result = (*(float *)&_value);
    // voltage[1] = String(_result);
    voltageB = _result;

    _value = _buffers[11] << 24 | _buffers[12] << 16 | _buffers[13] << 8 | _buffers[14];
    _result = (*(float *)&_value);
    // voltage[2] = String(_result);
    voltageC = _result;

    _value = _buffers[15] << 24 | _buffers[16] << 16 | _buffers[17] << 8 | _buffers[18];
    _result = (*(float *)&_value);
    // current[0] = String(_result);
    currentA = _result;

    _value = _buffers[19] << 24 | _buffers[20] << 16 | _buffers[21] << 8 | _buffers[22];
    _result = (*(float *)&_value);
    // current[1] = String(_result);
    currentB = _result;

    _value = _buffers[23] << 24 | _buffers[24] << 16 | _buffers[25] << 8 | _buffers[26];
    _result = (*(float *)&_value);
    // current[2] = String(_result);
    currentC = _result;

    _value = _buffers[27] << 24 | _buffers[28] << 16 | _buffers[29] << 8 | _buffers[30];
    _result = (*(float *)&_value);
    // power[0] = String(_result);
    powerA = _result / 1000;

    _value = _buffers[31] << 24 | _buffers[32] << 16 | _buffers[33] << 8 | _buffers[34];
    _result = (*(float *)&_value);
    // power[1] = String(_result);
    powerB = _result / 1000;

    _value = _buffers[35] << 24 | _buffers[36] << 16 | _buffers[37] << 8 | _buffers[38];
    _result = (*(float *)&_value);
    // power[2] = String(_result);
    powerC = _result / 1000;

    _return = true;
  }
  return _return;
}

bool METER::readParameter3Phase(float &voltageAVG, float &currentA, float &currentB, float &currentC, float &powerTotal, float &energy)
{
  // bool METER::readParameter3Phase(float *voltage, float *current, float *power){
  float voltageA, voltageB, voltageC;
  _return = false;
  memset(_buffers, 0, sizeof(_buffers));
  if (modbus.readMultipleParameter(0x0000, 18, _buffers))
  {
    _value = _buffers[3] << 24 | _buffers[4] << 16 | _buffers[5] << 8 | _buffers[6];
    _result = (*(float *)&_value);
    // voltage[0] = String(_result);
    voltageA = _result;

    _value = _buffers[7] << 24 | _buffers[8] << 16 | _buffers[9] << 8 | _buffers[10];
    _result = (*(float *)&_value);
    // voltage[1] = String(_result);
    voltageB = _result;

    _value = _buffers[11] << 24 | _buffers[12] << 16 | _buffers[13] << 8 | _buffers[14];
    _result = (*(float *)&_value);
    // voltage[2] = String(_result);
    voltageC = _result;
	
	voltageAVG = (voltageA + voltageB + voltageC);
	
    _value = _buffers[15] << 24 | _buffers[16] << 16 | _buffers[17] << 8 | _buffers[18];
    _result = (*(float *)&_value);
    // current[0] = String(_result);
    currentA = _result;

    _value = _buffers[19] << 24 | _buffers[20] << 16 | _buffers[21] << 8 | _buffers[22];
    _result = (*(float *)&_value);
    // current[1] = String(_result);
    currentB = _result;

    _value = _buffers[23] << 24 | _buffers[24] << 16 | _buffers[25] << 8 | _buffers[26];
    _result = (*(float *)&_value);
    // current[2] = String(_result);
    currentC = _result;

    _value = _buffers[27] << 24 | _buffers[28] << 16 | _buffers[29] << 8 | _buffers[30];
    _result = (*(float *)&_value);
    // power[0] = String(_result);
    powerTotal = _result / 1000;

    _value = _buffers[31] << 24 | _buffers[32] << 16 | _buffers[33] << 8 | _buffers[34];
    _result = (*(float *)&_value);
    // power[1] = String(_result);
    powerTotal += _result / 1000;

    _value = _buffers[35] << 24 | _buffers[36] << 16 | _buffers[37] << 8 | _buffers[38];
    _result = (*(float *)&_value);
    // power[2] = String(_result);
    powerTotal += _result / 1000;

    _return = true;
  }
  return _return;
}

bool METER::readEnergy3Phase(float &energy)
{
  _return = false;
  memset(_buffers, 0, sizeof(_buffers));
  if (modbus.readMultipleParameter(0x0156, 2, _buffers))
  {
    _value = _buffers[3] << 24 | _buffers[4] << 16 | _buffers[5] << 8 | _buffers[6];
    _result = (*(float *)&_value);
    // energy = String(_result);
    energy = _result;
    _return = true;
  }
  return _return;
}

