struct evtime {
  //  uint16_t timeConnectHH = 99;
  //  uint8_t timeConnectMM = 99;
  //  uint8_t timeConnectSS = 99;
  //  uint16_t timeChargingHH = 99;
  //  uint8_t timeChargingMM = 99;
  //  uint8_t timeChargingSS = 99;
  uint16_t timeConnectHH;
  uint8_t timeConnectMM;
  uint8_t timeConnectSS;
  uint16_t timeChargingHH;
  uint8_t timeChargingMM;
  uint8_t timeChargingSS;
};
evtime evv;

const unsigned long eventIntervalEV = 1000;
unsigned long previousTimeEV = 0;

void evinit(void);
#if EVCONTROL == 1
String evSerialNo(void);
uint8_t evMaxGetCurrent();

#else

#endif
uint8_t evEmergengy(void);
uint8_t evUserGetCurrent(void);
bool evUserSetCurrent(uint8_t current);
uint8_t evGetMode(void);
bool evSetMode(uint8_t en);
uint8_t evGetCoil(void);
bool evOpenCharge(String stat);
uint8_t evErrorEZero(void);
uint8_t evCarGetStat(void);
void timeConnect(void);
void timeCharging(void);

float evGetTemp(void);

void evinit() {
#if EVCONTROL == 1
  ev.init(2, Serial2);
#elif boardColour == 2 && EVCONTROL == 0
  swDelay(1000);
  ev.init(Serial1); //WA
  debugln("/////////////////////////////////////////////////////20");
#else
  swDelay(1000);
  ev.init(Serial2); //WA
  debugln("/////////////////////////////////////////////////////a0");
#endif
}

#if EVCONTROL == 1
String evSerialNo() {
  swDelay(10);
  // address 1021 - 1026 Serail Number
  String serialNumber;
  if (ev.getSerialNumber(serialNumber))
  {
    return serialNumber;
  }
  else {
    return "failed to get serial number";
  }
}
#else
#endif


uint8_t evUserGetCurrent() {
  swDelay(10);
  // get address 22000 setting the maximum permissible charging current
  uint8_t maxCurrentPermissible;
  if (ev.getSettingMaxCurrentPermissible(maxCurrentPermissible))
  {
    return maxCurrentPermissible;
  }
  else {
    return 0;
  }
}

bool evUserSetCurrent(uint8_t current) {
  swDelay(10);
  // set address 22000 setting the maximum permissible charging current
  if (ev.setSettingMaxCurrentPermissible(current))
  {
    //    debugln("Set maximum permissible charging current OK");
    return true;
  }
  else {
    return false;
  }
}

#if EVCONTROL == 1
uint8_t evMaxGetCurrent() {
  swDelay(10);
  // get max current address 2000 via S1, DIP5 and DIP6
  uint8_t maxCurrent;
  if (ev.getSettingMaxCurrent(maxCurrent))
  {
    return maxCurrent;
  }
  else {
    return 0;
  }
}
#else
#endif


// address 4000 get Configuring the enable charging process function
//// value is 0, 1, 2 3 -- 4 mean false to get
uint8_t evGetMode() {
  swDelay(10);
#if EVCONTROL == 1
  uint16_t en;
#else
  uint8_t en;
#endif
  if (ev.getEn(en))
  {
    return en;
  }
  return 4;
}

bool evSetMode(uint8_t en) {
  swDelay(10);
  if (ev.setEn(en))
  {
    return true;
  }
  else {
    return false;
  }
}

uint8_t evGetCoil() {
  // get coil address 20000
  swDelay(10);
  String value;
  if (ev.getCoil20000(value))
  {
    if (value == "ON") {
      return 1;
    }
    else if (value == "OFF") {
      return 2;
    }
  }
  else {
    return 3;
  }
}

bool evOpenCharge(String stat) {
  // address 20000 set enabling the charging process
  // pass value "ON" is enable or "OFF" is disable
  swDelay(10);
  if (stat == "ON") {
    if (ev.setCoil20000("ON"))
    {
      debugln("ononononononononon");
      return 1;
    }
  }
  else if (stat == "OFF") {
    if (ev.setCoil20000("OFF"))
    {
      debugln("offoffoffoffoff");
      return 0;
    }
  }
}

//#if EVCONTROL == 1
uint8_t evEmergengy() {
  swDelay(10);
  String error;
  if (ev.getEmergencyRCM(error)) {
    if (error == "Normal") {
      debugln("Normal");
      errorTemptext = "";
      //      debugln(errorTemptext);
      return 0;
    }
    else if (error == "RCM") {
      //      debugln("RCM");
      errorTemptext = "RCM";
      debugln(errorTemptext);
      return 1;
    }
    else if (error == "Emergency") {
      //      debugln("Emergency");
      errorTemptext = "Emergency";
      debugln(errorTemptext);
      return 2;
    }
  }
  else {
    debugln("Normal");
    errorTemptext = "NOT FOUND";
    //      debugln(errorTemptext);
    return 0;
  }
}
//#else
//#endif

#if EVCONTROL == 1
uint8_t evErrorEZero() {
  swDelay(10);
  uint16_t evErrorStatus;
  if (ev.getErrorStatus(evErrorStatus))
  {
    debug("Error status = ");  debugln(evErrorStatus);
    // Bit 2 rejection of 13 A cable
    if ((evErrorStatus & 0x0002) == 0x0002) {
      errorTemptext = "rejection of 13A cable ";
      debugln(errorTemptext);
      //      debugln("rejection of 13A cable ");
      return 1;
    }
    // Bit 3 invalid PP value
    if ((evErrorStatus & 0x0004) == 0x0004) {
      //      debugln("invalid PP value ");
      errorTemptext = "invalid PP value ";
      debugln(errorTemptext);
      return 2;
    }
    // Bit 4 invalid CP value
    if ((evErrorStatus & 0x0008) == 0x0008) {
      //      debugln("invalid CP value ");
      errorTemptext = "invalid CP value ";
      debugln(errorTemptext);
      return 3;
    }
    // Bit 5 status F due to no charging station availability
    if ((evErrorStatus & 0x0010) == 0x0010) {
      //      debugln("status F due to no charging station availability ");
      errorTemptext = "status F due to no charging station availability ";
      debugln(errorTemptext);
      return 4;
    }
    // Bit 6 locking
    if ((evErrorStatus & 0x0020) == 0x0020) {
      //      debugln("locking");
      errorTemptext = "locking";
      debugln(errorTemptext);
      return 5;
    }
    // Bit 7 locking
    if ((evErrorStatus & 0x0040) == 0x0040) {
      //      debugln("unlocking");
      errorTemptext = "unlocking";
      debugln(errorTemptext);
      return 6;
    }
    // Bit 8 LD unavailable during locking
    if ((evErrorStatus & 0x0080) == 0x0080) {
      //      debugln("LD unavailable during locking");
      errorTemptext = "LD unavailable during locking";
      debugln(errorTemptext);
      return 7;
    }
    // Bit 11 status D, vehicle rejected
    if ((evErrorStatus & 0x0400) == 0x0400) {
      //      debugln("status D, vehicle rejected");
      errorTemptext = "status D, vehicle rejected";
      debugln(errorTemptext);
      return 8;
    }
    // Bit 12 charging contactor error
    if ((evErrorStatus & 0x0800) == 0x0800) {
      //      debugln("charging contactor error");
      errorTemptext = "charging contactor error";
      debugln(errorTemptext);
      return 9;
    }
    // Bit 13 no diode in the Control Pilot circuit in the vehicle
    if ((evErrorStatus & 0x1000) == 0x1000) {
      //      debugln("no diode in the Control Pilot circuit in the vehicle");
      errorTemptext = "no diode in the Control Pilot circuit in the vehicle";
      debugln(errorTemptext);
      return 10;
    }
    // Bit 15 EV-RCM residual current detection triggered
    if ((evErrorStatus & 0x4000) == 0x4000) {
      //      debugln("EV-RCM residual current detection triggered");
      errorTemptext = "EV-RCM residual current detection triggered";
      debugln(errorTemptext);
      return 11;
    }
    // Bit 16 EV-RCM selftest error
    if ((evErrorStatus & 0x8000) == 0x8000) {
      //      debugln("EV-RCM selftest error");
      errorTemptext = "EV-RCM selftest errord";
      debugln(errorTemptext);
      return 12;
    }
  }
  else {
    debugln("No ERROR E0");
    return 0;
  }
}

uint8_t evCarGetStat() {
  // address 24000 get vehicle status A1, A2, B1, B2, C1, C2
  // address 24000 get vehicle status A1, A2, B1, B2, C1, C2, NN
  /////////////////////////////////////0, 1,  2,  3,  4,  5,  6
  swDelay(10);
  String vehicleStatus;

  if (evEmergengy() == 0) {
    swDelay(10);
    if (evErrorEZero() == 0) {
      swDelay(10);
      if (ev.getVehicleStatus(vehicleStatus))
      {
        if (vehicleStatus == "A1") {
          errorTemptext = "";
          return 0;
        }
        else if (vehicleStatus == "A2") {
          errorTemptext = "";
          return 1;
        }
        else if (vehicleStatus == "B1") {
          errorTemptext = "";
          return 2;
        }
        else if (vehicleStatus == "B2") {
          errorTemptext = "";
          return 3;
        }
        else if (vehicleStatus == "C1") {
          errorTemptext = "";
          return 4;
        }
        else if (vehicleStatus == "C2") {
          errorTemptext = "";
          return 5;
        }
        else {
          debugln("ddddddd");
          return 6;
        }
      }
      else {
        debugln("asdfghjgfgdfgf");
        return 6;
      }
    }
    else if (evErrorEZero() != 0) {
      debugln("dfjhvbdfivc");
      debugln(evErrorEZero());
      return 6;
    }
  }
  else if (evEmergengy() != 0) {
    debugln("7e3uwdjcxndsfg");
    return 6;
  }
}
#else
uint8_t evErrorEZero() {
  swDelay(10);
  uint8_t evErrorStatus;
  if (ev.getErrorStatus(evErrorStatus))
  {
    debug("Error status = ");  debugln(evErrorStatus);
    // Bit 2 rejection of 13 A cable
    if (evErrorStatus == 0x30)
    {
      debugln("_FAULT");
      errorTemptext = "_FAULT";
      return 1;
    }
    else if (evErrorStatus == 0x31)
    {
      debugln("_NORMAL");
      errorTemptext = "";
      return 0;
    }
    else if (evErrorStatus == 0x32)
    {
      debugln("_OV");
      errorTemptext = "_OV";
      return 2;
    }
    else if (evErrorStatus == 0x33)
    {
      debugln("_UV");
      errorTemptext = "_UV";
      return 3;
    }
    else if (evErrorStatus == 0x34)
    {
      debugln("_EOL");
      errorTemptext = "_EOL";
      return 4;
    }
    else if (evErrorStatus == 0x35)
    {
      debugln("_RESET");
      errorTemptext = "_RESET";
      return 5;
    }
    else if (evErrorStatus == 0x36)
    {
      debugln("_ST");
      errorTemptext = "_ST";
      return 6;
    }
    else if (evErrorStatus == 0x37)
    {
      debugln("_TBC");
      errorTemptext = "_TBC";
      return 7;
    }
    else {
      errorTemptext = "";
      return 0;
    }
  }
}

uint8_t evCarGetStat() {
  // address 24000 get vehicle status A1, A2, B1, B2, C1, C2
  // address 24000 get vehicle status A1, A2, B1, B2, C1, C2, NN
  /////////////////////////////////////0, 1,  2,  3,  4,  5,  6
  swDelay(10);
  String vehicleStatus;

  //  if (evEmergengy() == 0) {
  //    swDelay(10);
  //  Serial.print("/////////////////////////////////////////////////ERROR : "); Serial.println(evErrorEZero());
  //  Serial.println("//////////////////////////////////////////////////////////////////////////////////");
  if (evEmergengy() == 0) {
    swDelay(10);
    if (evErrorEZero() == 0) {
      swDelay(10);
      if (ev.getVehicleStatus(vehicleStatus))
      {
        if (vehicleStatus == "A1") {
          errorTemptext = "";
          return 0;
        }
        else if (vehicleStatus == "A2") {
          errorTemptext = "";
          //        Serial.print("vehicleStatus in ev function : "); Serial.println(vehicleStatus);
          return 1;
        }
        else if (vehicleStatus == "B1") {
          errorTemptext = "";
          return 2;
        }
        else if (vehicleStatus == "B2") {
          errorTemptext = "";
          return 3;
        }
        else if (vehicleStatus == "C1") {
          errorTemptext = "";
          return 4;
        }
        else if (vehicleStatus == "C2") {
          errorTemptext = "";
          return 5;
        }
        else {
          errorTemptext = "NOT READ IN STATUS";
          debugln("ddddddd");
          return 6;
        }
      }
      else {
        errorTemptext = "NOT READ IN ERROR";
        debugln("asdfghjgfgdfgf");
        return 6;
      }
    }
    else if (evErrorEZero() != 0) {
      debugln("dfjhvbdfivc");
      debugln(evErrorEZero());
      return 6;
    }

  }
  else if (evEmergengy() != 0) {
    debugln("7e3uwdjcxndsfg");
    return 6;
  }
}
#endif


void timeConnect() {
  // Address 24019 - 24020 time connect hhhh, mm ,ss
  swDelay(10);
  if (!ev.getTimeConnect(evv.timeConnectHH , evv.timeConnectMM, evv.timeConnectSS))
  {
    debugln("failed to get time connect from station");
    //    debug("Time connect Hours = "); debugln(evv.timeConnectHH);
    //    debug("Time connect Minutes = "); debugln(evv.timeConnectMM);
    //    debug("Time connect Second = "); debugln(evv.timeConnectSS);
  }
  else {
    //    evv.timeConnectHH = 99;
    //    evv.timeConnectMM = 99;
    //    evv.timeConnectSS = 99;
    //    debug("Time connect Hours = "); debugln(evv.timeConnectHH);
    //    debug("Time connect Minutes = "); debugln(evv.timeConnectMM);
    //    debug("Time connect Second = "); debugln(evv.timeConnectSS);
  }
}

void timeCharging() {
  swDelay(10);
  // Address 24017 - 24018 time connect hhhh, mm ,ss
  if (!ev.getTimeCharging(evv.timeChargingHH , evv.timeChargingMM, evv.timeChargingSS))
  {
    debugln("failed to get time charging from station");
    evv.timeChargingHH = 0;
    evv.timeChargingMM = 0;
    evv.timeChargingSS = 0;
    //    debug("Time charging Hours = "); debugln(evv.timeChargingHH);
    //    debug("Time charging Minutes = "); debugln(evv.timeChargingMM);
    //    debug("Time charging Second = "); debugln(evv.timeChargingSS);
    timeWAnotGet = false;
  }
  else {
    timeWAnotGet = true;
    //    if(evv.timeChargingSS == 59 && evv.timeChargingMM != 0){
    //      evv.timeChargingMM = evv.timeChargingMM - 1;
    //    }
    if (evv.timeChargingSS == 56 || evv.timeChargingSS == 55 || evv.timeChargingSS == 54 ) {
      durationMMTemp = evv.timeChargingMM;
    }
    else if (evv.timeChargingSS > 56) {
      //      tempHH = durationHourTemp;
      evv.timeChargingMM = durationMMTemp;
    }
    else {
      evv.timeChargingMM = evv.timeChargingMM;
    }
    //    else if (evv.timeChargingMM > 0 && evv.timeChargingSS == 0) {
    //      tempMM = durationMMTemp + 1;
    //    }
    //    else {
    //      tempMM = evv.timeChargingMM;
    //    }
    //    evv.timeChargingHH = 99;
    //    evv.timeChargingMM = 99;
    //    evv.timeChargingSS = 99;
    //    debug("Time charging Hours = "); debugln(evv.timeChargingHH);
    //    debug("Time charging Minutes = "); debugln(evv.timeChargingMM);
    //    debug("Time charging Second = "); debugln(evv.timeChargingSS);
  }
}

#if EVCONTROL == 1
#ifdef __cplusplus

extern "C" {

#endif

uint8_t temprature_sens_read();
#ifdef __cplusplus
}

#endif

uint8_t temprature_sens_read();

float evGetTemp() {
  swDelay(10);

  float evTemp = ((temprature_sens_read() - 32) / 1.8) - TEMPPHOENIXOFFSET;
  Serial.print("///////////////////////////////////////////////////////////////////////////////////////////////////////////// TEMPTEMPTEMP : ");
  Serial.println(temprature_sens_read());
  return evTemp;
}
#else
float evGetTemp() {
  swDelay(10);

  float evTemp;

  if (!ev.getTemp(evTemp)) {
    debugln("failed to get temperatur from station");
  }

  return evTemp;
}
#endif

void evAccidentReset(uint8_t loadVehicleStatus, uint8_t loadCurrent) {
  if (evUserGetCurrent() > 32) {
    evUserSetCurrent(loadCurrent);
  }
  else if (evUserGetCurrent() <= 32) {
    evUserSetCurrent(loadCurrent);
  }
  //  if (loadVehicleStatus == 4) {
  //    evOpenCharge("OFF");
  //  }
  //  if (evCarGetStat() == 4) {
  //
  //  }
  //  evlastStat =
  evlastStat = evCarGetStat();
}
uint8_t countReadMeter;
void evLogic(uint8_t lastVehicleStatus, uint8_t VehicleStatus) {
  //  Serial.print("Setup: Executing on core evLogic : ");
  //  Serial.println(xPortGetCoreID());
  //  if (!isScanning) {
  swDelay(15);
  readmeter(config.phaseset);
  swDelay(100);
#if EVCONTROL == 1

  while ((electric.energy == 0) && (countReadMeter < 10)) {
    swDelay(100);
    readmeter(config.phaseset);
    debugln("come on my energy come on my energy come on my energy come on my energy come on my energy come on my energy come on my energy come on my energy");
    countReadMeter++;
  }
  if ((electric.energy == 0) && (countReadMeter >= 10)) {
    electric.energy = 0.001;
    countReadMeter = 0;
  }
#else
#endif
  swDelay(15);
  evTempShared = evGetTemp();
  swDelay(15);
  //  }
  //  isReadMeterDone = true;
  if (execEvSetCurrent) {
    evUserSetCurrent(currentValue.currentlyAmp);
    debug("Set current : "); debugln(evUserGetCurrent());
    execEvSetCurrent = false;
  }
  if (currentValue.currentlyAmp >  config.currentmax || evUserGetCurrent() >  config.currentmax) {
    evUserSetCurrent(currentValue.currentlyAmp);
  }
  debugln("Before Loop");
  debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////");
  //  debug("Energy : "); debugln(currentValue.ChargeStartEnergy);
  //  debug("EnergyM : "); debugln(electric.energy);
  Serial.println("///////////////////////////////////////////////////////////////// BEFORE");
  Serial.print("startChargeHR : "); Serial.print(currentValue.startChargeHR);
  Serial.print(":"); Serial.print(currentValue.startChargeMN);
  Serial.print(":"); Serial.println(currentValue.startChargeSS);
  Serial.print("NstartChargeHR : "); Serial.print(evv.timeChargingHH);
  Serial.print(":"); Serial.print(evv.timeChargingMM);
  Serial.print(":"); Serial.println(evv.timeChargingSS);
  Serial.println("/////////////////////////////////////////////////////////////////////////");
  //  debug("initdone : "); debugln(initDone);

  //  currentValue.
  //  currentValue.
  //  currentValue.

  debug("vehicleStatus : "); debugln(VehicleStatus);
  debug("lastVehicleStatus : "); debugln(lastVehicleStatus);
  debug("currentValue.isStartOrFinish : "); debugln(currentValue.isStartOrFinish);
  debug("currentValue.isChargeSettingDone : "); debugln(currentValue.isChargeSettingDone);
  debug("currentValue.webChargeStatus : "); debugln(currentValue.webChargeStatus);
  debug("currentValue.isPressedC : "); debugln(currentValue.isPressedC);
  debug("Car current : "); debugln(evUserGetCurrent());
  debugln();
  debug("schtime.schflg : "); debugln(schtime.schflg);
  debug("schtime.schalw : "); debugln(schtime.schalw);
  //  Serial.print("vehicleStatus : "); Serial.println(VehicleStatus);
  //  Serial.print("lastVehicleStatus : "); Serial.println(lastVehicleStatus);
  //  //  Serial.print("schtime.schflg : "); Serial.println(schtime.schflg);
  //  //  Serial.print("schtime.schalw : "); Serial.println(schtime.schalw);
  //  Serial.print("currentValue.scheduleStart : "); Serial.println(currentValue.scheduleStart);
  debug("currentValue.scheduleStart : "); debugln(currentValue.scheduleStart);
  debug("coil status : "); debugln(evGetCoil());
  debug("Electric Energy : "); debugln(electric.energy);
  //  debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////");
  //Serial.println("////////////////////////////////////////////////////////////////////////////////////////////////////////");
  //  //  debugln();
  //  debugln();

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////add history
  if (initDone && (evv.timeChargingHH != 0 || evv.timeChargingMM != 0 || evv.timeChargingSS != 0) && (currentValue.startChargeHR == 0 && currentValue.startChargeMN == 0 && currentValue.startChargeSS == 0 )) {

    char buf[24];

    gcvt(electric.energy, 7, buf);
    //currentValue.ChargeStartEnergy = String(electric.energy);
    currentValue.ChargeStartEnergy = String(buf);

    currentValue.startYR = timetime.yearNow;
    currentValue.startMT = timetime.monthNow;
    currentValue.startDD = timetime.dateNow;
    currentValue.startHH = timetime.hoursNow;
    currentValue.startMN = timetime.minuiteNow;
    currentValue.startSS = timetime.secoundNow;
    //    printf("///DEBUG////////////////////////////////// %d/%d/%d -> %d:%d:%d //////////////////////////////////DEBUG///", currentValue.startYR, currentValue.startMT, currentValue.startDD, currentValue.startHH, currentValue.startMN, currentValue.startSS);
    //    printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
    printf("History Start -> Start Energy : %0.4f\n", atof(currentValue.ChargeStartEnergy.c_str()));
    //    printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");


    debugln("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh");
  }
  else if (initDone && (evv.timeChargingHH == 0 && evv.timeChargingMM == 0 && evv.timeChargingSS == 0) && (currentValue.startChargeHR != 0 || currentValue.startChargeMN != 0 || currentValue.startChargeSS != 0 )) {
    //else if (initDone && electric.energy != 0 && (evv.timeChargingHH == 0 && evv.timeChargingMM == 0 && evv.timeChargingSS == 0) && (currentValue.startChargeHR != 0 || currentValue.startChargeMN != 0 || currentValue.startChargeSS != 0 )) {
    //  else if (electric.energy != 0 && (evv.timeChargingHH == 0 && evv.timeChargingMM == 0 && evv.timeChargingSS == 0) && (currentValue.startChargeHR != 0 || currentValue.startChargeMN != 0 || currentValue.startChargeSS != 0 )) {
    //if (electric.energy < 0.01) {
    //}
    //else {
    addHistory(currentValue.startYR, currentValue.startMT, currentValue.startDD, currentValue.startHH, currentValue.startMN, currentValue.startSS, currentValue.startChargeHR, currentValue.startChargeMN, currentValue.startChargeSS, currentValue.RfidUseFname, currentValue.RfidUseLname, currentValue.RfidUseUid, atof(currentValue.ChargeStartEnergy.c_str()), jsonHistory);
    //}
    currentValue.startChargeHR = 0;
    currentValue.startChargeMN = 0;
    currentValue.startChargeSS = 0;

    if (lastVehicleStatus == 3 || lastVehicleStatus == 0) {
      if (config.rfidusing == 0) {
        currentValue.RfidUseFname = "Non RFID";
        currentValue.RfidUseLname = "Mode";
      }
      else if (config.rfidusing == 1) {
        currentValue.RfidUseFname = "RFID";
        currentValue.RfidUseLname = "Mode";
      }
    }
    //    printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
    //    if(config.rfidusing == 0){
    //      currentValue.RfidUseFname = "Non RFID";
    //      currentValue.RfidUseLname = "Mode";
    //    }
    //    else if(config.rfidusing == 1){
    //      currentValue.RfidUseFname = "RFID";
    //      currentValue.RfidUseLname = "Mode";
    //    }
    printf("History End -> End Energy : %0.4f, Start Energy : %0.4f\n", electric.energy, atof(currentValue.ChargeStartEnergy.c_str()));
    //    printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
    debugln("pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp");
  }
  ///////////////////////////////////////////////////////////////////////////////////////////// button function
  //  if (((lastVehicleStatus == 0 && VehicleStatus == 0) || (lastVehicleStatus == 1 && VehicleStatus == 1) || (lastVehicleStatus == 2 && VehicleStatus == 2)) && (currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1)) {

  if (((lastVehicleStatus == 0 && VehicleStatus == 0) || (lastVehicleStatus == 1 && VehicleStatus == 1) || (lastVehicleStatus == 2 && VehicleStatus == 2) || (lastVehicleStatus == 4 && VehicleStatus == 4)) && (currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1)) {
    execButtonFunc = true;
    if ((lastVehicleStatus == 0 && VehicleStatus == 0) && config.isInSleep == 1) {
      execSleepWake = true;
    }
    else {
      execSleepWake = false;
    }
    if ((lastVehicleStatus == 0 && VehicleStatus == 0) || (lastVehicleStatus == 1 && VehicleStatus == 1)) {
      execHardButton = false;
      execFactoryReset = true;
      execLedButtonBlink = false;
#if boardColour == 2
#else
      digitalWrite(chargeLedPin, LOW);
#endif
      rfidAutenChargeNowButton = 0;
      currentValue.isPressedC = false;

      execRfidAuthenChargeButton = false;
      if (config.rfidusing == 0) {
        currentValue.RfidUseFname = "Non RFID";
        currentValue.RfidUseLname = "Mode";
      }
      else if (config.rfidusing == 1) {
        currentValue.RfidUseFname = "RFID";
        currentValue.RfidUseLname = "Mode";
      }
    }
    else if (((lastVehicleStatus == 2 && VehicleStatus == 2) || (lastVehicleStatus == 4 && VehicleStatus == 4))) { //authen all
      //    else if (((lastVehicleStatus == 2 && VehicleStatus == 2) || (lastVehicleStatus == 4 && VehicleStatus == 4)) && currentValue.webChargeStatus != 6) { //dont authen on schedule
      //      if (config.rfidusing == 1 && !isOpenWeb) { // only out web, authen
      if (config.rfidusing == 1) { //always authen
        execRfidAuthenChargeButton = true;
        //        execLedButtonBlink = true;
        //        execLedButtonBlink = false; ////might use

        if (rfidAutenChargeNowButton == 1) {

          execRfidAuthenChargeButton = false;
          if (currentValue.webChargeStatus == 6) {
            execLedButtonBlink = true;
            execHardButton = false;
          }
          else if (currentValue.webChargeStatus != 6) {
#if boardColour == 2
            if (isInWebSite > TIMEFORNONWEB) {
              currentValue.isPressedC = true;
              currentValue.isChargeSettingDone = true;
              debugln("pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed");
            }
#else
            digitalWrite(chargeLedPin, HIGH);
#endif
            execLedButtonBlink = false;
            execHardButton = true;
          }
          debugln("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
          if (lastVehicleStatus == 4 && VehicleStatus == 4 && currentValue.webChargeStatus != 6) {
            evOpenCharge("ON");
            currentValue.isPressedC = true;
            currentValue.isChargeSettingDone = true;
            debugln("enable coil : wait car mode : again :02");
          }
        }
        else if (rfidAutenChargeNowButton == 0) {
          execRfidAuthenChargeButton = true;
          rfidReadDone = false;
#if boardColour == 2
#else
          digitalWrite(chargeLedPin, LOW);
#endif
          debugln("sssssssssssssssssssssssssssssssssssssssssssssss");
        }
      }
      else if (config.rfidusing == 0) {
        execHardButton = true;
        execRfidAuthenChargeButton = false;
        //        execLedButtonBlink = true;
        execLedButtonBlink = false;
        rfidAutenChargeNowButton = 0;
        if (!schtime.schflg) {
#if boardColour == 2
          if (isInWebSite > TIMEFORNONWEB) {
            currentValue.isPressedC = true;
            currentValue.isChargeSettingDone = true;
            debugln("pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed");
          }
#else
          digitalWrite(chargeLedPin, HIGH);
#endif
          debugln("isudfsdkfsdkfsjdfksjdfksjdfksdjfksjhdfoauehdasjd");
        }
        else if (schtime.schflg) {
          //          digitalWrite(chargeLedPin, HIGH);
          if (currentValue.isChargeSettingDone) {
            execHardButton = false;
            execLedButtonBlink = true;
            debugln("iurneovifermcfwamomekopfcmwekmfcwoeffceowp");
          }
          else if (!currentValue.isChargeSettingDone) {
            execHardButton = true;
            execLedButtonBlink = false;
#if boardColour == 2
#else
            digitalWrite(chargeLedPin, HIGH);
#endif
            if (schtime.clearSchedule) {
              debugln("riulcileurcnhleircnhirdhsrfcftujytrc");
            }
            else {
              if (isInWebSite > TIMEFORNONWEB) {
                currentValue.isPressedC = true;
                currentValue.isChargeSettingDone = true;
                debugln("pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed pressed");
              }
            }
          }

        }

      }
      execFactoryReset = false;
      //      debug("execHardButton"); debugln(execHardButton);
      //      debug("execRfidAuthenChargeButton"); debugln(execRfidAuthenChargeButton);
      //      debug("execLedButtonBlink"); debugln(execLedButtonBlink);
      //      debug("rfidAutenChargeNowButton"); debugln(rfidAutenChargeNowButton);
    }
  }
  else if ((lastVehicleStatus == 2 || lastVehicleStatus == 3) && ( VehicleStatus == 0 || VehicleStatus == 1 ) && (currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1)) {
    if (config.rfidusing == 1) {
      execRfidAuthenChargeButton = false;
      rfidAutenChargeNowButton = 0;
      rfidReadDone = true;
      debugln("oooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
    }
  }
  else if (lastVehicleStatus == 3 && VehicleStatus == 3 &&  currentValue.isStartOrFinish == 2) {
    execLedButtonBlink = true; // blink
    //    execLedButtonBlink = false;
    //      debug("execHardButton"); debugln(execHardButton);
    //      debug("execRfidAuthenChargeButton"); debugln(execRfidAuthenChargeButton);
    //      debug("execLedButtonBlink"); debugln(execLedButtonBlink);
    //      debug("rfidAutenChargeNowButton"); debugln(rfidAutenChargeNowButton);
  }
  else if (lastVehicleStatus == 3 && VehicleStatus == 3 &&  (currentValue.isStartOrFinish == 4 && currentValue.isStartOrFinish != 2)) {
    execLedButtonBlink = false; // blink
    //    execLedButtonBlink = false;
    //      debug("execHardButton"); debugln(execHardButton);
    //      debug("execRfidAuthenChargeButton"); debugln(execRfidAuthenChargeButton);
    //      debug("execLedButtonBlink"); debugln(execLedButtonBlink);
    //      debug("rfidAutenChargeNowButton"); debugln(rfidAutenChargeNowButton);
  }
  else {
    execButtonFunc = false;
    execHardButton = false;
    execFactoryReset = false;
    execLedButtonBlink = false;
#if boardColour == 2
#else
    digitalWrite(chargeLedPin, LOW);
#endif
  }

  //////////////////////////////////////////////////////////////////////////////////////////// logic start : setting flag : button flag : schedule flag : always flag :
  if (((lastVehicleStatus == 0 && VehicleStatus == 0) || (lastVehicleStatus == 1 && VehicleStatus == 1)) && (currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1)) {
    //    isActive++;
#if boardColour == 2
    if (!startAOffCoil) {
      swDelay(10);
      evOpenCharge("OFF");
      debugln("off off off off off off off off off off off off off off ");
      startAOffCoil = true;
    }
#endif
    currentValue.isPressedC = false;
    ////// Steady stage A1 : don't submit setting : standby mode
    if (!currentValue.isChargeSettingDone) {
      currentValue.webChargeStatus = 0;
      currentValue.isStartOrFinish = 0;
      debugln("stand by mode");
    }
    ////// Change stage A1 : do submit setting : standby mode
    else if (currentValue.isChargeSettingDone) {
      currentValue.isStartOrFinish = 1;
      if (!schtime.schflg) {
        currentValue.webChargeStatus = 0;
        debugln("stand by mode : ready to next : not schedule");
        Serial.println(F("stand by mode : ready to next : not schedule"));
      }
      else if (schtime.schflg) {
        if (!schtime.schalw) {
          currentValue.webChargeStatus = 6;
          if (currentValue.scheduleStart) {
            debugln("just connect car to complete your schedule : once");
            Serial.println(F("just connect car to complete your schedule : once"));
          }
          else if (!currentValue.scheduleStart) {
            setSchedule(schtime.schhr, schtime.schmm);
            /////schedule once start : this func will on button
            debug("hours : "); debug(schtime.schhr); debug("  minute : "); debug(schtime.schmm);
            debugln("stand by mode : ready to next : once schedule : A : 1");
            Serial.println(F("stand by mode : ready to next : once schedule : A : 1"));
          }
        }
        else if (schtime.schalw) {
          currentValue.webChargeStatus = 6;
          if (currentValue.scheduleStart) {
            debugln("just connect car to complete your schedule : daily");
            Serial.println(F("just connect car to complete your schedule : daily"));
          }
          else if (!currentValue.scheduleStart) {
            setSchedule(schtime.schhr, schtime.schmm);
            /////schedule once start : this func will on button
            debug("hours : "); debug(schtime.schhr); debug("  minute : "); debug(schtime.schmm);
            debugln("stand by mode : ready to next : daily schedule : A : 2");
            Serial.println(F("stand by mode : ready to next : daily schedule : A : 2"));
          }
        }
      }
    }
  }
  else if (lastVehicleStatus == 2 && VehicleStatus == 2 && currentValue.isStartOrFinish == 2 && currentValue.isPressedC) {
    swDelay(20);
    evOpenCharge("ON");
    debugln("enable coil : wait car mode : again : 02");
  }
  else if (lastVehicleStatus == 0 && VehicleStatus == 2 && currentValue.isStartOrFinish == 1 && currentValue.isChargeSettingDone) {
    ////// Change stage A1 to B1 : do submit setting : wait coil mode : no schedule
    debugln("Change stage A1 to B1 : do submit setting : wait coil mode : no schedule : you just connect plug");
    Serial.println(F("Change stage A1 to B1 : do submit setting : wait coil mode : no schedule : you just connect plug"));
  }
  else if (lastVehicleStatus == 2 && VehicleStatus == 0 && (currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1 )) {
    //    currentValue.webChargeStatus = 1;
    currentValue.isStartOrFinish = 0;
    debugln("Back to standby mode from 2 to 0");
    if (schtime.schflg) {
      currentValue.webChargeStatus = 6;
    }
    else if (!schtime.schflg) {
      currentValue.webChargeStatus = 1;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  else if (lastVehicleStatus == 4 && VehicleStatus == 4 && currentValue.isStartOrFinish == 2 && currentValue.isPressedC) {
    evOpenCharge("ON");
    debugln("enable coil : wait car mode : again :02");
    //    printf("something ocurr : lastVehicleStatus == 4 && VehicleStatus == 4 && currentValue.isStartOrFinish == 2 && currentValue.isPressedC");
  }
  else if (lastVehicleStatus == 0 && VehicleStatus == 4 && currentValue.isStartOrFinish == 1 && currentValue.isChargeSettingDone) {
    ////// Change stage A1 to B1 : do submit setting : wait coil mode : no schedule
    debugln("Change stage A1 to B1 : do submit setting : wait coil mode : no schedule : you just connect plug");
  }
  else if (lastVehicleStatus == 4 && VehicleStatus == 0 && (currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1 )) {
    currentValue.isStartOrFinish = 0;
    debugln("Back to standby mode from 2 to 0");
    if (schtime.schflg) {
      currentValue.webChargeStatus = 6;
    }
    else if (!schtime.schflg) {
      currentValue.webChargeStatus = 1;
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  else if (((lastVehicleStatus == 4 && VehicleStatus == 4) || (lastVehicleStatus == 2 && VehicleStatus == 2)) && (currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1)) {
    ////// Steady stage B1 : don't submit setting : standby mode : plug before setting
    if (!currentValue.isChargeSettingDone) {
      //      currentValue.webChargeStatus = 0;// old thing
      currentValue.webChargeStatus = 1;//new status logic
      currentValue.isStartOrFinish = 0;
      debugln("stand by mode : connect plug but dont submit your charge");
      //      Serial.println(F("stand by mode : connect plug but dont submit your charge"));
    }
    ////// Change stage B1 : do submit setting : standby mode : plug before setting
    else if (currentValue.isChargeSettingDone) {
      currentValue.isStartOrFinish = 1;
      if (!schtime.schflg) {
        if (!currentValue.isPressedC) {
          if (VehicleStatus == 3) {
            evOpenCharge("ON");
            debugln("enable coil : wait car mode");
            currentValue.webChargeStatus = 2;
            currentValue.isStartOrFinish = 2;
            if (simulateCar) {
              simulateCarValue = 3;
            }
          }
          else {
            currentValue.webChargeStatus = 1;
            debugln("stand by mode : ready to next : not schedule");
            Serial.println(F("stand by mode : ready to next : not schedule"));
          }
        }
        else if (currentValue.isPressedC) {
          evOpenCharge("ON");
          rfidAutenChargeNowButton = 0;
          debugln("enable coil : wait car mode");
          currentValue.webChargeStatus = 2;
          currentValue.isStartOrFinish = 2;
          if (simulateCar) {
            if (simulateCarValue == 2) {
              simulateCarValue = 3;
            }
            else if (simulateCarValue == 0) {
              simulateCarValue = 1;
            }
            else if (simulateCarValue == 4) {
              simulateCarValue = 5;
            }
          }
        }
      }
      else if (schtime.schflg) {
        if (!currentValue.scheduleStart) {
          if (!schtime.schalw) {
            currentValue.webChargeStatus = 6;
            setSchedule(schtime.schhr, schtime.schmm);
            /////schedule once start : this func will on button
            debug("hours : "); debug(schtime.schhr); debug("  minute : "); debug(schtime.schmm);
            debugln("stand by mode : ready to next : once schedule");
            Serial.println(F("stand by mode : ready to next : once schedule"));
          }
          else if (schtime.schalw) {
            currentValue.webChargeStatus = 6;
            setSchedule(schtime.schhr, schtime.schmm);
            /////schedule always start : this func will on button
            debug("hours : "); debug(schtime.schhr); debug("  minute : "); debug(schtime.schmm);
            debugln("stand by mode : ready to next : daily schedule");
            Serial.println(F("stand by mode : ready to next : daily schedule"));
          }
        }
        else if (currentValue.scheduleStart) {
          if (!currentValue.isPressedC) {
            currentValue.webChargeStatus = 6;
            //            execLedButtonBlink = true;
            debug("hours : "); debug(schtime.schhr); debug("  minute : "); debug(schtime.schmm); debug("  currentValue.scheduleStart : "); debugln(currentValue.scheduleStart);
            debugln("stand by mode : ready to next : wait schedule : 3");
            //            printf("N Now : %d, Last : %d \n", VehicleStatus, lastVehicleStatus);
            //            Serial.print("currentValue.isPressedC : "); Serial.println(currentValue.isPressedC);
            Serial.println(F("stand by mode : ready to next : wait schedule : 3"));
            Alarm.delay(1);
            ///////////////////////////////////////////////////////////////////////add new authen schedule
            //            if(rfidAutenChargeNowButton == 0){
            //
            //            }
            ///////////////////////////////////////////////////////////////////////end new authen schedule
          }
          else if (currentValue.isPressedC) {
            evOpenCharge("ON");
            debugln("enable coil : wait car mode");
            currentValue.webChargeStatus = 2;
            currentValue.isStartOrFinish = 2;
            execLedButtonBlink = true;
            //            printf("P Now : %d, Last : %d \n", VehicleStatus, lastVehicleStatus);
            //            printf("something ocurr : PRESS \n");
            if (simulateCar) {
              simulateCarValue = simulateCarValue + 1;
            }
          }
        }
      }
    }
  }
  else if (lastVehicleStatus == 3 && VehicleStatus == 3 && currentValue.isStartOrFinish == 2) {
    ////// Steady stage B2 : submit setting : on coil : standby charging mode
    debugln("B2 : enable coil : wait car mode");
    //    execLedButtonBlink = true;

    currentValue.isStartOrFinish == 3;
    currentValue.webChargeStatus = 2;
  }
  else if ((lastVehicleStatus == 3 || lastVehicleStatus == 2) && VehicleStatus == 5 && currentValue.isStartOrFinish == 2) {
    ////// Change stage B2 to C2 : submit setting : on coil : Start Bill
    printf("something ocurr : lastVehicleStatus == 3 || lastVehicleStatus == 2) && VehicleStatus == 5 && currentValue.isStartOrFinish == 2 \n");
    debugln("Change stage B2 to C2 : submit setting : on coil : Start Bill");
    currentValue.isStartOrFinish = 3;
    currentValue.webChargeStatus = 3;
  }

  else if (lastVehicleStatus == 5 && VehicleStatus == 5 && currentValue.isStartOrFinish == 2) {
    ////// Change stage B2 to C2 : submit setting : on coil : Start Bill
    printf("something ocurr : lastVehicleStatus == 5 && VehicleStatus == 5 && currentValue.isStartOrFinish == 2 \n");
    debugln("Change stage B2 to C2 : submit setting : on coil : Start Bill");
    currentValue.isStartOrFinish = 3;
    currentValue.webChargeStatus = 3;
  }

  else if (lastVehicleStatus == 5 && VehicleStatus == 5 && currentValue.isStartOrFinish == 3) {
    ////// Steady stage C2 : submit setting : on coil : Charging
    debugln("Charging!!! Charging!!! Charging!!! Charging!!! Charging!!!");
    //    digitalWrite(chargeLedPin, HIGH);
#if boardColour == 2
#else
    digitalWrite(chargeLedPin, LOW);
#endif
    execLedButtonBlink = false;
    currentValue.isStartOrFinish = 3;
    currentValue.webChargeStatus = 3;
  }
  else if (lastVehicleStatus == 5 && (VehicleStatus == 2 || VehicleStatus == 3) && currentValue.isStartOrFinish == 3) {
    ////// Change stage C2 to B2 : submit setting : on coil : Done Bill
    debugln("Change stage C2 to B2 : submit setting : on coil : start count");
    currentValue.isStartOrFinish = 4;
  }
  else if ((lastVehicleStatus == 2 || lastVehicleStatus == 3) && VehicleStatus == 5 && currentValue.isStartOrFinish == 4) {
    ////// Change stage B2 to C2 : submit setting : on coil : Start Bill
    debugln("Error magnetic furious swap bcbcbcbc");
    currentValue.isStartOrFinish = 3;
    chargingDoneCount = 0;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////Accident
  else if (lastVehicleStatus == 6 && VehicleStatus == 6) {
    debugln("Error Error Error");
    currentValue.webChargeStatus = 7;
  }
  else if (lastVehicleStatus == 6 && VehicleStatus == 4 && currentValue.isStartOrFinish == 3) {
    currentValue.isPressedC = false;
    debugln("open button again");
    currentValue.webChargeStatus = 1;
  }
  else if (lastVehicleStatus == 6 && VehicleStatus == 2 && currentValue.isStartOrFinish == 3) {
    currentValue.isPressedC = false;
    debugln("open button again");
    currentValue.webChargeStatus = 1;
  }
  else if (lastVehicleStatus == 6 && VehicleStatus == 0 && currentValue.isStartOrFinish == 3) {
    debugln("collecet bill || 6:0");
    currentValue.isPressedC = false;

    evOpenCharge("OFF");
    if (config.rfidusing == 0) {
      currentValue.RfidUseFname = "Non RFID";
      currentValue.RfidUseLname = "Mode";
    }
    else if (config.rfidusing == 1) {
      currentValue.RfidUseFname = "RFID";
      currentValue.RfidUseLname = "Mode";
    }
    if (simulateCar) {
      if (simulateCarValue == 5) {
        simulateCarValue = 4;
      }
      else if (simulateCarValue == 3) {
        simulateCarValue = 2;
      }
      else if (simulateCarValue == 1) {
        simulateCarValue = 0;
      }
    }
    debugln("disable coil : back to standby");
    if (schtime.schflg) {  //980
      if (!schtime.schalw) {
        currentValue.webChargeStatus = 0;
        currentValue.scheduleStart = false;
        currentValue.isChargeSettingDone = false;
        currentValue.isStartOrFinish = 0;
        cancelSchedule();
        schtime.schflg = false;
      }
      else if (schtime.schalw) {
        currentValue.isChargeSettingDone = true;
        currentValue.scheduleStart = true;
        currentValue.isStartOrFinish = 1;
        currentValue.webChargeStatus = 6;
      }
    }
    else {
      currentValue.isChargeSettingDone = false;
      currentValue.isStartOrFinish = 0;
      currentValue.webChargeStatus = 0;
    }
  }
  ////////////////////////////////////////////////////////////////////////////////switch
  //  else if (lastVehicleStatus == 0 && VehicleStatus == 4) {
  //    currentValue.isStartOrFinish = 3;
  //    currentValue.isPressedC = true;
  //    currentValue.isChargeSettingDone = true;
  //    debugln("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
  //  }
  /////////////////////////////////////////////////////////////////////////////////////
  else if (((lastVehicleStatus == 2 && VehicleStatus == 2) || (lastVehicleStatus == 4 && VehicleStatus == 4)) && currentValue.isStartOrFinish == 3) {           ////////////////acccident on
    if (!currentValue.isPressedC) {
      currentValue.webChargeStatus = 1;
      debugln("wait coil : wait car mode");
      printf("soomething occur : 224430");
    }
    else {
      evOpenCharge("ON");
      debugln("enable coil : wait car mode");
      printf("soomething occur : 224431");
    }
    if (simulateCar) {
      simulateCarValue = 5;
    }
  }
  else if (lastVehicleStatus == 2 && VehicleStatus == 0 && currentValue.isStartOrFinish == 3) {
    debugln("collecet bill");
    currentValue.webChargeStatus = 0;
    currentValue.isPressedC = false;
    if (schtime.schflg) {
      if (!schtime.schalw) {
        currentValue.scheduleStart = false;
        currentValue.isChargeSettingDone = false;
        currentValue.isStartOrFinish = 0;
        cancelSchedule();
        schtime.schflg = false;
      }
      else if (schtime.schalw) {
        currentValue.scheduleStart = true;
        currentValue.isStartOrFinish = 1;
      }
    }
    else {
      currentValue.isChargeSettingDone = false;
      currentValue.isStartOrFinish = 0;
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////finish
  //|| (lastVehicleStatus == 0 && VehicleStatus == 0)
  //  else if ((lastVehicleStatus == 3 && VehicleStatus == 3)  && (currentValue.isStartOrFinish == 4 || currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1)) {
  else if ((lastVehicleStatus == 3 && VehicleStatus == 3)  && currentValue.isStartOrFinish == 4) {
    ///// Steady stage B2 : submit setting : on coil : Charging done : FINISH
    if (chargingDoneCount == FinishSec) {
      debugln("Finish!!! Finish!!! Finish!!! Finish!!! Finish!!!");
      //      printf("currentValue.RfidUseFname : %s, currentValue.RfidUseLname : %s", currentValue.RfidUseFname, currentValue.RfidUseLname);
      debugln("/////////////////////////////////////////////////////////////////////////////////////////////////");
      debugln(currentValue.RfidUseFname);
      debugln(currentValue.RfidUseLname);
      debugln("/////////////////////////////////////////////////////////////////////////////////////////////////");
#if boardColour == 2
#else
      digitalWrite(chargeLedPin, LOW); //add 26/7/2022
#endif
      currentValue.webChargeStatus = 4;
      currentValue.isStartOrFinish = 5;
      currentValue.isPressedC = false;
      //      execLedButtonBlink = true;
      execLedButtonBlink = false;
      if (schtime.schflg) {
        if (!schtime.schalw) {
          currentValue.scheduleStart = false;
          currentValue.isChargeSettingDone = false;
          cancelSchedule();
          schtime.schflg = false;
        }
        else if (schtime.schalw) {
          currentValue.scheduleStart = true;
        }
      }
      else {
        currentValue.isChargeSettingDone = false;
      }
      chargingDoneCount = 0;
    }
    else {
#if boardColour == 2
#else
      digitalWrite(chargeLedPin, LOW);
#endif
      execLedButtonBlink = false;
      debugln("Almost Finish");
      debugln("/////////////////////////////////////////////////////////////////////////////////////////////////");
      debugln(currentValue.RfidUseFname);
      debugln(currentValue.RfidUseLname);
      debugln("/////////////////////////////////////////////////////////////////////////////////////////////////");
      //      printf("currentValue.RfidUseFname : %s, currentValue.RfidUseLname : %s", currentValue.RfidUseFname, currentValue.RfidUseLname);
      currentValue.webChargeStatus = 3;
      currentValue.isStartOrFinish = 4;
      chargingDoneCount++;
    }
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  else if ((lastVehicleStatus == 3 || lastVehicleStatus == 5 ) &&  VehicleStatus == 0 && currentValue.isStartOrFinish == 0 && currentValue.webChargeStatus == 6) {
    schtime.schflg = true;
    debugln("qowncdifcnhowiahncsviuefcoseic");
  }
  else if ((lastVehicleStatus == 3 || lastVehicleStatus == 5 ) && (VehicleStatus == 0 || VehicleStatus == 1) && (currentValue.isStartOrFinish == 3 || currentValue.isStartOrFinish == 4)) {
    //    currentValue.webChargeStatus = 4;
    currentValue.webChargeStatus = 0;
    currentValue.isStartOrFinish = 5;
    currentValue.isPressedC = false;
    if (schtime.schflg) {
      if (!schtime.schalw) {
        currentValue.scheduleStart = false;
        currentValue.isChargeSettingDone = false;
        cancelSchedule();
        schtime.schflg = false;
        debugln("dasdoihaisdygaisudhasuhdiaushdiasdjipfridoushoejiohjrfuwdh");
      }
      else if (schtime.schalw) {
        currentValue.scheduleStart = true;
        debugln("sdsfsluedow8ejfowe9jfwofwoeifjwoeijf");
      }
    }
    else {
      currentValue.isChargeSettingDone = false;
      debugln("iwuehdowaiksmdnkdsuhjliskdcnkhsduhcolic");
    }
    chargingDoneCount = 0;
  }

  else if (lastVehicleStatus == 3 &&  VehicleStatus == 3 && (currentValue.isStartOrFinish == 0 || currentValue.isStartOrFinish == 1)) {
    currentValue.webChargeStatus = 4;
#if boardColour == 2
#else
    digitalWrite(chargeLedPin, LOW); //add 26/7/2022
#endif
    debugln("pyu9okfdmslccdjbgfidkslmfjdmlkfs");
  }

  else if (lastVehicleStatus == 0 && VehicleStatus == 0 && currentValue.isStartOrFinish == 5) {
    currentValue.webChargeStatus = 0;
    //    if (chargingDoneCount == 0) {
#if boardColour == 2
#else
    digitalWrite(chargeLedPin, LOW);
#endif
    evOpenCharge("OFF"); //1115
    currentValue.currentlyAmp = config.currentmax;
    printf("////////////////////////Set current to current max : %d\n", currentValue.currentlyAmp );
    swDelay(30);
    evUserSetCurrent(currentValue.currentlyAmp);
    if (config.rfidusing == 0) {
      currentValue.RfidUseFname = "Non RFID";
      currentValue.RfidUseLname = "Mode";
    }
    else if (config.rfidusing == 1) {
      currentValue.RfidUseFname = "RFID";
      currentValue.RfidUseLname = "Mode";
    }
    debugln("disable coil : back to standby : force unplug");
    //    }
    //    if (chargingDoneCount == 3) {
    //      currentValue.webChargeStatus = 0;
    if (!schtime.schalw) {
      currentValue.isStartOrFinish = 1;
    }
    else {
      currentValue.isStartOrFinish = 0;
    }
    //      chargingDoneCount = 0;
    //    }
    //    else {
    //      chargingDoneCount++;
    //    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  else if (lastVehicleStatus == 3 && VehicleStatus == 3 && currentValue.isStartOrFinish == 5) {
    ///// Steady stage B2 : clear setting except always : on coil : Charging done : After finish

    //    if (chargingDoneCount == 3) {
    //      currentValue.webChargeStatus = 0;
    if (schtime.schflg) {
      if (!schtime.schalw) {
        currentValue.webChargeStatus = 6;
        currentValue.isStartOrFinish = 1;
      }
      else {
        currentValue.webChargeStatus = 4;
        currentValue.isStartOrFinish = 0;
      }
    }

    else if (!schtime.schflg) {
      currentValue.webChargeStatus = 4;
      currentValue.isStartOrFinish = 0;
    }

    //    chargingDoneCount = 0;
    //    }
    //    else {
    //      chargingDoneCount++;
    //    }
  }
  else if ((lastVehicleStatus == 3 || lastVehicleStatus == 5) && (VehicleStatus == 1 || VehicleStatus == 0) && currentValue.isStartOrFinish != 4) {
    ///// Steady stage B2 : submit setting : on coil : Charging done : After finish
    //    currentValue.isStartOrFinish = 0;
#if boardColour == 2
#else
    digitalWrite(chargeLedPin, LOW);
#endif
    evOpenCharge("OFF");
    currentValue.currentlyAmp = config.currentmax;
    printf("////////////////////////Set current to current max : %d\n", currentValue.currentlyAmp );
    swDelay(30);
    evUserSetCurrent(currentValue.currentlyAmp);
    //    if (config.rfidusing == 0) {
    //      currentValue.RfidUseFname = "Non RFID";
    //      currentValue.RfidUseLname = "Mode";
    //    }
    //    else if (config.rfidusing == 1) {
    //      currentValue.RfidUseFname = "RFID";
    //      currentValue.RfidUseLname = "Mode";
    //    }
    if (simulateCar) {
      if (simulateCarValue == 5) {
        simulateCarValue = 4;
      }
      else if (simulateCarValue == 3) {
        simulateCarValue = 2;
      }
      else if (simulateCarValue == 1) {
        simulateCarValue = 0;
      }
    }
    debugln("////////////////////////////////////////////////////////////////////////////////////////////////////disable coil : back to standby");
    if (!schtime.schalw) {
      currentValue.isStartOrFinish = 1;
      currentValue.webChargeStatus = 0;
      schtime.schflg = false;
      currentValue.isChargeSettingDone = false;
      cancelSchedule();
    }
    else {
      currentValue.isStartOrFinish = 0;
      currentValue.webChargeStatus = 6;
      schtime.schflg = true;
      currentValue.isChargeSettingDone = true;
      setSchedule(schtime.schhr, schtime.schmm);
      /////schedule always start : this func will on button
      debug("hours : "); debug(schtime.schhr); debug("  minute : "); debug(schtime.schmm);
      debugln("stand by mode : ready to next : daily schedule");
    }
  }
  else if ((lastVehicleStatus == 0 && VehicleStatus == 0) && currentValue.isStartOrFinish == 4) {
    if (!schtime.schalw) {
      currentValue.isStartOrFinish = 1;
      currentValue.webChargeStatus = 0;
    }
    else {
      currentValue.isStartOrFinish = 0;
      currentValue.webChargeStatus = 6;
    }
    debugln("vnuiwhriuwoejmvnwevwiejvweojvpwejvniwuehv");
  }

  else if (lastVehicleStatus == 5 && VehicleStatus == 5 && currentValue.isStartOrFinish == 0) {
    ////////////////////////////////////unexpect charging
    currentValue.isStartOrFinish = 3;
    currentValue.webChargeStatus = 3;
    currentValue.isChargeSettingDone = true;
    currentValue.isPressedC = true;
  }
  //  else if (lastVehicleStatus == 0 && VehicleStatus == 4) {
  //    ////////////////////////////////////unexpect charging
  //    currentValue.isStartOrFinish = 3;
  //    currentValue.webChargeStatus = 3;
  //    currentValue.isChargeSettingDone = true;
  //    currentValue.isPressedC = true;
  //  }
  else if (lastVehicleStatus == 0 && VehicleStatus == 0 && currentValue.isStartOrFinish == 3) {
    currentValue.isStartOrFinish = 0;
  }

  DateTime now = rtc.now(); // Get current time
  uint8_t ss;
  uint8_t mm;
  uint16_t hh;
  //  uint8_t dd;
  //  uint8_t mt;
  //  uint16_t yy;
  //
  ss = now.second();
  mm = now.minute();
  hh = now.hour();

  if (schtime.schflg && currentValue.webChargeStatus == 6 && currentValue.scheduleStart) {
    if (schtime.schhr == hh && schtime.schmm == mm && (ss == 1 || ss == 2 || ss == 3)) {
      debugln("///////////////////////////////////////////////////////////////////start schedule Force");
      //      Serial.println(F("///////////////////////////////////////////////////////////////////start schedule Force"));
      currentValue.isPressedC = true;
      if (evlastStat == 0 || evlastStat == 1) {
        if (!schtime.schalw) {
          swDelay(15);
          cancelSchedule();
          currentValue.isChargeSettingDone = false;
          currentValue.isPressedC = false;
          schtime.schflg = false;
          currentValue.webChargeStatus = 0;
          //          Serial.println(F("///////////////////////////////////////////////////////////////////start schedule : failed : no daily Force"));
          debugln("///////////////////////////////////////////////////////////////////start schedule : failed : no daily Force");
        }
        else if (schtime.schalw) {
          //      cancelSchedule();
          swDelay(15);
          currentValue.isPressedC = false;
          currentValue.webChargeStatus = 6;
          //      currentValue.isChargeSettingDone = true
          swDelay(15);
          setSchedule(schtime.schhr, schtime.schmm);
          //          Serial.println(F("///////////////////////////////////////////////////////////////////start schedule : failed : do daily Force"));
          debugln("///////////////////////////////////////////////////////////////////start schedule : failed : do daily Force");
        }
      }
      if (config.rfidusing == 1) {
        if (evlastStat == 2 &&  rfidAutenChargeNowButton == 0) {
          if (!schtime.schalw) {
            swDelay(15);
            cancelSchedule();
            currentValue.isChargeSettingDone = false;
            currentValue.isPressedC = false;
            schtime.schflg = false;
            currentValue.webChargeStatus = 1;
            //            Serial.println(F("///////////////////////////////////////////////////////////////////start schedule : failed : no daily Force"));
            debugln("///////////////////////////////////////////////////////////////////start schedule : failed : no daily Force");
          }
          else if (schtime.schalw) {
            //      cancelSchedule();
            swDelay(15);
            currentValue.isPressedC = false;
            currentValue.webChargeStatus = 6;
            //      currentValue.isChargeSettingDone = true
            swDelay(15);
            setSchedule(schtime.schhr, schtime.schmm);
            //            Serial.println(F("///////////////////////////////////////////////////////////////////start schedule : failed : do daily Force"));
            debugln("///////////////////////////////////////////////////////////////////start schedule : failed : do daily Force");
          }
        }
      }
    }
  }


  //  if (displayPage != 0 && displayPage != 5) {
  //    DynamicJsonBuffer jsonBuffer;
  //    JsonObject &jsonsend = jsonBuffer.createObject();
  //
  //    jsonsend["Mode"] = currentValue.webChargeStatus;
  //
  //    String output;
  //    jsonsend.printTo(output);
  //    debug("setting send : ");
  //    debugln(output);
  ////    websockets.broadcastTXT(output);
  //  }
  //  lastVehicleStatus = VehicleStatus;
  if (evv.timeChargingHH > 48 || evv.timeChargingMM > 61 || evv.timeChargingSS > 61) {

  }
  else {
    currentValue.startChargeHR = evv.timeChargingHH;
    currentValue.startChargeMN = evv.timeChargingMM;
    currentValue.startChargeSS = evv.timeChargingSS;
  }

  currentValue.carLastStat = evlastStat;
  evlastStat = VehicleStatus;
  //  currentValue.currentlyAmp = evUserGetCurrent();
  currentValue.carNowStat = VehicleStatus;

  saveAccidentOff(jsonAccident, currentValue);
  //  swDelay(10);
  //  isReadMeterDone = false;
}

void evRunning() {
  //  unsigned long currentTimeEV = millis();
  //
  //  if (itIsTime(currentTimeEV, previousTimeEV, eventIntervalEV)) {

  if (!execRfidAuthenChargeButton || !execRfidAdd) {
    ledState(currentValue.webChargeStatus);
  }
  evLogic(evlastStat, evCarGetStat());
#if EVCONTROL == 1
  timeCharging();
#else
  if (evlastStat == 5) {
    do {
      timeCharging();
    } while (!timeWAnotGet);
  }
  else {
    timeCharging();
  }
#endif
  timeConnect();

  //    previousTimeEV = currentTimeEV;
  //  }
  swDelay(15);
}
