#include "MODBUSBUFF.h"

void MODBUS::init(Stream &serial)
{
  _serial = &serial;
}

void MODBUS::init(int8_t slave, Stream &serial)
{
  _serial = &serial;
  _slave = slave;
}

bool MODBUS::readMultipleParameter(uint16_t address, uint16_t qty, uint8_t *Modbus_buffer)
{
  _Modbus_send[0] = _slave;
  _Modbus_send[1] = 0x04;
  _Modbus_send[2] = address>>8;
  _Modbus_send[3] = address&0x00FF;
  _Modbus_send[4] = qty>>8;
  _Modbus_send[5] = qty&0x00FF;
  crc_check(_crc, _Modbus_send, 6);

  _Modbus_send[6] = _crc[0];
  _Modbus_send[7] = _crc[1];

  memset(Modbus_buffer, 0, sizeof(Modbus_buffer));

  _serial->flush();
  _serial->write(_Modbus_send,8);

  _status = true;
  _readOk = true;
  _lastTime = millis();
  uint8_t i = 0;
  bool startbyte = false;
  while(_status&&_readOk)
  {
    if(_serial->available())
    {
	  Modbus_buffer[i] = _serial->read();
	  if(Modbus_buffer[i] == _slave && startbyte == false)
	  {
		  Modbus_buffer[0] = Modbus_buffer[i];
		  startbyte = true;
		  i = 0;
	  }
		
      if(i == (5+(qty*2))-1)     
        _readOk = false;
	
	  i++;
    }
    else if(millis() - _lastTime > 500)
    {
      _status = false;
    }
  }
  
  if(_readOk == false)
  {
	
	crc_check(_crc, Modbus_buffer, ((5+(qty*2))-2));
    if((_crc[0] == Modbus_buffer[(5+(qty*2))-2]) && (_crc[1] == Modbus_buffer[(5+(qty*2))-1]))
    {
      _return = true;
    }   
  }
  else
  {
    _return = false;
  }
  
  return _return;
}

bool MODBUS::readMultipleParameter(uint8_t slave ,uint16_t address, uint16_t qty, uint8_t *Modbus_buffer)
{
  _Modbus_send[0] = slave;
  _Modbus_send[1] = 0x04;
  _Modbus_send[2] = address>>8;
  _Modbus_send[3] = address&0x00FF;
  _Modbus_send[4] = qty>>8;
  _Modbus_send[5] = qty&0x00FF;
  crc_check(_crc, _Modbus_send, 6);

  _Modbus_send[6] = _crc[0];
  _Modbus_send[7] = _crc[1];

  memset(Modbus_buffer, 0, sizeof(Modbus_buffer));

  _serial->flush();
  _serial->write(_Modbus_send,8);

  _status = true;
  _readOk = true;
  _lastTime = millis();
  uint8_t i = 0;
  bool startbyte = false;
  while(_status&&_readOk)
  {
    if(_serial->available())
    {
	  Modbus_buffer[i] = _serial->read();
	  if(Modbus_buffer[i] == slave && startbyte == false)
	  {
		  Modbus_buffer[0] = Modbus_buffer[i];
		  startbyte = true;
		  i = 0;
	  }
		
      if(i == (5+(qty*2))-1)     
        _readOk = false;
	
	  i++;
    }
    else if(millis() - _lastTime > 500)
    {
      _status = false;
    }
  }
  
  if(_readOk == false)
  {
	
	crc_check(_crc, Modbus_buffer, ((5+(qty*2))-2));
    if((_crc[0] == Modbus_buffer[(5+(qty*2))-2]) && (_crc[1] == Modbus_buffer[(5+(qty*2))-1]))
    {
      _return = true;
    }   
  }
  else
  {
    _return = false;
  }
  
  return _return;
}

bool MODBUS::readMultipleParameter(uint16_t address, uint16_t qty, uint8_t *Modbus_buffer, uint16_t timeout)
{
  _Modbus_send[0] = _slave;
  _Modbus_send[1] = 0x04;
  _Modbus_send[2] = address>>8;
  _Modbus_send[3] = address&0x00FF;
  _Modbus_send[4] = qty>>8;
  _Modbus_send[5] = qty&0x00FF;
  crc_check(_crc, _Modbus_send, 6);

  _Modbus_send[6] = _crc[0];
  _Modbus_send[7] = _crc[1];

  memset(Modbus_buffer, 0, sizeof(Modbus_buffer));

  _serial->flush();
  _serial->write(_Modbus_send,8);

  _status = true;
  _readOk = true;
  _lastTime = millis();
  uint8_t i = 0;
  bool startbyte = false;
  while(_status&&_readOk)
  {
    if(_serial->available())
    {
	  Modbus_buffer[i] = _serial->read();
	  if(Modbus_buffer[i] == _slave && startbyte == false)
	  {
		  Modbus_buffer[0] = Modbus_buffer[i];
		  startbyte = true;
		  i = 0;
	  }
		
      if(i == (5+(qty*2))-1)     
        _readOk = false;
	
	  i++;
    }
    else if(millis() - _lastTime > timeout)
    {
      _status = false;
    }
  }
  
  if(_readOk == false)
  {
	
	crc_check(_crc, Modbus_buffer, ((5+(qty*2))-2));
    if((_crc[0] == Modbus_buffer[(5+(qty*2))-2]) && (_crc[1] == Modbus_buffer[(5+(qty*2))-1]))
    {
      _return = true;
    }   
  }
  else
  {
    _return = false;
  }
  
  return _return;
}

bool MODBUS::setfn06(uint8_t slave, uint16_t address, uint16_t value, uint8_t *Modbus_buffer){
  _Modbus_send[0] = slave;
  _Modbus_send[1] = 0x06;
  _Modbus_send[2] = address>>8;
  _Modbus_send[3] = address&0x00FF;
  _Modbus_send[4] = value>>8;
  _Modbus_send[5] = value&0x00FF;
  crc_check(_crc, _Modbus_send, 6);

  _Modbus_send[6] = _crc[0];
  _Modbus_send[7] = _crc[1];

  memset(Modbus_buffer, 0, sizeof(Modbus_buffer));

  _serial->flush();
  _serial->write(_Modbus_send,8);

  _status = true;
  _readOk = true;
  _lastTime = millis();
  uint8_t i = 0;
  bool startbyte = false;
  while(_status&&_readOk)
  {
    if(_serial->available())
    {
     /*  Modbus_buffer[i++] = _serial->read();
      if(i == (5+(qty*2)))     
        _readOk = false; */
	  Modbus_buffer[i] = _serial->read();
	  if(Modbus_buffer[i] == slave && startbyte == false)
	  {
		  Modbus_buffer[0] = Modbus_buffer[i];
		  startbyte = true;
		  i = 0;
	  }
		
      if(i == 7)     
        _readOk = false;
	
	  i++;
    }
    else if(millis() - _lastTime > 500)
    {
      _status = false;
    }
  }
  
  if(_readOk == false)
  {
    //crc_check(_crc, Modbus_buffer, ((5+(qty*2))-2));
    //if((_crc[0] == Modbus_buffer[(5+(qty*2))-2]) && (_crc[1] == Modbus_buffer[(5+(qty*2))-1]))
	
	crc_check(_crc, Modbus_buffer, 6);
    if((_crc[0] == Modbus_buffer[6]) && (_crc[1] == Modbus_buffer[7]))
    {
      _return = true;
    }   
  }
  else
  {
    _return = false;
  }
  
  return _return;
}

bool MODBUS::setfn06(uint16_t address, uint16_t value, uint8_t *Modbus_buffer){
  _Modbus_send[0] = _slave;
  _Modbus_send[1] = 0x06;
  _Modbus_send[2] = address>>8;
  _Modbus_send[3] = address&0x00FF;
  _Modbus_send[4] = value>>8;
  _Modbus_send[5] = value&0x00FF;
  crc_check(_crc, _Modbus_send, 6);

  _Modbus_send[6] = _crc[0];
  _Modbus_send[7] = _crc[1];

  memset(Modbus_buffer, 0, sizeof(Modbus_buffer));

  _serial->flush();
  _serial->write(_Modbus_send,8);

  _status = true;
  _readOk = true;
  _lastTime = millis();
  uint8_t i = 0;
  bool startbyte = false;
  while(_status&&_readOk)
  {
    if(_serial->available())
    {
     /*  Modbus_buffer[i++] = _serial->read();
      if(i == (5+(qty*2)))     
        _readOk = false; */
	  Modbus_buffer[i] = _serial->read();
	  if(Modbus_buffer[i] == _slave && startbyte == false)
	  {
		  Modbus_buffer[0] = Modbus_buffer[i];
		  startbyte = true;
		  i = 0;
	  }
		
      if(i == 7)     
        _readOk = false;
	
	  i++;
    }
    else if(millis() - _lastTime > 500)
    {
      _status = false;
    }
  }
  
  if(_readOk == false)
  {
    //crc_check(_crc, Modbus_buffer, ((5+(qty*2))-2));
    //if((_crc[0] == Modbus_buffer[(5+(qty*2))-2]) && (_crc[1] == Modbus_buffer[(5+(qty*2))-1]))
	
	crc_check(_crc, Modbus_buffer, 6);
    if((_crc[0] == Modbus_buffer[6]) && (_crc[1] == Modbus_buffer[7]))
    {
      _return = true;
    }   
  }
  else
  {
    _return = false;
  }
  
  return _return;
}

bool MODBUS::readSingleCoil(uint16_t address, uint8_t *Modbus_buffer)
{
  _Modbus_send[0] = _slave;
  _Modbus_send[1] = 0x01;
  _Modbus_send[2] = address>>8;
  _Modbus_send[3] = address&0x00FF;
  _Modbus_send[4] = 0x00;
  _Modbus_send[5] = 0x01;
  crc_check(_crc, _Modbus_send, 6);

  _Modbus_send[6] = _crc[0];
  _Modbus_send[7] = _crc[1];

  memset(Modbus_buffer, 0, sizeof(Modbus_buffer));

  _serial->flush();
  _serial->write(_Modbus_send,8);

  _status = true;
  _readOk = true;
  _lastTime = millis();
  uint8_t i = 0;
  bool startbyte = false;
  while(_status&&_readOk)
  {
    if(_serial->available())
    {
     /*  Modbus_buffer[i++] = _serial->read();
      if(i == (5+(qty*2)))     
        _readOk = false; */
	  Modbus_buffer[i] = _serial->read();
	  if(Modbus_buffer[i] == _slave && startbyte == false)
	  {
		  Modbus_buffer[0] = Modbus_buffer[i];
		  startbyte = true;
		  i = 0;
	  }
		
      if(i == 5)     
        _readOk = false;
	
	  i++;
    }
    else if(millis() - _lastTime > 500)
    {
      _status = false;
    }
  }
  
  if(_readOk == false)
  {
	crc_check(_crc, Modbus_buffer, 4);
    if((_crc[0] == Modbus_buffer[4]) && (_crc[1] == Modbus_buffer[5]))
    {
      _return = true;
    }   
  }
  else
  {
    _return = false;
  }
  
  return _return;
}

bool MODBUS::readSingleCoil(uint16_t address, uint8_t *Modbus_buffer, uint16_t timeout)
{
  _Modbus_send[0] = _slave;
  _Modbus_send[1] = 0x01;
  _Modbus_send[2] = address>>8;
  _Modbus_send[3] = address&0x00FF;
  _Modbus_send[4] = 0x00;
  _Modbus_send[5] = 0x01;
  crc_check(_crc, _Modbus_send, 6);

  _Modbus_send[6] = _crc[0];
  _Modbus_send[7] = _crc[1];

  memset(Modbus_buffer, 0, sizeof(Modbus_buffer));

  _serial->flush();
  _serial->write(_Modbus_send,8);

  _status = true;
  _readOk = true;
  _lastTime = millis();
  uint8_t i = 0;
  bool startbyte = false;
  while(_status&&_readOk)
  {
    if(_serial->available())
    {
     /*  Modbus_buffer[i++] = _serial->read();
      if(i == (5+(qty*2)))     
        _readOk = false; */
	  Modbus_buffer[i] = _serial->read();
	  if(Modbus_buffer[i] == _slave && startbyte == false)
	  {
		  Modbus_buffer[0] = Modbus_buffer[i];
		  startbyte = true;
		  i = 0;
	  }
		
      if(i == 5)     
        _readOk = false;
	
	  i++;
    }
    else if(millis() - _lastTime > timeout)
    {
      _status = false;
    }
  }
  
  if(_readOk == false)
  {
	crc_check(_crc, Modbus_buffer, 4);
    if((_crc[0] == Modbus_buffer[4]) && (_crc[1] == Modbus_buffer[5]))
    {
      _return = true;
    }   
  }
  else
  {
    _return = false;
  }
  
  return _return;
}

bool MODBUS::writeSingleCoil(uint16_t address, uint16_t value, uint8_t *Modbus_buffer){
  _Modbus_send[0] = _slave;
  _Modbus_send[1] = 0x05;
  _Modbus_send[2] = address>>8;
  _Modbus_send[3] = address&0x00FF;
  _Modbus_send[4] = value>>8;
  _Modbus_send[5] = value&0x00FF;
  crc_check(_crc, _Modbus_send, 6);

  _Modbus_send[6] = _crc[0];
  _Modbus_send[7] = _crc[1];

  memset(Modbus_buffer, 0, sizeof(Modbus_buffer));

  _serial->flush();
  _serial->write(_Modbus_send,8);

  _status = true;
  _readOk = true;
  _lastTime = millis();
  uint8_t i = 0;
  bool startbyte = false;
  while(_status&&_readOk)
  {
    if(_serial->available())
    {
     /*  Modbus_buffer[i++] = _serial->read();
      if(i == (5+(qty*2)))     
        _readOk = false; */
	  Modbus_buffer[i] = _serial->read();
	  if(Modbus_buffer[i] == _slave && startbyte == false)
	  {
		  Modbus_buffer[0] = Modbus_buffer[i];
		  startbyte = true;
		  i = 0;
	  }
		
      if(i == 7)     
        _readOk = false;
	
	  i++;
    }
    else if(millis() - _lastTime > 500)
    {
      _status = false;
    }
  }
  
  if(_readOk == false)
  {
    //crc_check(_crc, Modbus_buffer, ((5+(qty*2))-2));
    //if((_crc[0] == Modbus_buffer[(5+(qty*2))-2]) && (_crc[1] == Modbus_buffer[(5+(qty*2))-1]))
	
	crc_check(_crc, Modbus_buffer, 6);
    if((_crc[0] == Modbus_buffer[6]) && (_crc[1] == Modbus_buffer[7]))
    {
      _return = true;
    }   
  }
  else
  {
    _return = false;
  }
  
  return _return;
}

void MODBUS::crc_check(uint8_t *crc, byte buf[], int len)
{
  uint16_t crc_cal = 0xFFFF;

  for (int pos = 0; pos < len; pos++)
  {
    crc_cal ^= (uint16_t)buf[pos];

    for (int i = 8; i != 0; i--)
    {
      if ((crc_cal & 0x0001) != 0)
      {
        crc_cal >>= 1;
        crc_cal ^= 0xA001;
      }
      else
      {
        crc_cal >>= 1;
      }
     }

  }

  //return crc_cal;
  
  crc[0] = crc_cal&0x00FF;
  crc[1] = crc_cal>>8;
  //_serial->write(crc_cal);
}

