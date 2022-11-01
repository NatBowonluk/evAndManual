#include "EVController.h"

MODBUS modbusev;
void EVCONTROLLER::init(int8_t slave, Stream &serial)
{
  _serial = &serial;
  _slave = slave;
  modbusev.init(_slave, *_serial);
}

bool EVCONTROLLER::getSerialNumber(String &serialNumber)
{
  _return = false;
  serialNumber = "";
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x03FD, 6, _buffer))
  {
	  for(int i = 3; i <= 14; i++)
	    if(_buffer[i] != 0x20)
			serialNumber += String(_buffer[i]-0x30);	
	_return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getConfigSwitch(String &sw1, String &sw2)
{
  _return = false;
  sw1 = "";
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x07D0, 2, _buffer))
  {
    for(int i = 0; i <= 30; i++)
    {
      sw1 += String(_buffer[i], HEX) + " ";
    }
	
	_return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getSettingMaxCurrentPermissible(uint8_t &maxCurrentPermissible)
{
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x55F0, 1, _buffer))
  {
	  maxCurrentPermissible = _buffer[4];
	_return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::setSettingMaxCurrentPermissible(uint8_t maxCurrentPermissible)
{
  _return = false;
  if(modbusev.setfn06(0x55F0, maxCurrentPermissible, _buffer))
  {
    if(maxCurrentPermissible == _buffer[5])
      _return = true;
  } 
  return _return;
}

bool EVCONTROLLER::getSettingMaxCurrent(uint8_t &maxCurrent)
{
  _return = false;
   memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x07D0, 1, _buffer))
  {
    _swDip5 = (_buffer[4]>>4) & 0x01;
    _swDip6 = (_buffer[4]>>5) & 0x01;

    _swDip56 = _swDip5<<1 | _swDip6;
    
    switch(_swDip56){
      case 0b00000000 :
          maxCurrent = 16;
          break;
      case 0b00000001:
          maxCurrent = 20;
          break;
      case 0b00000010:
          maxCurrent = 32;
          break;
      case 0b00000011:
          maxCurrent = 63;
          break; 
    }
    _return = true;
  }
  return _return;
}

bool EVCONTROLLER::getEn(uint16_t &en)
{
  _return = false; 
   memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x0FA0, 1, _buffer))
  {
    en = _buffer[4];
    _return = true;
  }
  return _return;
}

bool EVCONTROLLER::setEn(uint16_t en)
{
  _return = false;
  if(modbusev.setfn06(0x0FA0, en, _buffer))
  {
    if((en>>8) == _buffer[4] && (en&0x00ff) == _buffer[5])
      _return = true;
  } 
  return _return;
}

bool EVCONTROLLER::getCoil20000(String &value){
  _return = false;
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readSingleCoil(0x4E20, _buffer))
  {
    if(_buffer[3] == 0x00)
      value = "OFF";
    else if(_buffer[3] == 0x01)
      value = "ON"; 
          
    _return = true;
  }
  return _return;
}

bool EVCONTROLLER::getCoil20000(byte &value){
  _return = false;
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readSingleCoil(0x4E20, _buffer))
  {
	value = _buffer[3];        
    _return = true;
  }
  return _return;
}

bool EVCONTROLLER::setCoil20000(String value)
{
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(value == "ON")
    _valueSingleCoil = 0xFF00;
  else if(value == "OFF")
    _valueSingleCoil = 0x0000;
    
  if(modbusev.writeSingleCoil(0x4E20,_valueSingleCoil ,_buffer))
  { 
    if(_valueSingleCoil == (_buffer[4]<<8 | _buffer[5]))
    {
      _return = true;
    }
        //_return = true;
  }
  return _return;
}

bool EVCONTROLLER::restartEVController()
{
  _return = false;
  
  if(modbusev.writeSingleCoil(0x5208,0xFF00 ,_buffer))
  { 
    if(0xFF00 == (_buffer[4]<<8 | _buffer[5]))
    {
      _return = true;
    }
  }
  return _return;
}	

bool EVCONTROLLER::getAllStatus(String &vehicleStatus, uint16_t &charginCurrentCP, uint8_t &statusInputs, uint8_t &statusOutputs,uint16_t &timeConnectHH, uint8_t &timeConnectMM, uint8_t &timeConnectSS 
      ,uint16_t &timeChargingHH , uint8_t &timeChargingMM, uint8_t &timeChargingSS , uint16_t &errorStatus){
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  
  if(modbusev.readMultipleParameter(0x5DC0, 26, _buffer))
  {
//    for(int i = 0; i < 59; i++)
//      rawData += String(_buffer[i], HEX) + " ";

     // vehicleStatus
     _valueStatus = _buffer[3]<<8 | _buffer[4];
     switch(_valueStatus)
     {
      case 0x4131 :
        vehicleStatus = "A1";
        break;
      case 0x4132 :
        vehicleStatus = "A2";
        break;
      case 0x4231 :
        vehicleStatus = "B1";
        break;
      case 0x4232 :
        vehicleStatus = "B2";
        break;
      case 0x4331 :
        vehicleStatus = "C1";
        break;
      case 0x4332 :
        vehicleStatus = "C2";
        break;
	  case 0x4431 :
        vehicleStatus = "D1";
        break;
	  case 0x4432 :
        vehicleStatus = "D2";
        break;
	  case 0x4530 :
        vehicleStatus = "E0";
        break;
	  case 0x4531 :
        vehicleStatus = "E1";
        break;
	  case 0x4630 :
        vehicleStatus = "F0";
        break;
	  case 0x4631 :
        vehicleStatus = "F1";
        break;
     }

     // Charging current via signal CP in A
     charginCurrentCP = _buffer[10];

     // Status input 
     statusInputs = _buffer[12];
     // Status output
     statusOutputs = _buffer[14];

     // Time connect state B, C, D
     timeConnectHH = _buffer[43]<<8 | _buffer[44];
     timeConnectMM = _buffer[41];
     timeConnectSS = _buffer[42];

     // Time connect state C, D
     timeChargingHH = _buffer[39]<<8 | _buffer[40];
     timeChargingMM = _buffer[37];
     timeChargingSS = _buffer[38];

     errorStatus = _buffer[53]<<8 | _buffer[54];
     
     _return = true;
  }
  return _return;
}

bool EVCONTROLLER::getAllStatus(uint16_t &vehicleStatus, uint16_t &charginCurrentCP, uint8_t &statusInputs, uint8_t &statusOutputs, uint16_t &timeConnectHH , uint8_t &timeConnectMM, uint8_t &timeConnectSS
      ,uint16_t &timeChargingHH , uint8_t &timeChargingMM, uint8_t &timeChargingSS , uint16_t &errorStatus){
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  
  if(modbusev.readMultipleParameter(0x5DC0, 26, _buffer))
  {	 
	// Status vehicle
	vehicleStatus = _buffer[3]<<8 | _buffer[4];
	
	// Charging current via signal CP in A
	charginCurrentCP = _buffer[10];
	
	// Status input 
     statusInputs = _buffer[12];
	 
	 // Status output
     statusOutputs = _buffer[14];
	 
	 // Time connect state B, C, D
     timeConnectHH = _buffer[43]<<8 | _buffer[44];
     timeConnectMM = _buffer[41];
     timeConnectSS = _buffer[42];
	 
	 // Time connect state C, D
     timeChargingHH = _buffer[39]<<8 | _buffer[40];
     timeChargingMM = _buffer[37];
     timeChargingSS = _buffer[38];

     errorStatus = _buffer[53]<<8 | _buffer[54];
	 
     _return = true;
  }
  return _return;  
}

bool EVCONTROLLER::getVehicleStatus(String &vehicleStatus){
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer)); 
  //if(modbusev.readMultipleParameter(0x5DC0, 26, _buffer))
  if(modbusev.readMultipleParameter(0x5DC0, 1, _buffer))
  {
     _valueStatus = _buffer[3]<<8 | _buffer[4];
     switch(_valueStatus)
     {
      case 0x4131 :
        vehicleStatus = "A1";
        break;
      case 0x4132 :
        vehicleStatus = "A2";
        break;
      case 0x4231 :
        vehicleStatus = "B1";
        break;
      case 0x4232 :
        vehicleStatus = "B2";
        break;
      case 0x4331 :
        vehicleStatus = "C1";
        break;
      case 0x4332 :
        vehicleStatus = "C2";
        break;
	  case 0x4431 :
        vehicleStatus = "D1";
        break;
	  case 0x4432 :
        vehicleStatus = "D2";
        break;
	  case 0x4530 :
        vehicleStatus = "E0";
        break;
	  case 0x4531 :
        vehicleStatus = "E1";
        break;
	  case 0x4630 :
        vehicleStatus = "F0";
        break;
	  case 0x4631 :
        vehicleStatus = "F1";
        break;
     }
	 _return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getChargingCurrentViaCP(uint16_t &charginCurrentViaCP)
{
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x5DC3, 1, _buffer))
  {
	  charginCurrentViaCP = _buffer[4];
	  _return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getStatusInput(uint8_t &statusInputs)
{
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x5DC4, 1, _buffer))
  {
	  statusInputs = _buffer[4];
	  _return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getStatusOutput(uint8_t &statusOutputs)
{
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x5DC5, 1, _buffer))
  {
	  statusOutputs = _buffer[4];
	  _return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getTimeConnect(uint16_t &timeConnectHH , uint8_t &timeConnectMM, uint8_t &timeConnectSS)
{
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x5DD3, 2, _buffer))
  {
	 timeConnectHH = _buffer[5]<<8 | _buffer[6];
     timeConnectMM = _buffer[3];
     timeConnectSS = _buffer[4];
	  _return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getTimeCharging(uint16_t &timeChargingHH , uint8_t &timeChargingMM, uint8_t &timeChargingSS)
{
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x5DD1, 2, _buffer))
  {
	 timeChargingHH = _buffer[5]<<8 | _buffer[6];
     timeChargingMM = _buffer[3];
     timeChargingSS = _buffer[4];
	 _return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getErrorStatus(uint16_t &errorStatus)
{
  _return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x5DD9, 1, _buffer))
  {
	 errorStatus = _buffer[3]<<8 | _buffer[4];
	 _return = true;
  }
  
  return _return;
}

bool EVCONTROLLER::getEmergencyRCM(String &error)
{
	_return = false;
  
  memset(_buffer, 0, sizeof(_buffer));
  if(modbusev.readMultipleParameter(0x6591, 1, _buffer))
  {
	 _valueError = _buffer[3]<<8 | _buffer[4];
	 if(_valueError<10000)
	 {
		 error = "Normal";
	 }
	 else if(_valueError>=10000 && _valueError <11500)
	 {
		 error = "RCM";
	 }
	 else if(_valueError>=11500)
	 {
		 error = "Emergency";
	 }
	 
	 _return = true;
  }
  
  return _return;
}