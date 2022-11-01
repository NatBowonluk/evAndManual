#include <EVController.h>

EVCONTROLLER ev;

String serialNumber;
uint8_t maxCurrentPermissible;
uint8_t maxCurrent;
uint16_t en;
String value;
String vehicleStatus;
uint16_t charginCurrentViaCP;
uint8_t statusInputs;
uint8_t statusOutputs;
uint16_t timeConnectHH;
uint8_t timeConnectMM;
uint8_t timeConnectSS;
uint16_t timeChargingHH;
uint8_t timeChargingMM; 
uint8_t timeChargingSS;
uint16_t errorStatus;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("Start");
  // initial port RS485 Protocol Modbus RTU ev controller Slave ID = 2
  Serial2.begin(9600);
  ev.init(2, Serial2);
  delay(1000);

  // address 1021 - 1026 Serail Number
  if(ev.getSerialNumber(serialNumber))
  {
    Serial.print("Serail Number : "); Serial.println(serialNumber);
  }
  Serial.println();
  delay(10);

  // get address 22000 setting the maximum permissible charging current
  if(ev.getSettingMaxCurrentPermissible(maxCurrentPermissible))
  {
    Serial.print("Max current permissilbe = "); Serial.println(maxCurrentPermissible);
  }
  Serial.println();
  delay(10);
/*
  // set address 22000 setting the maximum permissible charging current
  if(ev.setSettingMaxCurrentPermissible(16))
  {
    Serial.println("Set maximum permissible charging current OK");
  }
  Serial.println();
  delay(10);
 */ 
  // get max current address 2000 via S1, DIP5 and DIP6
  if(ev.getSettingMaxCurrent(maxCurrent))
  {
    Serial.print("Max Current = ");
    Serial.println(maxCurrent, DEC);
  }
  Serial.println();
  delay(10);

  // address 4000 set Configuring the enable charging process function
  // value is 0, 1, 2 3
  /*
  if(ev.setEn(1))
  {
    Serial.println("Set OK");
  }
  Serial.println();
  delay(10);
  */
  
  // address 4000 get Configuring the enable charging process function
  if(ev.getEn(en))
  {
    Serial.print("en = ");
    Serial.println(en);
  }
  Serial.println();
  delay(10);

  // address 20000 set enabling the charging process
  // pass value "ON" is enable or "OFF" is disable
  /*
  if(ev.setCoil20000("ON"))
  {
    Serial.print("Set OK");   
  }
  Serial.println();
  delay(10);
  */

  // address 20000 get enabling the charging process
  if(ev.getCoil20000(value))
  {
    Serial.print("value coil 20000 = ");   Serial.println(value);
  }
  Serial.println();
  delay(10);
  
  
  // address 24000 get vehicle status A1, A2, B1, B2, C1, C2
  if(ev.getVehicleStatus(vehicleStatus))
  {
    Serial.print("Vehicle status = ");  Serial.println(vehicleStatus);
  }
  Serial.println();
  delay(10);
  
  // address 24003 get charging current specification via signal CP
  if(ev.getChargingCurrentViaCP(charginCurrentViaCP))
  {
    Serial.print("Charging current specification via signal CP = ");  Serial.println(charginCurrentViaCP);
  }
  Serial.println();
  delay(10);

  // addres 24004 get status of digital inputs 
  if(ev.getStatusInput(statusInputs))
  {
    Serial.print("Status of digital inpus = ");  Serial.println(statusInputs, BIN);
    // Bit 0 is OUT
    if((statusInputs&0b00000001) == 0b00000001)
      Serial.print("EN = 1 ");
    else
      Serial.print("EN = 0 ");

    // Bit 1 is OUT
    if((statusInputs&0b00000010) == 0b00000010)
      Serial.print("XR = 1 ");
    else
      Serial.print("XR = 0 ");

    // Bit 2 is OUT
    if((statusInputs&0b00000100) == 0b00000100)
      Serial.print("ML = 1 ");
    else
      Serial.print("ML = 0 ");

    // Bit 3 is CON
    if((statusInputs&0b00001000) == 0b00001000)
      Serial.print("CCR = 1 ");
    else
      Serial.print("CCR = 0 ");

    // Bit 4 is IN
    if((statusInputs&0b00010000) == 0b00010000)
      Serial.print("IN = 1 ");
    else
      Serial.print("IN = 0 ");

    Serial.println();
  }
  Serial.println();
  delay(10);

  // Address 24005 status of digital outputs
  if(ev.getStatusOutput(statusOutputs))
  {
    Serial.print("Status of digital outputs = ");  Serial.println(statusOutputs, BIN);

     // Bit 0 is OUT
    if((statusOutputs&0b00000001) == 0b00000001)
      Serial.print("OUT = 1 ");
    else
      Serial.print("OUT = 0 ");

    // Bit 1 is OUT
    if((statusOutputs&0b00000010) == 0b00000010)
      Serial.print("ERR = 1 ");
    else
      Serial.print("ERR = 0 ");

    // Bit 2 is OUT
    if((statusOutputs&0b00000100) == 0b00000100)
      Serial.print("CHG = 1 ");
    else
      Serial.print("CHG = 0 ");

    // Bit 3 is CON
    if((statusOutputs&0b00001000) == 0b00001000)
      Serial.print("CON = 1 ");
    else
      Serial.print("CON = 0 ");

    Serial.println();
  } 

  Serial.println();
  delay(10);

  // Address 24019 - 24020 time connect hhhh, mm ,ss
  if(ev.getTimeConnect(timeConnectHH , timeConnectMM, timeConnectSS))
  {
    Serial.print("Time connect Hours = ");
    Serial.println(timeConnectHH);
    Serial.print("Time connect Minutes = ");
    Serial.println(timeConnectMM);
    Serial.print("Time connect Second = ");
    Serial.println(timeConnectSS);
  }
  Serial.println();
  delay(10);
  
  // Address 24017 - 24018 time connect hhhh, mm ,ss
  if(ev.getTimeCharging(timeChargingHH , timeChargingMM, timeChargingSS))
  {
    Serial.print("Time charging Hours = ");
    Serial.println(timeChargingHH);
    Serial.print("Time charging Minutes = ");
    Serial.println(timeChargingMM);
    Serial.print("Time charging Second = ");
    Serial.println(timeChargingSS);
  }
  Serial.println();
  delay(10);
  
  // Address 24025 Error status
  if(ev.getErrorStatus(errorStatus))
  {
    Serial.print("Error status = ");  Serial.println(errorStatus, BIN);
    // Bit 2 rejection of 13 A cable
    if((errorStatus&0x0002) == 0x0002)
      Serial.println("rejection of 13A cable ");
    // Bit 3 invalid PP value
    if((errorStatus&0x0004) == 0x0004)
      Serial.println("invalid PP value ");
    // Bit 4 invalid CP value
    if((errorStatus&0x0008) == 0x0008)
      Serial.println("invalid CP value ");
    // Bit 5 status F due to no charging station availability
    if((errorStatus&0x0010) == 0x0010)
      Serial.println("status F due to no charging station availability ");
    // Bit 6 locking
    if((errorStatus&0x0020) == 0x0020)
      Serial.println("locking");
    // Bit 7 locking
    if((errorStatus&0x0040) == 0x0040)
      Serial.println("unlocking");
    // Bit 8 LD unavailable during locking
    if((errorStatus&0x0080) == 0x0080)
      Serial.println("LD unavailable during locking");
    // Bit 11 status D, vehicle rejected
    if((errorStatus&0x0400) == 0x0400)
      Serial.println("status D, vehicle rejected");
    // Bit 12 charging contactor error
    if((errorStatus&0x0800) == 0x0800)
     Serial.println("charging contactor error");
    // Bit 13 no diode in the Control Pilot circuit in the vehicle
    if((errorStatus&0x1000) == 0x1000)
      Serial.println("no diode in the Control Pilot circuit in the vehicle");
    // Bit 15 EV-RCM residual current detection triggered
    if((errorStatus&0x4000) == 0x4000)
      Serial.println("EV-RCM residual current detection triggered");
    // Bit 16 EV-RCM selftest error
    if((errorStatus&0x8000) == 0x8000)
      Serial.println("EV-RCM selftest error");
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
