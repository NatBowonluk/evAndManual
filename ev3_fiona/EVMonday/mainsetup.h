void configplaying(void);
void scheduleplaying(void);
void AccidentOffplaying(void);
void TotalRFIDplaying(void);
void TotalHistoryplaying(void);

void configplaying()
{
  debugln(F("Loading configuration..."));
  loadConfiguration(jsonconfig, config);
  if (hasConfig == false)
  {
    debugln(F("Saving configuration..."));
    saveConfiguration(jsonconfig, config);
  }
  hasConfig = true;
  debugln(F("Print config file..."));
  printFile(jsonconfig);
  //  printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
  // printf("0Start Energy : %0.4f, Final Energy : %0.4f\n", atof(currentValue.ChargeStartEnergy.c_str()), electric.energy);
  //  printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
}

void scheduleplaying()
{
  debugln(F("Loading schedule..."));
  loadSchedule(jsonschedule, schtime);
  if (hasScheduleSet == false)
  {
    debugln(F("Saving schedule..."));
    saveSchedule(jsonschedule, schtime);
  }
  hasScheduleSet = true;
  debugln(F("Print schedule file..."));
  printFile(jsonschedule);
  //  printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
  // printf("9Start Energy : %0.4f, Final Energy : %0.4f\n", atof(currentValue.ChargeStartEnergy.c_str()), electric.energy);
  //  printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
}

void AccidentOffplaying()
{
  debugln(F("Loading Accident..."));
  loadAccidentOff(jsonAccident, currentValue);
  if (hasloadAccidentOffSet == false)
  {
    debugln(F("Saving Accident..."));
    saveAccidentOff(jsonAccident, currentValue);
  }
  hasloadAccidentOffSet = true;
  debugln(F("Print Accident file..."));
  printFile(jsonAccident);
  //  printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
  // printf("8Start Energy : %0.4f, Final Energy : %0.4f\n", atof(currentValue.ChargeStartEnergy.c_str()), electric.energy);
  // printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
}

void TotalHistoryplaying()
{
  debugln(F("Loading Total History..."));
  loadTotalHistory(jsonTotalHistory, history);
  if (hasloadHistorySet == false)
  {
    debugln(F("Saving Total History..."));
    createEmptyJson(jsonTotalHistory);
  }
  hasloadHistorySet = true;
  debugln(F("Print Total History file..."));
  printFile(jsonTotalHistory);
  //  printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
  // printf("7Start Energy : %0.4f, Final Energy : %0.4f\n", atof(currentValue.ChargeStartEnergy.c_str()), electric.energy);
  // printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
}

void TotalRFIDplaying()
{
  debugln(F("Loading Total RFID..."));
  loadTotalRfid(jsonRFIDtotal, rfidfig);
  if (hasloadTotalRFIDSet == false)
  {
    debugln(F("Saving Total RFID..."));
    createEmptyJson(jsonRFIDtotal);
  }
  hasloadTotalRFIDSet = true;
  debugln(F("Print Total RFID file..."));
  printFile(jsonRFIDtotal);
  //  printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
  //  printf("6Start Energy : %0.4f, Final Energy : %0.4f\n", atof(currentValue.ChargeStartEnergy.c_str()), electric.energy);
  //  printf("///////////////////////////////////////////////////////////////////////////////////////////////////\n");
}
