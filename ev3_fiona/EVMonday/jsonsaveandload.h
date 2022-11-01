void createEmptyJson(const char *filename);
void printFile(const char *filename);
void loadAccRfid(void);
void loadHistory(void);
void loadConfiguration(const char *filename, Config &config);
void saveConfiguration(const char *filename, const Config &config);
void loadSchedule(const char *filename, scheduletimefromweb &schtime);
void saveSchedule(const char *filename, const scheduletimefromweb &schtime);
void loadTotalRfid(const char *filename, RfidInfo &rfidfig);
void saveTotalRfid(const char *filename, const RfidInfo &rfidfig);
void loadTotalHistory(const char *filename, charginghistory &history);
void saveTotalHistory(const char *filename, const charginghistory &history);
void ssidAdd(String ssid , int16_t rssi);
void ssidClear(void);


void createEmptyJson(const char *filename) {
  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file)
  {
    debugln(F("Failed to create file empty"));
    return;
  }
  const size_t capacity = JSON_OBJECT_SIZE(0);
  DynamicJsonBuffer jsonBuffer(capacity);

  JsonObject& root = jsonBuffer.createObject();

  if (root.printTo(file) == 0) {
    debugln(F("Failed to write to file empty"));
  }
  file.close();
}

void printFile(const char *filename)
{
  File file = SPIFFS.open(filename);
  if (!file)
  {
    Serial.println(F("Failed to read file"));
    return;
  }
  while (file.available())
  {
    Serial.print((char)file.read());
  }
  Serial.println();

  file.close();
}

void loadAccRfid() {
  bool file = SPIFFS.exists(jsonaccRFID);

  if (!file) {
    debugln(F("Failed to read file, using default configuration"));
    //    file.close();
    File filewrite = SPIFFS.open(jsonaccRFID, "w");
    if (!filewrite) {
      debugln("Failed to open file for writing");
      return;
    }
    DynamicJsonBuffer jsonMainWriteBuffer;// Create Parent Json
    DynamicJsonBuffer jsonWriteBuffer;// Create Children Json and insert to Parent Json
    JsonObject& rootWriteMain = jsonMainWriteBuffer.createObject();// Parent Json name (rootWriteMain)
    JsonArray& Data = rootWriteMain.createNestedArray("data");// Create Data Array in Parent -> {"Data":[]}
    rootWriteMain.printTo(filewrite);
    filewrite.close();
  }
  else if (file) {
    //    file.close();
  }
  printFile(jsonaccRFID);
}

void loadHistory() {
  bool file = SPIFFS.exists("/js/logger.json");

  if (!file) {
    debugln(F("Failed to read file, using default configuration"));
    //    file.close();
    File filewrite = SPIFFS.open("/js/logger.json", "w");
    if (!filewrite) {
      debugln("Failed to open file for writing");
      return;
    }
    DynamicJsonBuffer jsonMainWriteBuffer;// Create Parent Json
    DynamicJsonBuffer jsonWriteBuffer;// Create Children Json and insert to Parent Json
    JsonObject& rootWriteMain = jsonMainWriteBuffer.createObject();// Parent Json name (rootWriteMain)
    JsonArray& Data = rootWriteMain.createNestedArray("Data");// Create Data Array in Parent -> {"Data":[]}
    rootWriteMain.printTo(filewrite);
    filewrite.close();
  }
  else if (file) {
    //    file.close();
  }
  printFile("/js/logger.json");
}

void loadConfiguration(const char *filename, Config &config)
{
  File file = SPIFFS.open(filename);
  DynamicJsonBuffer jsonBuffer(configsize);
  JsonObject &doc = jsonBuffer.parseObject(file);

  if (!doc.success()) {
    debugln(F("Failed to read file, using default configuration"));
    hasConfig = false;
  }

  strlcpy(config.username, doc["username"] | "admin", sizeof(config.username));
  strlcpy(config.userpass, doc["userpass"] | "admin", sizeof(config.userpass));
  strlcpy(config.ssid, doc["ssid"] | "Bluestone", sizeof(config.ssid));
  strlcpy(config.pass, doc["pass"] | "Blue@1234!", sizeof(config.pass));
  strlcpy(config.ssidap, doc["ssidap"] | "BluestoneAP03", sizeof(config.ssidap));
  strlcpy(config.passap, doc["passap"] | "", sizeof(config.passap));
  strlcpy(config.ip, doc["ip"] | "192.168.4.1", sizeof(config.ip));
  strlcpy(config.domainName, doc["domainName"] | "bluecharge", sizeof(config.domainName));
  //  strlcpy(config.gateway,doc["gateway"] | "192.168.1.1",sizeof(config.gateway));
  config.rfidusing = doc["rfidusing"] | 0;
  config.currentmax = doc["currentmax"] | 32;
  config.phaseset = doc["phaseset"] | 1;
  config.port = doc["port"] | 80;
  config.sleepSec = doc["sleep"] | 60;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  strlcpy(config.mqttcilentid, doc["mqttcilentid"] | "evmqttFFFFFF", sizeof(config.mqttcilentid));
  strlcpy(config.mqttuser, doc["mqttuser"] | "barry", sizeof(config.mqttuser));
  strlcpy(config.mqttpass, doc["mqttpass"] | "pinkywinky", sizeof(config.mqttpass));
  strlcpy(config.mqttip, doc["mqttip"] | "174.138.19.183", sizeof(config.mqttip));
  config.mqttport = doc["mqttport"] | 1883;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  config.isInSleep = doc["isInSleep"] | 0;
  config.dayThatDidntReset = doc["dayThatDidntReset"] | 5;
  config.maxDayThatDidntReset = doc["maxDayThatDidntReset"] | 5;
  config.resetHourRangeStart = doc["resetHourRangeStart"] | 1;
  config.resetHourRangeEnd = doc["resetHourRangeEnd"] | 23;
  config.criticalHeapRangeStart = doc["criticalHeapRangeStart"] | 20000;
  config.criticalHeapRangeEnd = doc["criticalHeapRangeEnd"] | 30000;
  config.doneReset = doc["doneReset"] | 0;
  config.firstSleepThisDay = doc["firstSleepThisDay"] | 0;
  DateTime now = rtc.now(); // Get current time
  uint8_t dd;
  dd = now.day();
  swDelay(100);
  config.currentDate = doc["currentDate"] | dd;

  file.close();
}

void saveConfiguration(const char *filename, const Config &config)
{
  //  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file)
  {
    debugln(F("Failed to create file config"));
    return;
  }
  DynamicJsonBuffer jsonBuffer(configsize);
  JsonObject &doc = jsonBuffer.createObject();

  doc["username"] = config.username;
  doc["userpass"] = config.userpass;
  doc["ssid"] = config.ssid;
  doc["pass"] = config.pass;
  doc["ssidap"] = config.ssidap;
  doc["passap"] = config.passap;
  doc["ip"] = config.ip;
  doc["domainName"] = config.domainName;
  //  doc["gateway"] = config.gateway;
  doc["rfidusing"] = config.rfidusing;
  doc["currentmax"] = config.currentmax;
  doc["phaseset"] = config.phaseset;
  doc["port"] = config.port;
  doc["sleep"] = config.sleepSec;

  doc["mqttcilentid"] = config.mqttcilentid;
  doc["mqttuser"] = config.mqttuser;
  doc["mqttpass"] = config.mqttpass;
  doc["mqttip"] = config.mqttip;
  doc["mqttport"] = config.mqttport;

  doc["isInSleep"] = config.isInSleep;
  doc["dayThatDidntReset"] = config.dayThatDidntReset;
  doc["maxDayThatDidntReset"] = config.maxDayThatDidntReset;
  doc["resetHourRangeStart"] = config.resetHourRangeStart;
  doc["resetHourRangeEnd"] = config.resetHourRangeEnd;
  doc["criticalHeapRangeStart"] = config.criticalHeapRangeStart;
  doc["criticalHeapRangeEnd"] = config.criticalHeapRangeEnd;
  doc["doneReset"] = config.doneReset;
  doc["firstSleepThisDay"] = config.firstSleepThisDay;
  swDelay(100);
  doc["currentDate"] = config.currentDate;

  if (doc.printTo(file) == 0) {
    debugln(F("Failed to write to file"));
  }

  file.close();
}

void loadSchedule(const char *filename, scheduletimefromweb &schtime)
{
  File file = SPIFFS.open(filename);
  DynamicJsonBuffer jsonBuffer(timesize);
  JsonObject &doc = jsonBuffer.parseObject(file);

  if (!doc.success()) {
    debugln(F("Failed to read file, using default configuration"));
    hasScheduleSet = false;
  }

  schtime.schfulltext = doc["schfulltext"] | "unknown";
  schtime.schalw = doc["always"] | false;
  schtime.schflg = doc["flag"] | false;
  //  schtime.schyy = doc["year"] | 2022;
  //  schtime.schmt = doc["month"] | 1;
  //  schtime.schdy = doc["day"] | 1;
  schtime.schhr = doc["hour"] | 1;
  schtime.schmm = doc["minute"] | 1;
  schtime.schss = doc["sec"] | 1;
  schtime.clearSchedule = doc["clearSchedule"] | true;
  file.close();
}

void saveSchedule(const char *filename, const scheduletimefromweb &schtime)
{

  //  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file)
  {
    debugln(F("Failed to create file schedule"));
    return;
  }
  DynamicJsonBuffer jsonBuffer(timesize);
  JsonObject &doc = jsonBuffer.createObject();

  doc["schfulltext"] = schtime.schfulltext ;
  doc["always"] = schtime.schalw;
  doc["flag"] = schtime.schflg;
  //  doc["year"] = schtime.schyy;
  //  doc["month"] = schtime.schmt;
  //  doc["day"] = schtime.schdy;
  doc["hour"] = schtime.schhr;
  doc["minute"] = schtime.schmm;
  doc["sec"] = schtime.schss;
  doc["clearSchedule"] = schtime.clearSchedule;

  if (doc.printTo(file) == 0) {
    debugln(F("Failed to write to file"));
  }

  file.close();
}

void loadTotalRfid(const char *filename, RfidInfo &rfidfig)
{
  File file = SPIFFS.open(filename);
  DynamicJsonBuffer jsonBuffer(rfidtotaljsonsize);
  JsonObject &doc = jsonBuffer.parseObject(file);

  if (!doc.success()) {
    debugln(F("Failed to read file max rfid"));
    hasloadTotalRFIDSet = false;
  }

  rfidfig.rfidtotal = doc["total"] | 0;

  file.close();
}

void saveTotalRfid(const char *filename, const RfidInfo &rfidfig)
{

  //  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file)
  {
    debugln(F("Failed to create file maxsize rfid"));
    return;
  }
  DynamicJsonBuffer jsonBuffer(rfidtotaljsonsize);
  JsonObject &doc = jsonBuffer.createObject();

  doc["total"] = rfidfig.rfidtotal;

  if (doc.printTo(file) == 0) {
    debugln(F("Failed to write to file"));
  }

  file.close();
}

void loadAccidentOff(const char *filename, forAccidentOff &currentValue)
{
  File file = SPIFFS.open(filename);
  DynamicJsonBuffer jsonBuffer(currentlyAccidentsize);
  JsonObject &doc = jsonBuffer.parseObject(file);

  if (!doc.success()) {
    debugln(F("Failed to read accident"));
    hasloadAccidentOffSet = false;
  }
  //  currentValue.isStillCharge = doc["isStillCharge"] | false;
  currentValue.currentlyAmp = doc["currentlyAmp"] | 32;
  currentValue.carLastStat = doc["carLastStat"] | 0;
  currentValue.isPressedC = doc["isPressedC"] | false;
  currentValue.isChargeSettingDone = doc["isChargeSettingDone"] | false;
  currentValue.isStartOrFinish = doc["isStartOrFinish"] | 0;
  currentValue.webChargeStatus =  doc["webChargeStatus"] | 0;
  currentValue.startChargeHR = doc["timeChargingHH"] | 0;
  currentValue.startChargeMN = doc["timeChargingMM"] | 0;
  currentValue.startChargeSS = doc["timeChargingSS"] | 0;
  currentValue.scheduleStart = doc["scheduleStart"] | false;
  currentValue.RfidUseFname = doc["RfidUseFname"] | "";
  currentValue.RfidUseLname = doc["RfidUseLname"] | "";
  currentValue.RfidUseUid = doc["RfidUseUid"] | "";
  currentValue.FirstGuyConnectedMAC = doc["FirstGuyConnectedMAC"] | "";
  currentValue.ChargeStartEnergy = doc["ChargeStartEnergy"] | "";
  //  printf("from load function : ChargeStartEnergy : %s\n", currentValue.ChargeStartEnergy);
  //  printf("from load function : ChargeStartEnergy : %0.4f\n", atof(currentValue.ChargeStartEnergy.c_str()));
  currentValue.startYR = doc["startYR"] | 0;
  currentValue.startMT = doc["startMT"] | 0;
  currentValue.startDD = doc["startDD"] | 0;
  currentValue.startHH = doc["startHH"] | 0;
  currentValue.startMN = doc["startMN"] | 0;
  currentValue.startSS = doc["startSS"] | 0;

  file.close();
}

void saveAccidentOff(const char *filename, const forAccidentOff &currentValue)
{
  //  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file)
  {
    debugln(F("Failed to create backup accident"));
    return;
  }
  DynamicJsonBuffer jsonBuffer(currentlyAccidentsize);
  JsonObject &doc = jsonBuffer.createObject();

  doc["currentlyAmp"] = currentValue.currentlyAmp;
  doc["carLastStat"] = currentValue.carLastStat;
  doc["isPressedC"] = currentValue.isPressedC;
  doc["isChargeSettingDone"] = currentValue.isChargeSettingDone;
  doc["isStartOrFinish"] = currentValue.isStartOrFinish;
  doc["webChargeStatus"] = currentValue.webChargeStatus;
  //  doc["startChargeYR"] =  currentValue.startChargeYR;
  //  doc["startChargeMT"] = currentValue.startChargeMT;
  //  doc["startChargeDD"] = currentValue.startChargeDD;
  doc["timeChargingHH"] = currentValue.startChargeHR;
  doc["timeChargingMM"] = currentValue.startChargeMN;
  doc["timeChargingSS"] = currentValue.startChargeSS;
  doc["scheduleStart"] = currentValue.scheduleStart;
  doc["RfidUseFname"] =   currentValue.RfidUseFname;
  doc["RfidUseLname"] = currentValue.RfidUseLname;
  doc["RfidUseUid"] = currentValue.RfidUseUid;
  doc["FirstGuyConnectedMAC"] = currentValue.FirstGuyConnectedMAC;
  doc["ChargeStartEnergy"] = currentValue.ChargeStartEnergy;


  doc["startYR"] = currentValue.startYR;
  doc["startMT"] = currentValue.startMT;
  doc["startDD"] = currentValue.startDD;
  doc["startHH"] = currentValue.startHH;
  doc["startMN"] = currentValue.startMN;
  doc["startSS"] = currentValue.startSS;

  if (doc.printTo(file) == 0) {
    debugln(F("Failed to write to file"));
  }
  file.close();
}

void loadTotalHistory(const char *filename, charginghistory &history)
{
  File file = SPIFFS.open(filename);
  DynamicJsonBuffer jsonBuffer(rfidtotaljsonsize);
  JsonObject &doc = jsonBuffer.parseObject(file);

  if (!doc.success()) {
    debugln(F("Failed to Load total history"));
    hasloadHistorySet = false;
  }

  history.totalChatge = doc["total"] | 0;

  file.close();
}

void saveTotalHistory(const char *filename, const charginghistory &history)
{

  //  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file)
  {
    debugln(F("Failed to create file maxsize rfid"));
    return;
  }
  DynamicJsonBuffer jsonBuffer(rfidtotaljsonsize);
  JsonObject &doc = jsonBuffer.createObject();

  doc["total"] = history.totalChatge;

  if (doc.printTo(file) == 0) {
    debugln(F("Failed to write to file"));
  }

  file.close();
}

void ssidAdd(String ssid , int16_t rssi) {
  DynamicJsonBuffer jsonBuffer(200);
  JsonObject& creatData = jsonBuffer.createObject();
  creatData["ssid"] = ssid;
  creatData["rssi"] = rssi;
  String data_;
  creatData.printTo(data_);

  //---------------------------------------------------------------
  JsonObject& dataRec = jsonBuffer.parseObject(data_);
  File file = SPIFFS.open("/js/wifistalist.json", "r");
  JsonObject& root = jsonBuffer.parseObject(file);
  JsonArray& dataArray = root["Data"];
  file.close();
  int count_ = dataArray.size();

  JsonObject& rootW = jsonBuffer.createObject();
  JsonArray& addData = rootW.createNestedArray("Data");

  for (int xx = 0 ; xx < count_; xx++) {
    addData.add(dataArray[xx]);
  }
  addData.add(dataRec);

  File fileW = SPIFFS.open("/js/wifistalist.json", "w");
  if (!fileW) {
    Serial.println("Failed to open file for writing");
  }
  rootW.printTo(fileW);
  fileW.close();
}

void ssidClear() {
  File filewrite = SPIFFS.open("/js/wifistalist.json", "w");
  if (!filewrite) {
    Serial.println("Failed to open file for writing");
    return;
  }
  DynamicJsonBuffer jsonMainWriteBuffer;// Create Parent Json
  DynamicJsonBuffer jsonWriteBuffer;// Create Children Json and insert to Parent Json
  JsonObject& rootWriteMain = jsonMainWriteBuffer.createObject();// Parent Json name (rootWriteMain)
  JsonArray& Data = rootWriteMain.createNestedArray("Data");// Create Data Array in Parent -> {"Data":[]}
  rootWriteMain.printTo(filewrite);
  filewrite.close();
}
