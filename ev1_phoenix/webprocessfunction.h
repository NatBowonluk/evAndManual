void recvFunction (String message);
void sendRandNumber(void);
void simulateMeter(uint8_t phase);
void evStatus(void);
void rfidAuthen(void);

void recvFunction (String message) {
  if (displayPage == 0) {                                            ////////////////////index page
    //    debugln("wait for finish page");
    //{"date":"Monday","day":"20","month":"June","year":"2022","hours":"13","minute":"23","second":"25","isPressedC":"true"}
    DynamicJsonBuffer jsonBuffer;
    JsonObject &doc = jsonBuffer.parseObject(message);

    if (!doc.success()) {
      debugln(F("Failed to read file, using page index"));
    }

    if (doc.containsKey("isPressedC")) {                                                           //////////////////////////////start charging
      currentValue.isPressedC = doc["isPressedC"] | true;
    }
    if (doc.containsKey("userCurrentSet")) {
      currentValue.currentlyAmp = doc["userCurrentSet"] | 32;
      execEvSetCurrent = true;
    }

    ////////{"flagSubmit":false,"alwaysSchedule":false,"flagClear":true,"flagSchedule":false}{"flagSubmit":false,"alwaysSchedule":false,"flagClear":true,"flagSchedule":false}
    if (doc.containsKey("flagClear")) {
      //      debug("Save MAC : ");debugln(currentValue.FirstGuyConnectedMAC);
      //      debug("Now MAC : ");debugln(OneConnectedApMac());
      //      if (doc["flagClear"] == true) {
      clearSchedule = doc["flagClear"] | false;
      if (clearSchedule) {
        currentValue.scheduleStart = false;
        cancelSchedule();
      }

    }
    //      else if (doc["flagClear"] == false) {
    if (doc.containsKey("alwaysSchedule")) {
      schtime.schalw = doc["alwaysSchedule"] | false;
    }
    if (doc.containsKey("flagSchedule")) {
      schtime.schflg = doc["flagSchedule"] | false;
      //      if (schtime.schflg) {
      //        currentValue.isChargeSettingDone = true;
      //      }
      //      else {
      //        currentValue.isChargeSettingDone = false;
      //      }
    }
    if (doc.containsKey("flagSubmit")) {
      currentValue.isChargeSettingDone = doc["flagSubmit"] | true;
    }
    //      }
    //    }

    if (doc.containsKey("day")) {
      chargeTimeWeb.dateStart = doc["day"] | 0;
    }
    if (doc.containsKey("month")) {
      chargeTimeWeb.monthStart = monthNumber(doc["month"]);
    }
    if (doc.containsKey("schedule")) {
      String scheduleBuff = doc["schedule"] | "0";
      schtime.schhr = getValue(scheduleBuff, ':', 0).toInt();
      schtime.schmm = getValue(scheduleBuff, ':', 1).toInt();
      schtime.schfulltext = scheduleBuff;

      currentValue.FirstGuyConnectedMAC = OneConnectedApMac();
    }
    if (doc.containsKey("year")) {
      chargeTimeWeb.yearStart = doc["year"] | 0;
    }
    if (doc.containsKey("hours")) {
      chargeTimeWeb.hoursStart = doc["hours"] | 0;
    }
    if (doc.containsKey("minute")) {
      chargeTimeWeb.minuiteStart = doc["minute"] | 0;
    }
    if (doc.containsKey("second")) {
      chargeTimeWeb.secoundStart = doc["second"] | 0;
    }

    //    if (doc.containsKey("flagSetTime")) {
    //      execSetTime = doc["flagSetTime"] | false;
    //      if (doc["hours"] == 0 && doc["minute"] == 0 && doc["second"] == 0) {
    //        execOnceSettime = 0;
    //      }
    //      else if (doc["hours"] != 0 || doc["minute"] != 0 || doc["second"] != 0) {
    //        debugln("time will adjust");
    //        adjustRTC(doc["year"] , monthNumber(doc["month"]), doc["day"], doc["hours"], doc["minute"], doc["second"]);
    //        adjustDeviceTime(doc["year"] , monthNumber(doc["month"]), doc["day"], doc["hours"], doc["minute"], doc["second"]);
    //
    //        execOnceSettime = 1;
    //      }
    //    }
    saveSchedule(jsonschedule, schtime);
    debug("date : "); debug(chargeTimeWeb.dateStart); debug("/"); debug(chargeTimeWeb.monthStart); debug("/"); debug(chargeTimeWeb.yearStart);
    debug(" time : "); debug(chargeTimeWeb.hoursStart); debug(":"); debug(chargeTimeWeb.minuiteStart); debug(":"); debugln(chargeTimeWeb.secoundStart);
    //{"date":"Monday","day":"20","month":"June","year":"2022","hours":"16","minute":"42","second":"37","schedule":"16:49","flagSchedule":"true"}
    debug(" Schedule Daily Task : "); debug(schtime.schhr); debug(":"); debugln(schtime.schmm);
    debug("isChargeSettingDone : "); debugln(currentValue.isChargeSettingDone);
    debug("clearSchedule : "); debugln(clearSchedule);
    debug("currentlyAmp : "); debugln(currentValue.currentlyAmp);
    debug("isPressedC : "); debugln(currentValue.isPressedC);
    debug("alwaysSchedule : "); debugln(schtime.schalw);
    debug("flagSchedule : "); debugln(schtime.schflg);
    //    String output;
    //    doc.printTo(output);
    //    //    debugln(output);
    //    websockets.broadcastTXT(output);
  }
  else if (displayPage == 1) {                                     //////////////////////status page
    DynamicJsonBuffer jsonBuffer;
    JsonObject &doc = jsonBuffer.parseObject(message);

    if (!doc.success()) {
      debugln(F("Failed to read file, using page status"));
    }

    int mes1 = doc["mes1"];
    int mes2 = doc["mes2"];
    const char* mes3 = doc["mes3"];

    debug("mes1 : "); debugln(mes1);
    debug("mes2 : "); debugln(mes2);
    debug("mes3 : "); debugln(mes3);
  }
  else if (displayPage == 2) {                                           ////////////////////setting page
    DynamicJsonBuffer jsonBuffer;
    JsonObject &doc = jsonBuffer.parseObject(message);

    if (!doc.success()) {
      debugln(F("Failed to read file, page setting"));
    }

    if (doc.containsKey("isAuthen")) {
      isAuthen = doc["isAuthen"] | false;
    }

    if (doc.containsKey("rfidMode")) {
      if (doc["rfidMode"] == 1) {
        currentValue.RfidUseFname = "RFID";
        currentValue.RfidUseLname = "Mode";
      }
      else if (doc["rfidMode"] == 0) {
        currentValue.RfidUseFname = "Non RFID";
        currentValue.RfidUseLname = "Mode";
      }
      config.rfidusing = doc["rfidMode"] | 0;
    }
    if (doc.containsKey("MaxCurrent")) {
      config.currentmax = doc["MaxCurrent"] | 32;
    }
    if (doc.containsKey("meterPhase")) {
      config.phaseset = doc["meterPhase"] | 1;
      //      initmeter(config.phaseset);
    }
    if (doc.containsKey("ssid")) {
      strlcpy(config.ssid, doc["ssid"] | "Bluestone", sizeof(config.ssid));
    }
    if (doc.containsKey("pass")) {
      strlcpy(config.pass, doc["pass"] | "", sizeof(config.pass));
    }
    if (doc.containsKey("ssidap")) {
      strlcpy(config.ssidap, doc["ssidap"] | "BluestoneAP", sizeof(config.ssidap));
    }
    if (doc.containsKey("passap")) {
      strlcpy(config.passap, doc["passap"] | "", sizeof(config.passap));
    }
    if (doc.containsKey("ip")) {
      strlcpy(config.ip, doc["ip"] | "192.168.4.1", sizeof(config.ip));
    }
    if (doc.containsKey("username")) {
      strlcpy(config.username, doc["userID"] | "admin", sizeof(config.username));
    }
    if (doc.containsKey("userpass")) {
      strlcpy(config.userpass, doc["userPass"] | "admin", sizeof(config.userpass));
    }
    if (doc.containsKey("domainName")) {
      strlcpy(config.domainName, doc["domainName"] | "bluestoneev", sizeof(config.domainName));
    }

    debug("username : "); debugln(String(config.username));
    debug("userpass : "); debugln(String(config.userpass));
    debug("ssid : "); debugln(String(config.ssid));
    debug("pass : "); debugln(String(config.pass));
    debug("ssidap : "); debugln(String(config.ssidap));
    debug("passap : "); debugln(String(config.passap));
    debug("ip : "); debugln(String(config.ip));
    debug("domainName : "); debugln(String(config.domainName));
    debug("phaseset : "); debugln(String(config.phaseset));
    debug("rfidusing : "); debugln(String(config.rfidusing));
    debug("currentmax : "); debugln(String(config.currentmax));

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    if (doc.containsKey("domainName") || doc.containsKey("userpass") || doc.containsKey("username") || doc.containsKey("ip") || doc.containsKey("passap") || doc.containsKey("ssidap") || doc.containsKey("pass") || doc.containsKey("ssid")) {
      swDelay(1000);
      ESP.restart();
    }
  }
  else if (displayPage == 3) {                                                        ////////////////////////////logger
    debugln("log page");
  }
  else if (displayPage == 4) {                                                             /////////////////////////rfid
    DynamicJsonBuffer jsonBuffer;
    JsonObject &doc = jsonBuffer.parseObject(message);

    if (!doc.success()) {
      debugln(F("Failed to read file, page rfid"));
    }

    if (doc.containsKey("fname")) {
      rfidfig.rfidfname = doc["fname"] | "Unknown";
      rfidGetInputName = true;
    }
    if (doc.containsKey("lname")) {
      rfidfig.rfidlname = doc["lname"] | "Unknown";
      rfidGetInputName = true;
    }
    if (doc.containsKey("flagDelete")) {
      isRfidDelete = doc["flagDelete"] | "true";
    }
    if (doc.containsKey("delUid")) {
      delUid = doc["delUid"] | "Unknown";
    }
    if (doc.containsKey("status")) {
      rfidStatus = doc["status"] | 0;
    }

    debug("rfidStatus : "); debugln(String(rfidStatus));
    debug("delUid : "); debugln(String(delUid));
    debug("flagDelete : "); debugln(String(isRfidDelete));
    debug("rfidfname : "); debugln(String(rfidfig.rfidfname));
    debug("rfidlname : "); debugln(String(rfidfig.rfidlname));
  }
}

void sendRandNumber() {
  uint8_t h = random(21, 40);
  uint8_t t = random(0, 20);

  String JSON_Data = "{\"rand1\":";
  JSON_Data += t;
  JSON_Data += ",\"rand2\":";
  JSON_Data += h;
  JSON_Data += "}";
  debugln(JSON_Data);
  websockets.broadcastTXT(JSON_Data);
}

void simulateMeter(uint8_t phase) {
  readmeter(phase);

  DynamicJsonBuffer jsonBuffer;
  JsonObject &doc = jsonBuffer.createObject();
  //  if (currentValue.isStartOrFinish == 3) {
  if (currentValue.startChargeHR != 0 || currentValue.startChargeMN != 0 || currentValue.startChargeSS != 0 ) {
    //}
    TimeDuration = "";
    if (currentValue.startChargeHR < 10) {
      TimeDuration += "0";
      TimeDuration += String(currentValue.startChargeHR);
    }
    else {
      TimeDuration += String(currentValue.startChargeHR);
    }
    TimeDuration += ":";
    if (currentValue.startChargeMN < 10) {
      TimeDuration += "0";
      TimeDuration += String(currentValue.startChargeMN);
    }
    else {
      TimeDuration += String(currentValue.startChargeMN);
    }
    TimeDuration += ":";
    if (currentValue.startChargeSS < 10) {
      TimeDuration += "0";
      TimeDuration += String(currentValue.startChargeSS);
    }
    else {
      TimeDuration += String(currentValue.startChargeSS);
    }
    doc["TimeCount"] = TimeDuration;

    debugln(TimeDuration);

    //    if (ssbuff == 59) {
    //      ssbuff = 0;
    //      if (mnbuff == 59) {
    //        hrbuff++;
    //      }
    //      else {
    //        mnbuff++;
    //      }
    //    }
    //    else {
    //      ssbuff++;
    //    }
  }
  else {
    doc["TimeCount"] = "--:--:--";
  }
  if (currentValue.webChargeStatus == 6) {
    doc["ScheduleTime"] = schtime.schfulltext;
  }
  doc["Mode"] = currentValue.webChargeStatus;
  //  currentValue.ChargeStartEnergy
  doc["Energy"] = electric.energy - currentValue.ChargeStartEnergy;
  debugln("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd");
  debug("ee2"); debugln(electric.energy);
  debug("ee3"); debugln(currentValue.ChargeStartEnergy);
  debug("Voltage"); debugln(electric.voltage3[0]);
  debug("Current"); debugln(electric.current3[0]);
  debug("Power"); debugln(electric.power3[0]);
  debugln("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd");
  if (phase == 1) {
    doc["Voltage"] = electric.voltage1;
    doc["Current"] = electric.current1;
    doc["Power"] = electric.power1;
  }
  else if (phase == 3) {
    doc["Voltage"] = electric.voltage3[0];
    doc["Current"] = electric.current3[0];
    doc["Power"] = electric.power3[0];

    doc["Voltage2"] = electric.voltage3[1];
    doc["Current2"] = electric.current3[1];
    doc["Power2"] = electric.power3[1];

    doc["Voltage3"] = electric.voltage3[2];
    doc["Current3"] = electric.current3[2];
    doc["Power3"] = electric.power3[2];
  }
  //  String currentAmpBuff;
  //  if (currentValue.currentlyAmp < 10) {
  //    currentAmpBuff = " ";
  //    currentAmpBuff += currentValue.currentlyAmp;
  //  }
  //  else {
  //    currentAmpBuff = currentValue.currentlyAmp;
  //  }
  doc["userCurrentSet"] = currentValue.currentlyAmp;
  String output;
  doc.printTo(output);
  //    debugln(output);
  websockets.broadcastTXT(output);
}

void evStatus() {

  //forAccidentOff currentValue;
  DynamicJsonBuffer jsonBuffer;
  JsonObject &doc = jsonBuffer.createObject();

  doc["Mode"] = currentValue.webChargeStatus;
  bool rfidUseBuff;
  if (config.rfidusing == 0) {
    rfidUseBuff = false;
  }
  else if (config.rfidusing == 1) {
    rfidUseBuff = true;
  }

  //  String currentAmpBuff;
  //  if (currentValue.currentlyAmp < 10) {
  //    currentAmpBuff = " ";
  //    currentAmpBuff += currentValue.currentlyAmp;
  //  }
  //  else {
  //    currentAmpBuff = currentValue.currentlyAmp;
  //  }
  doc["ScheduleTime"] = schtime.schfulltext;
  doc["flagSubmit"] = currentValue.isChargeSettingDone;
  //  doc["userCurrentSet"] = currentValue.currentlyAmp;
  doc["Pressing"] = currentValue.isPressedC;
  doc["RFIDUse"] = rfidUseBuff;
  doc["Status"] = currentValue.carNowStat;
  doc["StatusPrevious"] = currentValue.carLastStat;
  
  if (currentValue.webChargeStatus == 7) {
    doc["errorTemptext"] = errorTemptext;
  }

  String output;
  doc.printTo(output);
  //    debugln(output);
  websockets.broadcastTXT(output);
}

void rfidSend() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject &doc = jsonBuffer.createObject();

  doc["uid"] = nowUid;
  doc["status"] = rfidStatus;

  String output;
  doc.printTo(output);
  debug("rfid send");
  debugln(output);
  websockets.broadcastTXT(output);
}

void settingShow() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject &doc = jsonBuffer.createObject();

  doc["username"] = config.username;
  doc["userpass"] = config.userpass;
  doc["ssid"] = config.ssid;
  doc["pass"] = config.pass;
  doc["ssidap"] = config.ssidap;
  doc["passap"] = config.passap;
  doc["ip"] = config.ip;
  //  doc["gateway"] = config.gateway;
  doc["rfidusing"] = config.rfidusing;
  doc["currentmax"] = config.currentmax;
  doc["phaseset"] = config.phaseset;
  doc["domainName"] = config.domainName;
  String output;
  doc.printTo(output);
  debug("setting send : ");
  debugln(output);
  websockets.broadcastTXT(output);
}

void sendSetTimeflag() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject &doc = jsonBuffer.createObject();

  doc["Mode"] = currentValue.webChargeStatus;
  doc["userCurrentSet"] = currentValue.currentlyAmp;
  if (execOnceSettime == 0) {
    doc["SetTime"] = false;
  }
  else if (execOnceSettime == 1) {
    doc["SetTime"] = true;
    execSetTime = false;
    execOnceSettime = 0;
  }

  String output;
  doc.printTo(output);
  debug("setting send : ");
  debugln(output);
  websockets.broadcastTXT(output);
}

void rfidAuthen() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject &doc = jsonBuffer.createObject();

  nowUid = "";
  rfidreadread();
  //had temporary var
  if (nowUid != "" || nowUid != "timeout") {
    if (checkRFID(nowUid, jsonaccRFID) == false) {
      execRfidAuthen = false;
      debugln("Hello Member");
      rfidAuthenStatus = 1;
    }
    else {
      execRfidAuthen = true;
      rfidAuthenStatus = 0;
      debugln("You are not member");
    }
    doc["status"] = rfidAuthenStatus;

  }
  else {
    execRfidAuthen = true;
    rfidAuthenStatus = 2;
    debugln("Time out ");
  }
  String output;
  doc.printTo(output);
  debug("setting send : ");
  debugln(output);
  websockets.broadcastTXT(output);
}
