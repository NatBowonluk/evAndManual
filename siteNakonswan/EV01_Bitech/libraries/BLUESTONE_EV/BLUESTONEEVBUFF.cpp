#include <BLUESTONEEVBUFF.h>

MODBUS modbusev3;

void BLUESTONEEV::init(Stream &serial)
{
	_serial = &serial;
	modbusev3.init(*_serial);
	//modbusev3.init(0x11 , *_serial);
}

bool BLUESTONEEV::setCurrent(uint8_t slave, uint8_t current_setting){
	_return = false;
	if(modbusev3.setfn06(slave, 0x7532, current_setting, _buffers))
	{
		if(current_setting == _buffers[5])
      _return = true;
	} 
	return _return;
}

bool BLUESTONEEV::readAll(uint8_t slave, String &vehicleStatus, uint8_t &current_meter, uint8_t &current_setting)
{
	_return = false;
	vehicleStatus = ""; 	current_meter = 0; current_setting = 0;		
	memset(_buffers, 0, sizeof(_buffers));
	if(modbusev3.readMultipleParameter(slave, 0x7530, 3, _buffers))
	{
		//vehicleState = "A1";
		_valueStatus = _buffers[3]<<8 | _buffers[4];
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
		
		current_meter = _buffers[6];
		current_setting = _buffers[8];
		_return = true;
	}
	return _return;
}


