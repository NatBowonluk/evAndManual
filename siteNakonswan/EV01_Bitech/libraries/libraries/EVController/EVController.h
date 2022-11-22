#ifndef EVCONTROLLER_H
#define EVCONTROLLER_h

#include <MODBUS.h>


class EVCONTROLLER{
  public:
    void init(int8_t slave, Stream &serial);
	bool getSerialNumber(String &serialNumber);
    bool getConfigSwitch(String &sw1, String &sw2); 
    bool getSettingMaxCurrent(uint8_t &maxCurrent);
    bool getEn(uint16_t &en);
    bool setEn(uint16_t en);
    bool getCoil20000(String &value);
	bool getCoil20000(byte &value);
	bool getEmergencyRCM(String &error);
    bool setCoil20000(String value);
    bool getAllStatus(String &vehicleStatus, uint16_t &charginCurrentCP, uint8_t &statusInputs, uint8_t &statusOutputs, uint16_t &timeConnectHH , uint8_t &timeConnectMM, uint8_t &timeConnectSS
      ,uint16_t &timeChargingHH , uint8_t &timeChargingMM, uint8_t &timeChargingSS , uint16_t &errorStatus);
	bool getAllStatus(uint16_t &vehicleStatus, uint16_t &charginCurrentCP, uint8_t &statusInputs, uint8_t &statusOutputs, uint16_t &timeConnectHH , uint8_t &timeConnectMM, uint8_t &timeConnectSS
      ,uint16_t &timeChargingHH , uint8_t &timeChargingMM, uint8_t &timeChargingSS , uint16_t &errorStatus);
	bool getVehicleStatus(String &vehicleStatus);
	bool getChargingCurrentViaCP(uint16_t &charginCurrentViaCP);
	bool getStatusInput(uint8_t &statusInputs);
	bool getStatusOutput(uint8_t &statusOutputs);
	bool getTimeConnect(uint16_t &timeConnectHH , uint8_t &timeConnectMM, uint8_t &timeConnectSS);
	bool getTimeCharging(uint16_t &timeChargingHH , uint8_t &timeChargingMM, uint8_t &timeChargingSS);
	bool getErrorStatus(uint16_t &errorStatus);
	bool getSettingMaxCurrentPermissible(uint8_t &maxCurrentPermissible);
	bool setSettingMaxCurrentPermissible(uint8_t maxCurrentPermissible);
	bool restartEVController();
  private:
    bool _return;
    uint8_t _buffer[100];
    uint32_t _value;
    uint8_t _slave;
    Stream* _serial;
    float _result;
    String _sw1,_sw2;
    uint8_t _swDip5;
    uint8_t _swDip6;
    uint8_t _swDip56;
    uint16_t _valueSingleCoil;
    uint16_t _valueStatus;
	uint16_t _valueError;
};

#endif
