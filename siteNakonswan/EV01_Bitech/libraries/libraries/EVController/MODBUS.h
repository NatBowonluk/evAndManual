#ifndef MODBUS_H
#define MODBUS_H
#include <Arduino.h>

class MODBUS{
  public:
    void init(int8_t slave, Stream &serial);
    bool readMultipleParameter(uint16_t address, uint16_t qty, uint8_t *Modbus_buffer);
	bool setfn06(uint16_t address, uint16_t value, uint8_t *Modbus_buffer);
	bool readSingleCoil(uint16_t address, uint8_t *Modbus_buffer);
	bool writeSingleCoil(uint16_t address, uint16_t value, uint8_t *Modbus_buffer);
    
  private:
    uint8_t _slave;
    Stream* _serial;
    uint8_t _address[2];
    uint8_t _Modbus_send[8];
    uint8_t _Modbus_buffer[100];
    uint8_t _qty;
    uint8_t _fn;
    uint8_t _crc[2];
    uint32_t _lastTime;
    bool _status;
    bool _readOk;
    bool _return;
    //void crc_check(uint8_t[2] &crc);
    void crc_check(uint8_t *crc, byte buf[], int len);
};
#endif
