#ifndef BLUESTONEEVBUFF_H
#define BLUESTONEEVBUFF_H

#include <MODBUSBUFF.h>

class BLUESTONEEV{
	public:
		void init(Stream &serial);
		bool readAll(uint8_t slave, String &vehicleStatus, uint8_t &current_meter, uint8_t &current_setting);
		bool setCurrent(uint8_t slave, uint8_t current_setting);
	private:
		uint8_t _slave;
		Stream* _serial;
		bool _return;
		uint8_t _buffers[20];
		uint16_t _valueStatus;
};
#endif