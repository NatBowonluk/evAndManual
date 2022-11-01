#if RFIDSALE == 1
const unsigned long eventIntervalrfid = 5000;
unsigned long previousTimerfid = 0;

bool checkRFID(String uid, const char *filename);
void addAccountRFID(String fname, String lname, String uid, const char *filename);
void delAccountRFID(String uid, const char *filename);
//void resetRfid(void);
void rfidinit(void);
void rfidreadread(/*const char *fname, const char *lname*/void);

///////{"data"][i]["uid"]
bool checkRFID(String uid, const char *filename)
{
  if (rfidfig.rfidtotal == 0) {
    rfidfig.rfidtotal = 1;
  }

  File file = SPIFFS.open(filename);

  DynamicJsonBuffer jsonBuffer(mainrfidaccsize + (rfidfig.rfidtotal * mainrfidpower));

  JsonObject &doc = jsonBuffer.parseObject(file);

  if (!doc.success()) {
    debugln(F("Failed to read file rfid acc"));
    file.close();
    return false;
  }
  JsonArray& dataArray = doc["data"];
  debug("datatag:"); debugln(uid);
  int count_ = dataArray.size();
  if (count_ == 0) {
    count_ = count_ + 1;
  }
  rfidfig.rfidtotal = count_ + 1;
  debug("datasize:"); debugln(rfidfig.rfidtotal);

  for (int i = 0; i <= rfidfig.rfidtotal; i++) {
    JsonObject& dataA = doc["data"][i];
    const char* dataAuid = dataA["uid"];
    debug("datainjson:"); debugln(dataAuid);
    if (uid == String(dataAuid)) {
      file.close();
      const char* dataAfname = dataA["fname"];
      const char* dataAlname = dataA["lname"];
      if (config.rfidusing == 1 && execDisconnect == true) {
        currentValue.RfidUseFname = String(dataAfname);
        currentValue.RfidUseLname = String(dataAlname);
        currentValue.RfidUseUid = String(dataAuid);
      }
      else if (config.rfidusing == 0) {
        //      else {
        currentValue.RfidUseFname = "Non RFID";
        currentValue.RfidUseLname = "Mode";
        currentValue.RfidUseUid = "Unknown";
      }
      debug("Fname : "); debugln(String(dataAfname));
      debug("Lname : "); debugln(String(dataAlname));
      debug("UID : "); debugln(String(dataAlname));
      debugln("check 0");
      return false;
    }
  }
  file.close();
  debugln("check 1");
  return true;
  //  }
  //  else if (rfidfig.rfidtotal == 0) {
  //    return true;
  //  }
}

void addAccountRFID(String fname, String lname, String uid, const char *filename)
{ if (rfidfig.rfidtotal == 0) {
    rfidfig.rfidtotal = 1;
  }
  DynamicJsonBuffer jsonBuffer(mainrfidaccsize + (rfidfig.rfidtotal * mainrfidpower));
  JsonObject& creatData = jsonBuffer.createObject();
  creatData["uid"] = uid;
  creatData["fname"] = fname;
  creatData["lname"] = lname;
  String data_;
  creatData.printTo(data_);

  JsonObject& dataRec = jsonBuffer.parseObject(data_);
  File file = SPIFFS.open(filename, "r");
  JsonObject& root = jsonBuffer.parseObject(file);
  JsonArray& dataArray = root["data"];
  file.close();
  int count_ = dataArray.size();

  JsonObject& rootW = jsonBuffer.createObject();
  JsonArray& addData = rootW.createNestedArray("data");
  debug("datasize:"); debugln(count_);
  rfidfig.rfidtotal = count_ + 1;

  //  if (count_ > 10) {
  //    for (int xx = 1 ; xx < count_; xx++) {
  //      addData.add(dataArray[xx]);
  //    }
  //    addData.add(dataRec);
  //  }
  //  else {
  for (int xx = 0 ; xx < count_; xx++) {
    addData.add(dataArray[xx]);
  }
  addData.add(dataRec);
  //  }
  //  for (int xx = 0 ; xx < count_; xx++) {
  //    addData.add(dataArray[xx]);
  //  }
  //  addData.add(dataRec);

  File fileW = SPIFFS.open(filename, "w");
  if (!fileW) {
    debugln("Failed to open file for writing");
  }
  rootW.printTo(fileW);
  fileW.close();
  printFile(filename);
  saveTotalRfid(jsonRFIDtotal, rfidfig);
}

void delAccountRFID(String uid, const char *filename) {
  if (rfidfig.rfidtotal == 0) {
    rfidfig.rfidtotal = 1;
  }

  DynamicJsonBuffer jsonBuffer(mainrfidaccsize + (rfidfig.rfidtotal * mainrfidpower));

  File file = SPIFFS.open(filename, "r");
  JsonObject& root = jsonBuffer.parseObject(file);
  JsonArray& dataArray = root["data"];
  file.close();
  int count_ = dataArray.size();

  JsonObject& rootW = jsonBuffer.createObject();
  JsonArray& addData = rootW.createNestedArray("data");

  for (int xx = 0 ; xx < count_; xx++) {
    if (dataArray[xx]["uid"] != uid) {
      debug("delete card A : "); Serial.println(uid);
      //      debug("delete card B : ");Serial.println(addData);
      addData.add(dataArray[xx]);
    }
  }
  rfidfig.rfidtotal = count_ - 1;
  File fileW = SPIFFS.open(filename, "w");
  if (!fileW) {
    debugln("Failed to open file for writing");
  }
  rootW.printTo(fileW);
  fileW.close();
}

//void resetRfid() {
//  debugln();
//  digitalWrite(resetRfids,  HIGH);
//  delay(100);
//  digitalWrite(resetRfids,  LOW);
//}

void rfidinit(void)
{
#if boardColour == 2
  //  pinMode(RFID_Reset, OUTPUT);
  //  digitalWrite(RFID_Reset, LOW);
#else
#endif
  register uint8_t countRF = 0;
  register bool flagCheck = true;
  debugln("rfidstart!");
  //  resetRfid();
  while (flagCheck) {
    if (rfid.init())
    {
      debugln("RFID Ready");
      flagCheck = false;
    }
    else
    {
      debugln("RFID Fail");
      swDelay(1000);
      countRF += 1;
      if (countRF <= 4) {
        //        resetRfid();'#if boardColour == 1
#if boardColour == 2
        //        iox.digitalWrite(resetPower, HIGH);
        //        delay(5000);
#elif boardColour == 1
        digitalWrite(resetPower, HIGH);
        delay(5000);
#else

#endif
        flagCheck = true;
      }
      else {
        flagCheck = false;
      }
    }
  }
  swDelay(1000);
}

void rfidreadread(/*const char *fname, const char *lname*/)
{
  //  resetRfid();
  inRFID = true;
  debugln("run run run run run");
  register String id;
  register boolean success;
  rfid.getIdHex(id, success);
  if (success)
  {
    debugln();
    debug("Outside id = ");
    debugln(id);
    //      debugln(id.length());
  }
  else {
    debugln("Timed out waiting for a card");
    id = "";
  }
  debugln(success);
  debugln(id);
  debugln(id.length());
  if (id.length() > 0) {
    debugln("read done");
    nowUid = id;
    rfidReadDone = true;
  }
  else if (id.length() < 1) {
    debugln("read failed");
    nowUid = "timeout";
    rfidReadDone = false;
  }
  startReadTag = false;
  execRfidv2 = false;
#if boardColour == 2
  //  delay(1000);
#else
  //  swDelay(1000);
#endif
  inRFID = false;
}
#else
#endif
