void initSPIFFS(void);
void initstartupvalue(void);
void initsensor(void);
void initserial(void);

void initSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    debugln("An error has occurred while mounting SPIFFS");
  }
  debugln("SPIFFS mounted successfully");
}

void initstartupvalue() {
  configplaying();
  scheduleplaying();
  AccidentOffplaying();
  TotalRFIDplaying();
  TotalHistoryplaying();
  loadAccRfid();
  loadHistory();
  //  loadAccidentOff(jsonAccident, currentValue);
  //  loadTotalRfid(jsonRFIDtotal, rfidfig);
  //  loadTotalHistory(jsonTotalHistory, history);
  //  printFile(jsonAccident);
}

void initsensor() {

  initLED();
  config.phasesetold = config.phaseset;
  //  rtcinit();
#if RFIDSALE == 1
  rfidinit();
#else
#endif
  /////////////////////////////////////////////////init meter address 2
#if EVCONTROL == 1
  initmeter(1);
  evinit();
#elif EVCONTROL == 0 && boardColour == 2
  while (!Serial1);
  delay(1000);
  evinit();

  if (evCarGetStat() == 0 || evCarGetStat() == 1) {
    ev.setPhaseMeter(config.phaseset);
  }
  delay(200);
#else
  while (!Serial2);
  delay(1000);
  evinit();

  if (evCarGetStat() == 0 || evCarGetStat() == 1) {
    ev.setPhaseMeter(config.phaseset);
  }
#endif

}

void initserial() {
  Serial.begin(115200);
#if EVCONTROL == 1
  Serial2.begin(9600);
#elif EVCONTROL == 0 && boardColour == 2
  Serial1.begin(115200, SERIAL_8N1, 26, 27);
  delay(300);
  debugln("b = 2");
#else
  Serial2.begin(115200);
  delay(300);
  debugln("b = 1");
#endif

}
