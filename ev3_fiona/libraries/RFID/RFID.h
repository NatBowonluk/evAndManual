#ifndef RFID_H
#define RFID_H
#include "PN532_I2C.h"
#include "PN532.h"
#include "NfcAdapter.h"

class RFID{
	public:
		bool init();
		void getIdHex(String &id, boolean &success);
		void getIdDecimal(String &id, boolean &success);
    private:
		boolean _success;
		String _id;
		uint8_t _uid[7];
		uint8_t _uidLength;
		uint16_t _result;
};
#endif
