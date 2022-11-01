//void addHistory(uint16_t chargeHour, uint8_t chargeMinute, uint8_t chargeSecound, String fname, String lname, String uid, float chargeEnergy, const char *filename);
void addHistory(uint16_t startYY, uint8_t startMT, uint8_t startDD, uint8_t startHH, uint8_t startMM, uint8_t startSS , uint16_t chargeHour, uint8_t chargeMinute, uint8_t chargeSecound, String fname, String lname, String uid, float chargeEnergy, const char *filename);

void addHistory(uint16_t startYear, uint8_t startMonth, uint8_t startDay, uint8_t startHour, uint8_t startMinute, uint8_t startSecound , uint16_t chargeHour, uint8_t chargeMinute, uint8_t chargeSecound, String fname, String lname, String uid, float chargeEnergy, const char *filename)
//void addHistory(uint16_t chargeHour, uint8_t chargeMinute, uint8_t chargeSecound, String fname, String lname, String uid, float chargeEnergy, const char *filename)
//void addHistory(uint16_t chargeHour, uint8_t chargeMinute, uint8_t chargeSecound, uint8_t startHour, uint8_t startMinute, uint8_t startSecound, uint8_t startDay, uint8_t startMonth, uint16_t startYear, String fname, String lname, float chargeEnergy, const char *filename)
{
  String combinedTime = "";
  if (startHour < 10) {
    combinedTime += "0";
    combinedTime += startHour;
  }
  else {
    combinedTime += startHour;
  }
  combinedTime += ":";
  if (startMinute < 10) {
    combinedTime += "0";
    combinedTime += startMinute;
  }
  else {
    combinedTime += startMinute;
  }
  combinedTime += ":";
  if (startSecound < 10) {
    combinedTime += "0";
    combinedTime += startSecound;
  }
  else {
    combinedTime += startSecound;
  }
  //  combinedTime += startHour;
  //  combinedTime += ":";
  //  combinedTime += startMinute;
  //  combinedTime += ":";
  //  combinedTime += startSecound;

  String combinedDate = "";
  if (startDay < 10) {
    combinedDate += "0";
    combinedDate += startDay;
  }
  else {
    combinedDate += startDay;
  }
  combinedDate += "/";
  if (startMonth < 10) {
    combinedDate += "0";
    combinedDate += startMonth;
  }
  else {
    combinedDate += startMonth;
  }
  combinedDate += "/";

  combinedDate += startYear;

  //  String combinedDate = "";
  //  combinedDate += startDay;
  //  combinedDate += "/";
  //  combinedDate += startMonth;
  //  combinedDate += "/";
  //  combinedDate += startYear;

  String combinedChargeTime = "";
  if (chargeHour < 10) {
    combinedChargeTime += "0";
    combinedChargeTime += chargeHour;
  }
  else {
    combinedChargeTime += chargeHour;
  }
  combinedChargeTime += ":";
  if (chargeMinute < 10) {
    combinedChargeTime += "0";
    combinedChargeTime += chargeMinute;
  }
  else {
    combinedChargeTime += chargeMinute;
  }
  combinedChargeTime += ":";
  if (chargeSecound < 10) {
    combinedChargeTime += "0";
    combinedChargeTime += chargeSecound;
  }
  else {
    combinedChargeTime += chargeSecound;
  }

  if (config.rfidusing == 0) {
    fname = "Non RFID";
    lname = "Mode";
    uid = "NOUSED";
  }

  String combinedName = "";
  combinedName += fname;
  combinedName += " ";
  combinedName += lname;
  debug("electric.energy : "); debugln(electric.energy);
  debug("chargeEnergy : "); debugln(chargeEnergy);
  float deltaEnergy = electric.energy - chargeEnergy;

  if (history.totalChatge == 0) {
    history.totalChatge = 1;
  }

  DynamicJsonBuffer jsonBuffer(mainrfidaccsize + (history.totalChatge * mainrfidpower));
  JsonObject& creatData = jsonBuffer.createObject();
  creatData["Date"] = combinedDate;
  creatData["Time"] = combinedTime;
  creatData["Name"] = combinedName;
  //  creatData["UID"] = uid;
  creatData["Hours"] = combinedChargeTime;
  creatData["Energy"] = deltaEnergy;
  debug("abs deltaEnergy : "); debugln(deltaEnergy);
//  creatData["Energy"] = abs(deltaEnergy);
//  debug("abs deltaEnergy : "); debugln(abs(deltaEnergy));

  String data_;
  creatData.printTo(data_);

  JsonObject& dataRec = jsonBuffer.parseObject(data_);
  File file = SPIFFS.open(filename, "r");
  JsonObject& root = jsonBuffer.parseObject(file);
  JsonArray& dataArray = root["Data"];
  file.close();
  int count_ = dataArray.size();

  JsonObject& rootW = jsonBuffer.createObject();
  JsonArray& addData = rootW.createNestedArray("Data");
  debug("datasize:"); debugln(count_);
  Serial.print(F("///////////////////////////////////////// DATASIZE A : ")); Serial.println(count_);
  Serial.print(F("///////////////////////////////////////// DATASIZE B : ")); Serial.println(history.totalChatge);
  history.totalChatge = count_ + 1;

  if (count_ > MAXHISTORYLOG) {
    for (int xx = 1 ; xx < count_; xx++) {
      addData.add(dataArray[xx]);
    }
    addData.add(dataRec);
  }
  else {
    for (int xx = 0 ; xx < count_; xx++) {
      addData.add(dataArray[xx]);
    }
    addData.add(dataRec);
  }


  File fileW = SPIFFS.open(filename, "w");
  if (!fileW) {
    debugln("Failed to open file for writing");
  }
  rootW.printTo(fileW);
  fileW.close();
  printFile(filename);
  saveTotalHistory(jsonTotalHistory, history);
}
