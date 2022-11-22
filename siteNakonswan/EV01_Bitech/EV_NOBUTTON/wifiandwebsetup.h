void notFound(AsyncWebServerRequest *request);
void handleRoot(AsyncWebServerRequest * request);
void handleOTA(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
void handleOTApage(AsyncWebServerRequest * request);
void handleStatus(AsyncWebServerRequest * request);
void handleSetting(AsyncWebServerRequest * request);
void handleAddRfid(AsyncWebServerRequest * request);
void handleHistory(AsyncWebServerRequest * request);
void handleCSS(AsyncWebServerRequest * request);
void handleJquery(AsyncWebServerRequest * request);

void initWifiApServer(void);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}

void handleRoot(AsyncWebServerRequest * request) {
  //  execRfidAuthenChargeButton = true;
  //        execLedButtonBlink = true;
  //  execLedButtonBlink = false;
  //  if (rfidAutenChargeNowButton == 0) {
  //  if (evCarGetStat() == 2) {//authen out web
  //    rfidAutenChargeNowButton = 1;
  //  }
  //  else {
  //    rfidAutenChargeNowButton = 0;
  //  }
  //    execHardButton = true;
  //    execRfidAuthenChargeButton = false;
  //    digitalWrite(chargeLedPin, HIGH);
  //    debugln("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  //  }
#if boardColour == 1
  digitalWrite(buzzer, LOW);
#else
  ledcWrite(buzzerChannel, 0);
#endif
  nowUid = "";
  rfidGetInputName = false;
  rfidReadDone = false;
  displayPage = 0;
  execMeter = true;
  execEvSend = true;
  execSettingSend = false;
  execRfidAdd = false;
  execSettingWait = 0;
  execSetTimeWait = 1;
  execRfidAuthen = false;
  nowUid = "";
  rfidStatus = 0;
  connectOTA = false;
  const char* dataType = "text/html";

  debugln("Stream the array!");

  AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, index_html_gz, index_html_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}

void handleOTA(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{

  String logmessage;
  if (!index)
  {
    debugln("Update");
    //ota start
    if (!Update.begin())
    {
      Update.printError(Serial);
    }
  }

  if (len)
  {
    //ota process
    if (Update.write(data, len) != len)
    {
      Update.printError(Serial);
      return request->send(400, "text/plain", "OTA could not begin");
    }
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    debugln(logmessage);
  }

  if (final)
  {
    if (!Update.end(true))
    {
      Update.printError(Serial);
      return request->send(400, "text/plain", "Could not end OTA");
    }
    else
    {
      restartNow = true;
      return request->send(400, "text/plain", "OTA Done");
      //flag restart when OTA done
      //ota done
    }
  }
}


void handleOTApage(AsyncWebServerRequest * request) {
  //  rfidAutenChargeNowButton = 0;
#if boardColour == 1
  digitalWrite(buzzer, LOW);
#else
  ledcWrite(buzzerChannel, 0);
#endif
  nowUid = "";
  rfidGetInputName = false;
  rfidReadDone = false;
  displayPage = 5;
  execMeter = false;
  execEvSend = false;
  execRfidAdd = false;
  execSettingWait = 1;
  execSetTime = false;
  execSetTimeWait = 0;
  execRfidAuthen = false;
  rfidStatus = 0;
  nowUid = "";
  connectOTA = true;

  const char* dataType = "text/html";

  debugln("Stream the array!");

  if (!request->authenticate(otaUser, otaPass)) {
    return request->requestAuthentication();
  }
  AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, ota_html_gz, ota_html_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}


void handleStatus(AsyncWebServerRequest * request) {
  //  rfidAutenChargeNowButton = 0;
#if boardColour == 1
  digitalWrite(buzzer, LOW);
#else
  ledcWrite(buzzerChannel, 0);
#endif
  nowUid = "";
  rfidGetInputName = false;
  rfidReadDone = false;
  displayPage = 1;
  ///////////////////////////////////////////////////////
  //  initmeter(1);
  //////////////////////////////////////////////////////
  execMeter = true;
  execEvSend = true;
  execRfidAdd = false;
  execSettingSend = false;
  execSettingWait = 0;
  execSetTime = false;
  execSetTimeWait = 0;
  execRfidAuthen = false;
  nowUid = "";
  rfidStatus = 0;
  connectOTA = false;

  const char* dataType = "text/html";

  debugln("Stream the array!");
  //    if (config.rfidusing == 0) {

  if (config.phaseset == 1) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, statusnow_html_gz, statusnow_html_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  }
  else if (config.phaseset == 3) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, status3phasenow_html_gz, status3phasenow_html_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  }
}

void handleSetting(AsyncWebServerRequest * request) {
  //  rfidAutenChargeNowButton = 0;
#if boardColour == 1
  digitalWrite(buzzer, LOW);
#else
  ledcWrite(buzzerChannel, 0);
#endif
  nowUid = "";
  rfidGetInputName = false;
  rfidReadDone = false;
  displayPage = 2;
  execMeter = false;
  execEvSend = false;
  execRfidAdd = false;
  execSettingWait = 1;
  execSetTime = false;
  execSetTimeWait = 0;
  execRfidAuthen = false;
  rfidStatus = 0;
  nowUid = "";
  connectOTA = false;

  const char* dataType = "text/html";

  debugln("Stream the array!");
  //
  //  if (!request->authenticate(String(config.username).c_str(), String(config.userpass).c_str())) {
  //    return request->requestAuthentication();
  //  }
  AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, settings_html_gz, settings_html_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}

void handleAddRfid(AsyncWebServerRequest * request) {
  //  rfidAutenChargeNowButton = 0;
#if boardColour == 1
  digitalWrite(buzzer, LOW);
#else
  ledcWrite(buzzerChannel, 0);
#endif
  nowUid = "";
  rfidGetInputName = false;
  rfidReadDone = false;
  displayPage = 4;
  execMeter = false;
  execEvSend = false;
  ///////////////////////////////////
  execRfidAdd = false;
  ///////////////////////////////////////
  execSettingSend = false;
  execSettingWait = 0;
  execSetTime = false;
  execSetTimeWait = 0;
  execRfidAuthen = false;
  rfidStatus = 0;
  connectOTA = false;
  if (nowUid == "" || nowUid == "timeout") {
    rfidReadDone = false;
  }

  const char* dataType = "text/html";

  debugln("Stream the array!");

  AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, addrfid_html_gz, addrfid_html_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}

void handleHistory(AsyncWebServerRequest * request) {
  //  rfidAutenChargeNowButton = 0;
#if boardColour == 1
  digitalWrite(buzzer, LOW);
#else
  ledcWrite(buzzerChannel, 0);
#endif
  nowUid = "";
  rfidGetInputName = false;
  rfidReadDone = false;
  displayPage = 3;
  execMeter = false;
  execEvSend = false;
  execRfidAdd = false;
  execSettingSend = false;
  execSettingWait = 0;
  execSetTime = false;
  execSetTimeWait = 0;
  execRfidAuthen = false;
  rfidAuthenStatus = 0;
  nowUid = "";
  rfidStatus = 0;
  connectOTA = false;

  const char* dataType = "text/html";

  debugln("Stream the array!");
  AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, history_html_gz, history_html_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}

void handleCSS(AsyncWebServerRequest * request) {
#if boardColour == 1
  digitalWrite(buzzer, LOW);
#else
  ledcWrite(buzzerChannel, 0);
#endif
  const char* dataType = "text/css";

  debugln("Stream the array!");
  AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, style_css_gz, style_css_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}

void handleJquery(AsyncWebServerRequest * request) {
#if boardColour == 1
  digitalWrite(buzzer, LOW);
#else
  ledcWrite(buzzerChannel, 0);
#endif
  const char* dataType = "text/html";

  debugln("Stream the array!");
  AsyncWebServerResponse *response = request->beginResponse_P(200, dataType, jquery_js_gz, jquery_js_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}

void initWifiApServer() {
  //  WiFi.onEvent(OnWiFiEvent);
  WiFi.disconnect(true);
  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.mode(WIFI_MODE_APSTA);
  initwifiap();
  initwifista();

  server = new AsyncWebServer(config.port);

  server->on("/", handleRoot);
  server->on("/updateFirmware", handleOTApage);
  server->on("/StatusPage", handleStatus);
  server->on("/SettingPage", handleSetting);
  server->on("/SignUpPage", handleAddRfid);
  server->on("/Historypage", handleHistory);
  server->on("/jquery.js", handleJquery);
  server->on("/style.css", handleCSS);
  server->on("/upload", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->send(200);
  }, handleOTA);
  server->on("/json/logger.json", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/js/logger.json", "text/javascript");
  });
  server->on("/factoryreset", HTTP_GET, [](AsyncWebServerRequest * request) {
    factoryReset();
    request->send(200);
  });

  server->on("/scanrfid", HTTP_GET, [](AsyncWebServerRequest * request) {
    execRfidAdd = true;
    //    request->send(401);
    request->send(200);
  });
  server->on("/cancelscanrfid", HTTP_GET, [](AsyncWebServerRequest * request) {
    execRfidAdd = false;
    isCancel = true;
    //    request->send(401);
    request->send(200);
  });


  server->on("/logout", HTTP_GET, [](AsyncWebServerRequest * request) {
    isAuthen = false;
    //    request->send(401);
    request->send(200);
  });

  server->on("/logoutupdatefw", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    isAuthen = false;
    request->send(401);
    //    request->send(200);
  });

  server->on("/clearhistory", HTTP_GET, [](AsyncWebServerRequest * request) {
    logClear();
    request->send(200);
  });
  server->on("/settingfirst", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    rfidAutenChargeNowButton = 0;
    String DataSend;

    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    //    root["rfidMode"] = config.rfidusing;
    uint32_t calcSleep;

    if (config.sleepSec == 0) {
      calcSleep = 0;
    }
    else if (config.sleepSec == 30) {
      calcSleep = 1;
    }
    else if (config.sleepSec == 60) {
      calcSleep = 2;
    }
    else if (config.sleepSec == 5 * 60) {
      calcSleep = 3;
    }
    else if (config.sleepSec == 10 * 60) {
      calcSleep = 4;
    }
    else if (config.sleepSec == 15 * 60) {
      calcSleep = 5;
    }
    else if (config.sleepSec == 30 * 60) {
      calcSleep = 6;
    }
    else if (config.sleepSec == 60 * 60) {
      calcSleep = 7;
    }
    else if (config.sleepSec == 90 * 60) {
      calcSleep = 8;
    }
    else if (config.sleepSec == 120 * 60) {
      calcSleep = 9;
    }

    if (config.rfidusing == 1) {
      String rfidNameBuff;
      rfidNameBuff += currentValue.RfidUseFname;
      rfidNameBuff += " ";
      rfidNameBuff += currentValue.RfidUseLname;
      root["isAuthen"] = isAuthen;
      root["rfidName"] = rfidNameBuff;
      root["username"] = config.username;
      root["password"] = config.userpass;
      root["rfidMode"] = config.rfidusing;
      root["powerMeter"] = config.phaseset;
      root["ssidSta"] = config.ssid;
      root["passSta"] = config.pass;
      root["ssidAp"] = config.ssidap;
      root["passAp"] = config.passap;
      root["ip"] = config.ip;
      root["domainName"] = config.domainName;
      rfidNameBuff = "";
      root["currentMax"] = config.currentmax;
      root["sleepSec"] = calcSleep;
    }
    else if (config.rfidusing == 0) {
      root["rfidName"] = "";
      root["isAuthen"] = isAuthen;
      root["username"] = config.username;
      root["password"] = config.userpass;
      root["rfidMode"] = config.rfidusing;
      root["powerMeter"] = config.phaseset;
      root["ssidSta"] = config.ssid;
      root["passSta"] = config.pass;
      root["ssidAp"] = config.ssidap;
      root["passAp"] = config.passap;
      root["ip"] = config.ip;
      root["domainName"] = config.domainName;
      root["currentMax"] = config.currentmax;
      root["sleepSec"] = calcSleep;
    }
    root["Mode"] = currentValue.webChargeStatus;

    root["saleRFID"] = RFIDSALE;
//    rfidPageForAdd
//    rfidPageForAddLine
    root.printTo(DataSend);
    debugln("777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777/settingfirst");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/checkmode", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    rfidAutenChargeNowButton = 0;
    String DataSend;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["rfidMode"] = config.rfidusing;
    root["Mode"] = currentValue.webChargeStatus;
    root["Status"] = evlastStat;

    root.printTo(DataSend);
    debugln("999999999999999999999999999999999999999999999999999999999999999999999999999999999999/checkmode");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/historyfirst", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    rfidAutenChargeNowButton = 0;
    String DataSend;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["rfidMode"] = config.rfidusing;
    root["Mode"] = currentValue.webChargeStatus;
    root["Status"] = evlastStat;
    root.printTo(DataSend);
    debugln("999999999999999999999999999999999999999999999999999999999999999999999999999999999999/history");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/indexfirst", HTTP_GET, [](AsyncWebServerRequest * request) {
    String DataSend;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    Serial.print("Setup: Executing on core ");
    Serial.println(xPortGetCoreID());
    root["rfidMode"] = config.rfidusing;
    root["rfidAuthen"] = rfidAutenChargeNowButton;
    if (config.rfidusing == 1) {
      String rfidNameBuff;
      rfidNameBuff += currentValue.RfidUseFname;
      rfidNameBuff += " ";
      rfidNameBuff += currentValue.RfidUseLname;
      root["rfidName"] = rfidNameBuff;
      root["Mode"] = currentValue.webChargeStatus;
      root["Status"] = evlastStat;
      root["flagSubmit"] = currentValue.isChargeSettingDone;
      root["userCurrentSet"] = currentValue.currentlyAmp;
      rfidNameBuff = "";
      root["rfidIndexStage"] = rfidIndexStage;
    }
    else if (config.rfidusing == 0) {
      root["rfidName"] = "";
      root["Mode"] = currentValue.webChargeStatus;
      root["Status"] = evlastStat;
      root["flagSubmit"] = currentValue.isChargeSettingDone;
      root["userCurrentSet"] = currentValue.currentlyAmp;
    }
    if (currentValue.webChargeStatus == 7) {
      root["errorTemptext"] = errorTemptext;
    }
    //    if (currentValue.webChargeStatus == 6) {
    root["ScheduleTime"] = schtime.schfulltext;
    //    root["ScheduleAlways"] =
    //    }

    //    if (evlastStat == 2) { //authen oug web
    //      rfidAutenChargeNowButton = 1;
    //    }
    //    else if (evlastStat == 0 || evlastStat == 1) {
    //      rfidAutenChargeNowButton = 0;
    //    }

    root["staConnection"] = execReconnectWifi;

    root["temperature"] = evTempShared;

    if (!execReconnectWifi) {
      root["ipSTA"] = ipToString(WiFi.localIP());
    }

    root.printTo(DataSend);
    debugln("333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333/indexfirst");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/currentupdate", HTTP_GET, [](AsyncWebServerRequest * request) {
    String currentBuffer = request->getParam(1)->value();

    currentValue.currentlyAmp = currentBuffer.toInt();;
    execEvSetCurrent = true;

    String DataSend;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["userCurrentSet"] = currentValue.currentlyAmp;

    root.printTo(DataSend);
    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/currentupdate");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/currentshow", HTTP_GET, [](AsyncWebServerRequest * request) {
    String DataSend;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["Mode"] = currentValue.webChargeStatus;

    root["userCurrentSet"] = currentValue.currentlyAmp;
    root["maxCurrentSet"] = config.currentmax;
    root["ScheduleAlways"] = schtime.schalw;
    root["ScheduleFlag"] = schtime.schflg;
    root["ScheduleTime"] = schtime.schfulltext;

    root.printTo(DataSend);
    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/currentshow");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/statusonefirst", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    rfidAutenChargeNowButton = 0;
    String DataSend;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["rfidMode"] = config.rfidusing;
    root["Status"] = evlastStat;
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
      root["TimeCount"] = TimeDuration;

      debugln(TimeDuration);
    }
    else {
      root["TimeCount"] = "--:--:--";
    }

    if (currentValue.webChargeStatus == 6) {
      root["ScheduleTime"] = schtime.schfulltext;
    }


    if (config.rfidusing == 1) {
      String rfidNameBuff;
      rfidNameBuff += currentValue.RfidUseFname;
      rfidNameBuff += " ";
      rfidNameBuff += currentValue.RfidUseLname;
      root["rfidName"] = rfidNameBuff;
      rfidNameBuff = "";
    }
    else if (config.rfidusing == 0) {
      root["rfidName"] = "";
    }

    String combinedTime = "";
    if (currentValue.startHH < 10) {
      combinedTime += "0";
      combinedTime += currentValue.startHH;
    }
    else {
      combinedTime += currentValue.startHH;
    }
    combinedTime += ":";
    if (currentValue.startMN < 10) {
      combinedTime += "0";
      combinedTime += currentValue.startMN;
    }
    else {
      combinedTime += currentValue.startMN;
    }

    root["startTime"] = combinedTime;

    root["Mode"] = currentValue.webChargeStatus;
    root["Energy"] = electric.energy - atof(currentValue.ChargeStartEnergy.c_str());

    root["Voltage"] = electric.voltage1;
    root["Current"] = electric.current1;
    root["Power"] = electric.power1;


    root.printTo(DataSend);
    debugln("1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111/statusonefirst");
    request->send_P(200, "text/plain", DataSend.c_str());
  });
  server->on("/statusthreefirst", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    rfidAutenChargeNowButton = 0;
    String DataSend;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    root["rfidMode"] = config.rfidusing;
    root["Status"] = evlastStat;
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
      root["TimeCount"] = TimeDuration;

      debugln(TimeDuration);
    }
    else {
      root["TimeCount"] = "--:--:--";
    }

    if (currentValue.webChargeStatus == 6) {
      root["ScheduleTime"] = schtime.schfulltext;
    }


    if (config.rfidusing == 1) {
      String rfidNameBuff;
      rfidNameBuff += currentValue.RfidUseFname;
      rfidNameBuff += " ";
      rfidNameBuff += currentValue.RfidUseLname;
      root["rfidName"] = rfidNameBuff;
      rfidNameBuff = "";
    }
    else if (config.rfidusing == 0) {
      root["rfidName"] = "";
    }

    String combinedTime = "";
    if (currentValue.startHH < 10) {
      combinedTime += "0";
      combinedTime += currentValue.startHH;
    }
    else {
      combinedTime += currentValue.startHH;
    }
    combinedTime += ":";
    if (currentValue.startMN < 10) {
      combinedTime += "0";
      combinedTime += currentValue.startMN;
    }
    else {
      combinedTime += currentValue.startMN;
    }

    root["startTime"] = combinedTime;

    root["Mode"] = currentValue.webChargeStatus;
    root["Energy"] = electric.energy - atof(currentValue.ChargeStartEnergy.c_str());

    root["Voltage"] = electric.voltage3[0];
    root["Current"] = electric.current3[0];
    root["Power"] = electric.power3[0];

    root["Voltage2"] = electric.voltage3[1];
    root["Current2"] = electric.current3[1];
    root["Power2"] = electric.power3[1];

    root["Voltage3"] = electric.voltage3[2];
    root["Current3"] = electric.current3[2];
    root["Power3"] = electric.power3[2];


    root.printTo(DataSend);
    debugln("22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222/statusthreefirst");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/stealthsettime", HTTP_GET, [](AsyncWebServerRequest * request) {
    String hours = request->getParam(1)->value();
    String minute = request->getParam(2)->value();
    String second = request->getParam(3)->value();
    String day = request->getParam(4)->value();
    String month = request->getParam(5)->value();
    String year = request->getParam(6)->value();
    uint8_t Month;
    debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
    debug(day);  debug('/');  debug(month);  debug('/');  debugln(year);
    debug(hours);  debug(':');  debug(minute);  debug(':');  debugln(second);
    //debugln(String(request->getParam(0)->value()));
    //debugln(String(request->getParam(1)->value()));
    //debugln(String(request->getParam(2)->value()));
    //debugln(String(request->getParam(3)->value()));
    //debugln(String(request->getParam(4)->value()));
    //debugln(String(request->getParam(5)->value()));
    //debugln(String(request->getParam(6)->value()));
    //debugln(String(request->getParam(7)->value()));
    //debugln(String(request->getParam(8)->value()));
    //debugln(String(request->getParam(9)->value()));
    //debugln(String(request->getParam(10)->value()));
    //debugln(String(request->getParam(11)->value()));
    debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
    if (month == "January") {
      Month = 1;
    }
    else if (month == "February") {
      Month = 2;
    }
    else if (month == "March") {
      Month = 3;
    }
    else if (month == "April") {
      Month = 4;
    }
    else if (month == "May") {
      Month = 5;
    }
    else if (month == "June") {
      Month = 6;
    }
    else if (month == "July") {
      Month = 7;
    }
    else if (month == "August") {
      Month = 8;
    }
    else if (month == "September") {
      Month = 9;
    }
    else if (month == "October") {
      Month = 10;
    }
    else if (month == "November") {
      Month = 11;
    }
    else if (month == "December") {
      Month = 12;
    }
    String Datasend;
    if (currentValue.webChargeStatus == 6 && (execRfidAuthenChargeButton || execRfidAdd)) {
      Datasend = "in mode 6 and rfid, dont stealth set";
    }
    else if (currentValue.webChargeStatus != 6 && (!execRfidAuthenChargeButton || !execRfidAdd)) {
      /*
      if (!fisrtStealthSetTime) {
        //        swDelay(15);
        adjustRTC(year.toInt() , Month, day.toInt(), hours.toInt(), minute.toInt(), second.toInt());
        //        swDelay(15);
        adjustDeviceTime(year.toInt() , Month, day.toInt(), hours.toInt(), minute.toInt(), second.toInt());
        fisrtStealthSetTime = true;
        debugln("set time set time set time set time set time set time set time set time set time set time set time");
      }
      else {
        if(countforsettime == stealthtime){
          fisrtStealthSetTime = false;
        }
        countforsettime++;       
        Datasend = "already set time, dont stealth set";
        debugln("dont set time dont set time dont set time dont set time dont set time dont set time dont set time");
      }
      */

      Datasend += year.toInt();
      Datasend += "/";
      Datasend += Month;
      Datasend += "/";
      Datasend += day.toInt();
      Datasend += " ";
      Datasend += hours.toInt();
      Datasend += ":";
      Datasend += minute.toInt();
      Datasend += ":";
      Datasend += second.toInt();
     
    }

    debugln(Datasend);
    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/stealthsettime");
    request->send_P(200, "text/plain", Datasend.c_str());
  });

  server->on("/logindone", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    String isAuthenBuffer = request->getParam(1)->value();

    //    if(isAuthenBuffer == "0"){
    isAuthen = true;
    //    }

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/logindone");
    String DataSend;
    DataSend = "isAuthen : " + isAuthen;
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/logincancel", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    String isAuthenBuffer = request->getParam(1)->value();

    //    if(isAuthenBuffer == "0"){
    isAuthen = false;
    //    }

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/logincancel");
    String DataSend;
    DataSend = "isAuthen : " + isAuthen;
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/userset", HTTP_GET, [](AsyncWebServerRequest * request) {
    String userIDBuffer = request->getParam(1)->value();
    String userPassBuffer = request->getParam(2)->value();

    debug("user : "); debugln(userIDBuffer);
    debug("pass : "); debugln(userPassBuffer);

    //    uint8_t userIDBufferlen = userIDBuffer.length() + 1;
    //    uint8_t userPassBufferlen = userPassBuffer.length() + 1;
    //
    //    char userIDarray[userIDBufferlen];
    //    char userPassarray[userPassBufferlen];
    //
    //    userIDBuffer.toCharArray(userIDarray, userIDBufferlen);
    //    userPassBuffer.toCharArray(userPassarray, userPassBufferlen);

    strlcpy(config.username, userIDBuffer.c_str(), sizeof(config.username));
    strlcpy(config.userpass, userPassBuffer.c_str(), sizeof(config.userpass));

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/userset");
    String DataSend;
    DataSend = "userIDBuffer : " + String(userIDBuffer.c_str()) + ", userPassBuffer : " + String(userPassBuffer.c_str());
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/rfidset", HTTP_GET, [](AsyncWebServerRequest * request) {
    String rfidSetBuffer = request->getParam(1)->value();

    debug("rfidset : "); debugln(rfidSetBuffer);

    uint8_t rfidSetBufferNum = rfidSetBuffer.toInt();

    if (rfidSetBufferNum == 1) {
      currentValue.RfidUseFname = "RFID";
      currentValue.RfidUseLname = "Mode";
    }
    else if (rfidSetBufferNum == 0) {
      currentValue.RfidUseFname = "Non RFID";
      currentValue.RfidUseLname = "Mode";
    }
    config.rfidusing = rfidSetBufferNum;

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/rfidset");
    String DataSend;
    DataSend = "rfidset : " + String(rfidSetBufferNum);
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/phaseset", HTTP_GET, [](AsyncWebServerRequest * request) {
    String phaseSetBuffer = request->getParam(1)->value();

    debug("phaseset : "); debugln(phaseSetBuffer);

    uint8_t phaseSetBufferNum = phaseSetBuffer.toInt();

    config.phaseset = phaseSetBufferNum;

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/phaseset");
    String DataSend;
    DataSend = "phaseset : " + String(phaseSetBufferNum);
    request->send_P(200, "text/plain", DataSend.c_str());
  });


  server->on("/wifiset", HTTP_GET, [](AsyncWebServerRequest * request) {
    String ssidSTABuffer = request->getParam(1)->value();
    String passSTABuffer = request->getParam(2)->value();
    String ssidAPBuffer = request->getParam(3)->value();
    String passAPBuffer = request->getParam(4)->value();
    String domainNameBuffer = request->getParam(5)->value();

    debug("ssidSTABuffer : "); debugln(ssidSTABuffer);
    debug("passSTABuffer : "); debugln(passSTABuffer);
    debug("ssidAPBuffer : "); debugln(ssidAPBuffer);
    debug("passAPBuffer : "); debugln(passAPBuffer);
    debug("domainNameBuffer : "); debugln(domainNameBuffer);

    strlcpy(config.ssid, ssidSTABuffer.c_str(), sizeof(config.ssid));
    strlcpy(config.pass, passSTABuffer.c_str(), sizeof(config.pass));
    strlcpy(config.ssidap, ssidAPBuffer.c_str(), sizeof(config.ssidap));
    strlcpy(config.passap, passAPBuffer.c_str(), sizeof(config.passap));
    strlcpy(config.domainName, domainNameBuffer.c_str(), sizeof(config.domainName));

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/phaseset");
    debugln("Restart");
    String DataSend;
    DataSend = "ssidSTAarray : " + String(ssidSTABuffer.c_str()) + "passSTAarray : " + String(passSTABuffer.c_str()) + "ssidAParray : " + String(ssidAPBuffer.c_str()) + "passAParray : " + String(passAPBuffer.c_str()) + "domainNamearray : " + String(domainNameBuffer.c_str());
    request->send_P(200, "text/plain", DataSend.c_str());
    delay(1000);
    debugln("going to restart");
#if boardColour == 1
    digitalWrite(resetPower, HIGH);
    delay(5000);
#else

#endif
    debugln("Hard power RESTART");
    ESP.restart();
  });

  server->on("/portset", HTTP_GET, [](AsyncWebServerRequest * request) {
    String portBuffer = request->getParam(1)->value();

    debug("portBuffer : "); debugln(portBuffer);

    uint16_t portBufferNum = portBuffer.toInt();

    config.port = portBufferNum;

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/phaseset");
    debugln("Restart");
    String DataSend;
    DataSend = "portBuffer" + String(portBuffer.c_str());
    request->send_P(200, "text/plain", DataSend.c_str());
    delay(1000);
    debugln("going to restart");
#if boardColour == 1
    digitalWrite(resetPower, HIGH);
    delay(5000);
#else

#endif
    debugln("Hard power RESTART");
    ESP.restart();
  });

  server->on("/sleeprangeset", HTTP_GET, [](AsyncWebServerRequest * request) {
    String startBuffer = request->getParam(1)->value();
    String endBuffer = request->getParam(1)->value();

    debug("startBuffer : "); debugln(startBuffer);
    debug("endBuffer : "); debugln(endBuffer);

    uint8_t startBufferNum = startBuffer.toInt();
    uint8_t endBufferNum = endBuffer.toInt();

    config.resetHourRangeStart = startBufferNum;
    config.resetHourRangeEnd = endBufferNum;

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/sleeprangeset");
    debugln("Restart");
    String DataSend;
    DataSend = "startBuffer" + String(startBuffer.c_str()) + "endBuffer" + String(endBuffer.c_str()) ;
    request->send_P(200, "text/plain", DataSend.c_str());
    delay(1000);
    debugln("going to restart");
#if boardColour == 1
    digitalWrite(resetPower, HIGH);
    delay(5000);
#else

#endif
    debugln("Hard power RESTART");
    ESP.restart();
  });

  server->on("/chargebutton", HTTP_GET, [](AsyncWebServerRequest * request) {
    String hours = request->getParam(4)->value();
    String minute = request->getParam(5)->value();
    String second = request->getParam(6)->value();
    String day = request->getParam(1)->value();
    String month = request->getParam(2)->value();
    String year = request->getParam(3)->value();
    uint8_t Month;
    debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
    debug(day);  debug('/');  debug(month);  debug('/');  debugln(year);
    debug(hours);  debug(':');  debug(minute);  debug(':');  debugln(second);
    debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
    if (month == "January") {
      Month = 1;
    }
    else if (month == "February") {
      Month = 2;
    }
    else if (month == "March") {
      Month = 3;
    }
    else if (month == "April") {
      Month = 4;
    }
    else if (month == "May") {
      Month = 5;
    }
    else if (month == "June") {
      Month = 6;
    }
    else if (month == "July") {
      Month = 7;
    }
    else if (month == "August") {
      Month = 8;
    }
    else if (month == "September") {
      Month = 9;
    }
    else if (month == "October") {
      Month = 10;
    }
    else if (month == "November") {
      Month = 11;
    }
    else if (month == "December") {
      Month = 12;
    }

    String Datasend;

    Datasend += year.toInt();
    Datasend += "/";
    Datasend += Month;
    Datasend += "/";
    Datasend += day.toInt();
    Datasend += " ";
    Datasend += hours.toInt();
    Datasend += ":";
    Datasend += minute.toInt();
    Datasend += ":";
    Datasend += second.toInt();

    currentValue.isChargeSettingDone = true;
    currentValue.isPressedC = true;

    //    schtime.schalw = false;
    //    schtime.schflg = false;

    chargeTimeWeb.yearStart = year.toInt();
    chargeTimeWeb.monthStart = Month;
    chargeTimeWeb.dateStart = day.toInt();
    chargeTimeWeb.hoursStart = hours.toInt();
    chargeTimeWeb.minuiteStart = minute.toInt();
    chargeTimeWeb.secoundStart = second.toInt();

    rfidAutenChargeNowButton = 1;

    saveSchedule(jsonschedule, schtime);
    swDelay(15);
    evUserSetCurrent(config.currentmax);
    //    String Datasend = "chargebutton";
    debugln(Datasend);
    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/chargebutton");
    request->send_P(200, "text/plain", Datasend.c_str());
  });

  server->on("/schedulecancel", HTTP_GET, [](AsyncWebServerRequest * request) {
    String alwaysScheduleBuffer = request->getParam(1)->value();
    //    String flagScheduleBuffer = request->getParam(2)->value();

    if (alwaysScheduleBuffer == "0" || alwaysScheduleBuffer == "false") {
      schtime.schalw = false;
      schtime.schflg = false;
      currentValue.scheduleStart = false;
      cancelSchedule();
    }
    else if (alwaysScheduleBuffer == "1" || alwaysScheduleBuffer == "true") {
      schtime.schalw = true;
      schtime.schflg = true;
      currentValue.scheduleStart = true;
    }


    currentValue.isChargeSettingDone = false;
    currentValue.isPressedC = false;
    //    schtime.schalw = false;
    //    schtime.schflg = false;

    saveSchedule(jsonschedule, schtime);

    String Datasend = "schedulecancel";
    debugln(Datasend);
    Serial.println("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/schedulecancel");
    request->send_P(200, "text/plain", Datasend.c_str());
  });

  server->on("/scheduleclear", HTTP_GET, [](AsyncWebServerRequest * request) {
    String alwaysScheduleBuffer = request->getParam(1)->value();
    //    String flagScheduleBuffer = request->getParam(2)->value();

    //    if (alwaysScheduleBuffer == "0" || alwaysScheduleBuffer == "false") {
    //      schtime.schalw = false;
    //      schtime.schflg = false;
    //      currentValue.scheduleStart = false;
    //      cancelSchedule();
    //    }
    //    else if (alwaysScheduleBuffer == "1" || alwaysScheduleBuffer == "true") {
    //      schtime.schalw = true;
    //      schtime.schflg = true;
    //      currentValue.scheduleStart = true;
    //    }
    currentValue.scheduleStart = false;
    cancelSchedule();
    schtime.schfulltext = "unknown";
    currentValue.isChargeSettingDone = false;
    currentValue.isPressedC = false;
    schtime.schalw = false;
    schtime.schflg = false;
    schtime.clearSchedule = true;
    saveSchedule(jsonschedule, schtime);

    String Datasend = "scheduleclear";
    debugln(Datasend);
    Serial.println("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/scheduleclear");
    request->send_P(200, "text/plain", Datasend.c_str());
  });

  server->on("/schedulealwayschange", HTTP_GET, [](AsyncWebServerRequest * request) {
    String alwaysScheduleBuffer = request->getParam(1)->value();

    if (alwaysScheduleBuffer == "0" || alwaysScheduleBuffer == "false") {
      schtime.schalw = false;
    }
    else if (alwaysScheduleBuffer == "1" || alwaysScheduleBuffer == "true") {
      schtime.schalw = true;
    }

    debug("alwaysScheduleBuffer : "); debugln(schtime.schalw);
    String Datasend = "schedulealwayschange : ";
    Datasend += schtime.schalw;
    debugln(Datasend);
    Serial.println("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/schedulealwayschange");
    request->send_P(200, "text/plain", Datasend.c_str());
  });

  server->on("/schedulesubmit", HTTP_GET, [](AsyncWebServerRequest * request) {
    currentValue.isChargeSettingDone = true;

    schtime.schflg = true;
    schtime.clearSchedule = false;
    String hours = request->getParam(4)->value();
    String minute = request->getParam(5)->value();
    String second = request->getParam(6)->value();
    String day = request->getParam(1)->value();
    String month = request->getParam(2)->value();
    String year = request->getParam(3)->value();

    String scheduleBuffer = request->getParam(7)->value();
    String alwaysScheduleBuffer = request->getParam(8)->value();

    if (alwaysScheduleBuffer == "0" || alwaysScheduleBuffer == "false") {
      schtime.schalw = false;
    }
    else if (alwaysScheduleBuffer == "1" || alwaysScheduleBuffer == "true") {
      schtime.schalw = true;
    }

    schtime.schhr = getValue(scheduleBuffer, ':', 0).toInt();
    schtime.schmm = getValue(scheduleBuffer, ':', 1).toInt();
    schtime.schfulltext = scheduleBuffer;

    uint8_t Month;
    debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
    debug(day);  debug('/');  debug(month);  debug('/');  debugln(year);
    debug(hours);  debug(':');  debug(minute);  debug(':');  debugln(second);
    debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
    if (month == "January") {
      Month = 1;
    }
    else if (month == "February") {
      Month = 2;
    }
    else if (month == "March") {
      Month = 3;
    }
    else if (month == "April") {
      Month = 4;
    }
    else if (month == "May") {
      Month = 5;
    }
    else if (month == "June") {
      Month = 6;
    }
    else if (month == "July") {
      Month = 7;
    }
    else if (month == "August") {
      Month = 8;
    }
    else if (month == "September") {
      Month = 9;
    }
    else if (month == "October") {
      Month = 10;
    }
    else if (month == "November") {
      Month = 11;
    }
    else if (month == "December") {
      Month = 12;
    }

    String Datasend;

    Datasend += year.toInt();
    Datasend += "/";
    Datasend += Month;
    Datasend += "/";
    Datasend += day.toInt();
    Datasend += " ";
    Datasend += hours.toInt();
    Datasend += ":";
    Datasend += minute.toInt();
    Datasend += ":";
    Datasend += second.toInt();

    chargeTimeWeb.yearStart = year.toInt();
    chargeTimeWeb.monthStart = Month;
    chargeTimeWeb.dateStart = day.toInt();
    chargeTimeWeb.hoursStart = hours.toInt();
    chargeTimeWeb.minuiteStart = minute.toInt();
    chargeTimeWeb.secoundStart = second.toInt();

    saveSchedule(jsonschedule, schtime);
    printFile(jsonschedule);

    //    String Datasend;
    Datasend = "schedulesubmit : " + schtime.schfulltext;
    debugln(Datasend);
    Serial.println("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/schedulesubmit");
    request->send_P(200, "text/plain", Datasend.c_str());
  });

  server->on("/deleterfidmember", HTTP_GET, [](AsyncWebServerRequest * request) {
    String uidBuffer = request->getParam(1)->value();
    delUid = uidBuffer;

    debugln("////////////////////////////////////////////////////////");
    debugln(delUid);
#if RFIDSALE == 1
    delAccountRFID(delUid, jsonaccRFID);
#else
#endif
    isRfidDelete = false;
    rfidStatus = 3;
    rfidReadDone = false;

    String DataSend = "delete : " + delUid;
    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/currentupdate");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/addrfidmember", HTTP_GET, [](AsyncWebServerRequest * request) {
    String fnameBuffer = request->getParam(1)->value();
    String lnameBuffer = request->getParam(2)->value();
    String rfidStatusBuffer = request->getParam(3)->value();

    rfidfig.rfidfname = fnameBuffer;
    rfidfig.rfidlname = lnameBuffer;

    debugln(rfidfig.rfidfname);
    debugln(rfidfig.rfidlname);
#if RFIDSALE == 1
    addAccountRFID(rfidfig.rfidfname, rfidfig.rfidlname, nowUid, jsonaccRFID);
#else
#endif
    rfidGetInputName = false;
    rfidStatus = 0;
    rfidReadDone = false;
    //    rfidSend();

    String DataSend = "fname : " + rfidfig.rfidfname + ", lname : " + rfidfig.rfidlname + ", uid : " + nowUid;
    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/currentupdate");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/addrfidfirst", HTTP_GET, [](AsyncWebServerRequest * request) {
    String DataSend;
    DynamicJsonBuffer jsonBuffer;
    JsonObject &doc = jsonBuffer.createObject();

    doc["uid"] = nowUid;
    doc["status"] = rfidStatus;

    doc.printTo(DataSend);
    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/currentupdate");
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/scanwifistart", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    isScanning = true;
    //    Serial.print("Setup: Executing on core /scanwifistart : ");
    //    Serial.println(xPortGetCoreID());
    //    scanWifiList();
    isScanning = true;
    scanWifi();
    delay(300);
    debug("Local IP 2: "); debugln(WiFi.localIP());
    //    WiFi.begin(String(config.ssid).c_str(), String(config.pass).c_str(), 1, 0, 1);
    Serial.println("scan doneeeeeeeeeeeeee");

    //    isScanning = false;


    //    Serial.println("9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999/scanwifistart");
    debugln("9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999/scanwifistart");
    //    while (WiFi.status() != WL_CONNECTED) {
    //      Serial.println('.');
    //      TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    //      TIMERG0.wdt_feed = 1;
    //      TIMERG0.wdt_wprotect = 0;
    //      delay(500);
    //    }
    //    delay(2000);
    //    Serial.println("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd");
    //    delay(2000);
    //    Serial.println("dddddaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaadfadcsddddddddddddddds");
    //    delay(4000);
    request->send_P(200, "text/plain", "scan done");
    isScanning = false;
    TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed = 1;
    TIMERG0.wdt_wprotect = 0;
    Serial.println("2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222/scanwifistart");
  });

  server->on("/startchargingcurrent", HTTP_GET, [](AsyncWebServerRequest * request) {
    String startchargingcurrentBuffer = request->getParam(1)->value();

    debug("startchargingcurrent : "); debugln(startchargingcurrentBuffer);

    uint8_t startchargingcurrentBufferNum = startchargingcurrentBuffer.toInt();

    config.currentmax = startchargingcurrentBufferNum;

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/phaseset");
    String DataSend;
    DataSend = "startchargingcurrent : " + String(startchargingcurrentBufferNum);
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/alive", HTTP_GET, [](AsyncWebServerRequest * request) {
    isActive = 0;
    isInWebSite = 0;
    String DataSend = "alive";
    //     Serial.println(F("alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive"));
    Serial.println(F("alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive"));
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/getrightfirmwarename", HTTP_GET, [](AsyncWebServerRequest * request) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    String specialPrefixBoard;
    String specialPrefix = "";
    String specialPrefixRTC;
    String build;
    String companyName = COMPANYNAME;
    String deviceCode = DEVICECODE;
    String mainControl = MAINCONTROLLER;

#if RTCIC == 1
    specialPrefixRTC = "3231";
#else
    specialPrefixRTC = "1307";
#endif

#if boardColour == 1
    specialPrefixBoard = "B";
#else
    specialPrefixBoard = "G";
#endif

#if EVCONTROL == 1
    build = "HPH";
#else
    build = "HWA";
#endif

    specialPrefix = "S";
    specialPrefix += specialPrefixBoard;
    specialPrefix += specialPrefixRTC;

    root["companyMode"] = companyName;
    root["build"] = build;
    root["mainControl"] = mainControl;
    root["deviceCode"] = deviceCode;
    root["specialPrefix"] = specialPrefix;

    String DataSend = "";
    root.printTo(DataSend);
    request->send_P(200, "text/plain", DataSend.c_str());
  });

  server->on("/sleepmodeset", HTTP_GET, [](AsyncWebServerRequest * request) {
    isActive = 0;

    String sleepmodesetBuffer = request->getParam(1)->value();

    debug("sleepmodesetBuffer : "); debugln(sleepmodesetBuffer);

    uint32_t sleepmodesetBufferNum = sleepmodesetBuffer.toInt();
    uint32_t calcSleep;

    if (sleepmodesetBufferNum == 0) {
      calcSleep = 0;
    }
    else if (sleepmodesetBufferNum == 1) {
      calcSleep = 30 ;
    }
    else if (sleepmodesetBufferNum == 2) {
      calcSleep = 60 ;
    }
    else if (sleepmodesetBufferNum == 3) {
      calcSleep = 5 * 60 ;
    }
    else if (sleepmodesetBufferNum == 4) {
      calcSleep = 10 * 60 ;
    }
    else if (sleepmodesetBufferNum == 5) {
      calcSleep = 15 * 60;
    }
    else if (sleepmodesetBufferNum == 6) {
      calcSleep = 30 * 60;
    }
    else if (sleepmodesetBufferNum == 7) {
      calcSleep = 60 * 60;
    }
    else if (sleepmodesetBufferNum == 8) {
      calcSleep = 90 * 60 ;
    }
    else if (sleepmodesetBufferNum == 9) {
      calcSleep = 120 * 60 ;
    }

    config.sleepSec = calcSleep;

    saveConfiguration(jsonconfig, config);
    debugln(F("Print config file..."));
    printFile(jsonconfig);

    String DataSend = "sleepmodesetBuffer : ";
    DataSend += calcSleep;

    request->send_P(200, "text/plain", DataSend.c_str());
  });

  //  server->on("/rfidindexupdate", HTTP_GET, [](AsyncWebServerRequest * request) {
  //    String rfidCmd = request->getParam(1)->value();
  //    uint8_t rfidIndexStageBuff = rfidCmd.toInt();
  //
  //    Serial.println(rfidIndexStageBuff);
  //
  //    String DataSend;
  //    DynamicJsonBuffer jsonBuffer;
  //    JsonObject &doc = jsonBuffer.createObject();
  //    String rfidNameBuff;
  //    rfidNameBuff += currentValue.RfidUseFname;
  //    rfidNameBuff += " ";
  //    rfidNameBuff += currentValue.RfidUseLname;
  //
  //    doc["rfidName"] = rfidNameBuff;
  //    doc["rfidIndexStage"] = rfidIndexStage;
  //
  //    doc.printTo(DataSend);
  //    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/currentupdate");
  //    request->send_P(200, "text/plain", DataSend.c_str());
  //  });

  server->serveStatic("/", SPIFFS, "/");
  server->serveStatic("/js/wifistalist.json", SPIFFS, "/js/wifistalist.json");
  server->onNotFound(notFound);

  server->begin();
  //  server->begin();  // it will start webserver
  //  websockets.begin();
  //  websockets.onEvent(webSocketEvent);
}
