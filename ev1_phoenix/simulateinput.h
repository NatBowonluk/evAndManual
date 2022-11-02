void simulate(void);
void printprintf(void);

char rx_byte = 0;

void simulate() {
//  if (Serial.available() > 0) {    // is a character available?
//    rx_byte = Serial.read();       // get the character
//    //simulateCar
//    if (rx_byte == '1') {
//      currentValue.isPressedC = true;
//      //      Serial.printf(F("Simulate : Button : %d\n", currentValue.isPressedC));
//      Serial.print(F("Simulate : Button : ")); Serial.println(currentValue.isPressedC);
//    }
//    else if (rx_byte == '2') {
//      currentValue.isPressedC = false;
//      //      Serial.printf(F("Simulate : Button : %d\n", currentValue.isPressedC));
//      Serial.print(F("Simulate : Button : ")); Serial.println(currentValue.isPressedC);
//    }
//    else if (rx_byte == '3') {
//      currentValue.isChargeSettingDone = true;
//      //      Serial.printf(F("Simulate : Setting : %d\n", currentValue.isChargeSettingDone));
//      Serial.print(F("Simulate : Setting : ")); Serial.println(currentValue.isChargeSettingDone);
//    }
//    else if (rx_byte == '4') {
//      currentValue.isChargeSettingDone = false;
//      //      Serial.printf(F("Simulate : Setting : %d\n", currentValue.isChargeSettingDone));
//      Serial.print(F("Simulate : Setting : ")); Serial.println(currentValue.isChargeSettingDone);
//    }
//    else if (rx_byte == '5') {
//      //      rfidSend();
//      Serial.println(F("rfidSend : done\n"));
//    }
//    else if (rx_byte == '9') {
//      simulateCar = false;
//      //      Serial.printf(F("Simulate : CarStatusMode : %d\n", simulateCar));
//      Serial.print(F("Simulate : CarStatusMode : ")); Serial.println(simulateCar);
//    }
//    else if (rx_byte == '0') {
//      simulateCar = true;
//      //      Serial.printf(F("Simulate : CarStatusMode : %d\n", simulateCar));
//      Serial.print(F("Simulate : CarStatusMode : ")); Serial.println(simulateCar);
//    }
//    else if (rx_byte == 'a') {
//      simulateCarValue = 0;
//      //      Serial.printf(F("Simulate : CarStatus : %d\n", simulateCarValue));
//      Serial.print(F("Simulate : CarStatus : ")); Serial.println(simulateCarValue);
//    }
//    else if (rx_byte == 's') {
//      simulateCarValue = 1;
//      //      Serial.printf(F("Simulate : CarStatus : %d\n", simulateCarValue));
//      Serial.print(F("Simulate : CarStatus : ")); Serial.println(simulateCarValue);
//    }
//    else if (rx_byte == 'd') {
//      simulateCarValue = 2;
//      //      Serial.printf(F("Simulate : CarStatus : %d\n", simulateCarValue));
//      Serial.print(F("Simulate : CarStatus : ")); Serial.println(simulateCarValue);
//    }
//    else if (rx_byte == 'f') {
//      simulateCarValue = 3;
//      //      Serial.printf(F("Simulate : CarStatus : %d\n", simulateCarValue));
//      Serial.print(F("Simulate : CarStatus : ")); Serial.println(simulateCarValue);
//    }
//    else if (rx_byte == 'g') {
//      simulateCarValue = 4;
//      //      Serial.printf(F("Simulate : CarStatus : %d\n", simulateCarValue));
//      Serial.print(F("Simulate : CarStatus : ")); Serial.println(simulateCarValue);
//    }
//    else if (rx_byte == 'h') {
//      simulateCarValue = 5;
//      //      Serial.printf(F("Simulate : CarStatus : %d\n", simulateCarValue));
//      Serial.print(F("Simulate : CarStatus : ")); Serial.println(simulateCarValue);
//    }
//    else if (rx_byte == 'j') {
//      simulateCarValue = 6;
//      //      Serial.printf(F("Simulate : CarStatus : %d\n", simulateCarValue));
//      Serial.print(F("Simulate : CarStatus : ")); Serial.println(simulateCarValue);
//    }
//    else if (rx_byte == 'z') {
//      Serial.println(F("ESP.restart"));
//      ESP.restart();
//    }
//    else if (rx_byte == '/') {
//      generalDebugFlag = true;
//      //      Serial.printf(F("Debug : %d\n", generalDebugFlag));
//      Serial.print(F("Debug : ")); Serial.println(generalDebugFlag);
//    }
//    else if (rx_byte == '.') {
//      generalDebugFlag = false;
//      //      Serial.printf(F("Debug : %d\n", generalDebugFlag));
//      Serial.print(F("Debug : ")); Serial.println(generalDebugFlag);
//    }
//  }
}

void generalDebug() {
  //  Serial.println(F());
  Serial.println(F("///////////////////////////////////////////////////////////////"));
  Serial.println(F("_______________________________________________________________"));
  Serial.println(F("Congiguration"));
  //  Serial.printf(F("Login ID -> User : %s, Pass : %s\n", String(config.username), String(config.userpass)));
  Serial.print(F("Login ID -> User : ")); Serial.print(String(config.username)); Serial.print(F(", Pass : ")); Serial.println(String(config.userpass));
  //  Serial.printf(F("WiFi STA -> Ssid : %s, Pass : %s\n", String(config.ssid), String(config.pass)));
  Serial.print(F("WiFi STA -> Ssid : ")); Serial.print(String(config.ssid)); Serial.print(F(", Pass : ")); Serial.println(String(config.pass));
  //  Serial.printf(F("WiFi AP -> Ssid : %s, Pass : %s\n", String(config.ssidap), String(config.passap)));
  Serial.print(F("WiFi AP -> Ssid : ")); Serial.print(String(config.ssidap)); Serial.print(F(", Pass : ")); Serial.println(String(config.passap));
  //  Serial.printf(F("Domain -> http://%s.local:%d/\n", String(config.domainName), config.port));
  Serial.print(F("Domain -> http://")); Serial.print(String(config.domainName)); Serial.print(F(".local:")); Serial.println(config.port);
  //  Serial.printf(F("IP AP -> %s, IP STA -> %s\n", String(WiFi.softAPIP()), String(WiFi.localIP())));
  Serial.print(F("IP AP -> ")); Serial.print(ipToString(WiFi.softAPIP())); Serial.print(F(", IP STA -> ")); Serial.println(ipToString(WiFi.localIP()));
  //  Serial.printf(F("Mode -> Rfid : %d, Phase : %d\n", String(config.rfidusing), String(config.phaseset)));
  Serial.print(F("Mode -> Rfid : ")); Serial.print(config.rfidusing); Serial.print(F(", Phase : ")); Serial.println(config.phaseset);
  //  Serial.printf(F("Time RTC Now -> %d:%d:%d %d/%d/%d \n", timetime.hoursNow, timetime.minuiteNow, timetime.secoundNow, timetime.dateNow, timetime.monthNow, timetime.yearNow));
  Serial.print(F("Time RTC Now -> ")); Serial.print(timetime.hoursNow); Serial.print(F(":")); Serial.print(timetime.minuiteNow); Serial.print(F(":")); Serial.print(timetime.secoundNow); Serial.print(F(" ")); Serial.print(timetime.dateNow); Serial.print(F("/")); Serial.print(timetime.monthNow); Serial.print(F("/")); Serial.println(timetime.yearNow);
  //  Serial.printf(F("Time ESP Now -> %d:%d:%d\n", hour(), minute(), second()));
  Serial.print(F("Time ESP Now -> ")); Serial.print(hour()); Serial.print(F(":")); Serial.print(minute()); Serial.print(F(":")); Serial.println(second());
  Serial.println(F("_______________________________________________________________"));
  Serial.println(F("Charging History"));
  //  Serial.printf(F("Delta Energy -> Now : %0.4f - Start : %0.4f\n", electric.energy, atof(currentValue.ChargeStartEnergy.c_str())));
  Serial.print(F("Delta Energy -> Now : ")); Serial.print(electric.energy); Serial.print(F(" - Start : ")); Serial.println(atof(currentValue.ChargeStartEnergy.c_str()));
  //  Serial.printf(F("Start Charging Time -> %d:%d:%d\n", currentValue.startChargeHR, currentValue.startChargeMN, currentValue.startChargeSS));
  Serial.print(F("Start Charging Time -> ")); Serial.print(currentValue.startChargeHR); Serial.print(F(":")); Serial.print(currentValue.startChargeMN); Serial.print(F(":")); Serial.println(currentValue.startChargeSS);
  Serial.println(F("_______________________________________________________________"));
  Serial.println(F("Electric Vehicle Status"));
  if (!simulateCar) {
    //    Serial.printf(F("Now : %d, Last : %d, Current : %d, Coil : %d\n", evCarGetStat(), evlastStat, evUserGetCurrent(), evGetCoil()));
    Serial.print(F("Now : ")); Serial.print(evCarGetStat()); Serial.print(F(", Last : ")); Serial.print(evlastStat); Serial.print(F(", Current : ")); Serial.print(evUserGetCurrent()); Serial.print(F(", Coil : ")); Serial.println(evGetCoil());
  }
  else {
    //    Serial.printf(F("Now : %d, Last : %d, Current : Simulate, Coil : Simulate\n", simulateCarValue, evlastStat));
    Serial.print(F("Now : ")); Serial.print(evCarGetStat()); Serial.print(F(", Last : ")); Serial.print(evlastStat); Serial.println(F(", Current : Simulate, Coil : Simulate"));
  }
  Serial.print(F("ErrorStatus : ")); Serial.println(errorTemptext);
  Serial.println(F("_______________________________________________________________"));
  Serial.println(F("Controller Status"));
  //  Serial.printf(F("Car : %d, Setting : %d, Button : %d, ,Schedule : %d, Daily : %d, Web : %d\n", currentValue.isStartOrFinish, currentValue.isChargeSettingDone, currentValue.isPressedC, schtime.schflg, schtime.schalw, currentValue.webChargeStatus));
  Serial.print(F("Car : ")); Serial.print(currentValue.isStartOrFinish); Serial.print(F(", Setting : ")); Serial.print(currentValue.isChargeSettingDone); Serial.print(F(", Button : ")); Serial.print(currentValue.isPressedC); Serial.print(F(", Schedule : ")); Serial.print(schtime.schflg); Serial.print(F(", Daily : ")); Serial.print(schtime.schalw); Serial.print(F(", Web : ")); Serial.println(currentValue.webChargeStatus);
  Serial.println(F("_______________________________________________________________"));
  Serial.println(errorMakeSureCount);
  Serial.println(F("_______________________________________________________________"));
  if (ev.getVehicleStatus(vehicleStatusBufferForDebug)) {
    Serial.print(F("Pure Vehicle Status Function : ")); Serial.println(vehicleStatusBufferForDebug);
  }
  Serial.println(F("_______________________________________________________________"));
  Serial.println(F("///////////////////////////////////////////////////////////////"));
}
