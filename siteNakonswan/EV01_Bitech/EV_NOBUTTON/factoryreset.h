void logClear(void);
void AccClear(void);
void factoryReset(void);

void logClear() {
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


void AccClear() {
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

void factoryReset() {
  cancelSchedule();
  swDelay(15);
  logClear();
  swDelay(15);
  createEmptyJson(jsonTotalHistory);
  swDelay(15);
  createEmptyJson(jsonRFIDtotal);
  swDelay(15);
  createEmptyJson(jsonAccident);
  swDelay(15);
  SPIFFS.remove(jsonconfig);
  swDelay(15);
  SPIFFS.remove(jsonschedule);
  swDelay(15);
  configplaying();
  scheduleplaying();
  //    AccidentOffplaying();
  loadAccidentOff(jsonAccident, currentValue);
  loadTotalRfid(jsonRFIDtotal, rfidfig);
  loadTotalHistory(jsonTotalHistory, history);
  printFile(jsonAccident);
  swDelay(15);
#if boardColour == 2
  iox.digitalWrite(resetPower, HIGH);
  delay(5000);
#elif boardColour == 1
  digitalWrite(resetPower, HIGH);
  delay(5000);
#else

#endif
  ESP.restart();
}
