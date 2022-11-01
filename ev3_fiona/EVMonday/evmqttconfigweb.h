void notFound(AsyncWebServerRequest *request);
void handleRoot(AsyncWebServerRequest *request);
void handleCSS(AsyncWebServerRequest *request);
void handleJS(AsyncWebServerRequest *request);
void handleBanner(AsyncWebServerRequest *request);
void handleCancel(AsyncWebServerRequest *request);
void handleCheck(AsyncWebServerRequest *request);
void handleScanRFID(AsyncWebServerRequest *request);
void handleWaitRFID(AsyncWebServerRequest *request);
void handleLogo(AsyncWebServerRequest *request);

void initWifiApServer(void);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}
void handleRoot(AsyncWebServerRequest *request)
{
  if (!request->authenticate(config.username, config.userpass))
  {
    return request->requestAuthentication();
  }
  Serial.println(F("Stream the array! : handleRoot"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, index_html_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}
void handleCSS(AsyncWebServerRequest *request)
{
  Serial.println(F("Stream the array! : handleCSS"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", style_css_gz, style_css_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}
void handleJS(AsyncWebServerRequest *request)
{
  Serial.println(F("Stream the array! : handleJS"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", javascript_js_gz, javascript_js_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}
void handleBanner(AsyncWebServerRequest *request)
{
  Serial.println(F("Stream the array! : handleBanner"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", banner_png, banner_png_len);
  request->send(response);
}
void handleCancel(AsyncWebServerRequest *request)
{
  Serial.println(F("Stream the array! : handleCancel"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", cancel_png, cancel_png_len);
  request->send(response);
}
void handleCheck(AsyncWebServerRequest *request)
{
  Serial.println(F("Stream the array! : handleCheck"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", check_png, check_png_len);
  request->send(response);
}
void handleScanRFID(AsyncWebServerRequest *request)
{
  Serial.println(F("Stream the array! : handleScanRFID"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", rfid_scan_png, rfid_scan_png_len);
  request->send(response);
}
void handleWaitRFID(AsyncWebServerRequest *request)
{
  Serial.println(F("Stream the array! : handleWaitRFID"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "image/gif", rfid_wait_gif, rfid_wait_gif_len);
  request->send(response);
}
void handleLogo(AsyncWebServerRequest *request)
{
  Serial.println(F("Stream the array! : handleLogo"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", logo_ico, logo_ico_len);
  request->send(response);
}

void initWifiApServer()
{
  //  WiFi.onEvent(OnWiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

  initwifiap();
  initwifista();
  mqttSetup(true, "174.138.19.183", 1883);

  server = new AsyncWebServer(config.port);

  server->on("/", handleRoot);
  server->on("/javascript.js", handleJS);
  server->on("/style.css", handleCSS);
  server->on("/banner.png", handleBanner);
  server->on("/check.png", handleCheck);
  server->on("/cancel.png", handleCancel);
  server->on("/rfid_scan.png", handleScanRFID);
  server->on("/rfid_wait.gif", handleWaitRFID);
  server->on("/logo.ico", handleLogo);

  server->on("/json/logger.json", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/js/logger.json", "text/javascript");
  });
  server->on("/factoryreset", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    factoryReset();
    request->send(200);
  });

  server->on("/deviceconfig", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    uint8_t * argsTotal = new uint8_t[1];
    argsTotal[0] = request->args();
    String *argValue = new String[argsTotal[0]];

    if (request->hasArg("apssid")) {
      argValue[0] = request->arg("apssid");
      strlcpy(config.ssidap, argValue[0].c_str(), sizeof(config.ssidap));
    }
    if (request->hasArg("appass")) {
      argValue[1] = request->arg("appass");
      strlcpy(config.passap, argValue[1].c_str(), sizeof(config.passap));
    }
    if (request->hasArg("stassid")) {
      argValue[2] = request->arg("stassid");
      strlcpy(config.ssid, argValue[2].c_str(), sizeof(config.ssid));
    }
    if (request->hasArg("stapass")) {
      argValue[3] = request->arg("stapass");
      strlcpy(config.pass, argValue[3].c_str(), sizeof(config.pass));
    }
    if (request->hasArg("domain")) {
      argValue[4] = request->arg("domain");
      strlcpy(config.domainName, argValue[4].c_str(), sizeof(config.domainName));
    }
    if (request->hasArg("mqttclientid")) {
      argValue[5] = request->arg("mqttclientid");
      strlcpy(config.mqttcilentid, argValue[5].c_str(), sizeof(config.mqttcilentid));
    }
    String *ipAll = new String[1];
    if (request->hasArg("mqtt1ip")) {
      argValue[6] = request->arg("mqtt1ip");
    }
    if (request->hasArg("mqtt2ip")) {
      argValue[7]  = request->arg("mqtt2ip");
    }
    if (request->hasArg("mqtt3ip")) {
      argValue[8] = request->arg("mqtt3ip");
    }
    if (request->hasArg("mqtt4ip")) {
      argValue[9] = request->arg("mqtt4ip");
      ipAll[0] = argValue[6];
      ipAll[0] += ".";
      ipAll[0] += argValue[7];
      ipAll[0] += ".";
      ipAll[0] += argValue[8];
      ipAll[0] += ".";
      ipAll[0] += argValue[9];
      strlcpy(config.mqttip, ipAll[0].c_str(), sizeof(config.mqttip));
    }

    if (request->hasArg("mqttport")) {
      argValue[10] = request->arg("mqttport");
      config.mqttport = atoi(argValue[10].c_str());
    }
    if (request->hasArg("mqttuser")) {
      argValue[11] = request->arg("mqttuser");
      strlcpy(config.mqttuser, argValue[11].c_str(), sizeof(config.mqttuser));
    }
    if (request->hasArg("mqttpass")) {
      argValue[12] = request->arg("mqttpass");
      strlcpy(config.mqttpass, argValue[12].c_str(), sizeof(config.mqttpass));
    }

    saveConfiguration(jsonconfig, config);
    Serial.println(F("Print config file..."));
    printFile(jsonconfig);


    request->send_P(200, "text/plain", "Setting Network Done");
    delete[] ipAll;
    delete[] argsTotal;
    delete[] argValue;

    delay(1000);
    Serial.println(F("Device is going to restart!!!"));
#if boardColour == 1
    Serial.println(F("HARD RESTART"));
    digitalWrite(resetPower, HIGH);
    delay(5000);
#else

#endif
    Serial.println(F("SOFT RESTART"));
    ESP.restart();
  });

  server->on("/evconfig", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    uint8_t * argsTotal = new uint8_t[1];
    argsTotal[0] = request->args();
    String *argValue = new String[argsTotal[0]];

    if (request->hasArg("meter")) {
      argValue[1] = request->arg("meter");
      config.phaseset = atoi(argValue[1].c_str());
    }

    if (request->hasArg("current")) {
      argValue[0] = request->arg("current");
      currentValue.currentlyAmp = atoi(argValue[0].c_str());
      config.currentmax = currentValue.currentlyAmp;
    }
    sharedModbusUse = true;

    Serial.print(F("////////////////////////// Current CHANGE A : ")); Serial.println(config.currentmax);
    Serial.print(F("////////////////////////// Current CHANGE B : ")); Serial.println(argValue[0]);

    do {
      swDelay(50);
      evUserSetCurrent(config.currentmax);
      swDelay(50);
      Serial.print(F("////////////////////////// Current CHANGE D : ")); Serial.println(evUserGetCurrent());
      swDelay(50);
    } while (evUserGetCurrent() == 0);

    //    swDelay(200);
    //    evUserSetCurrent(config.currentmax);
    swDelay(200);
    Serial.print(F("////////////////////////// Current CHANGE C : ")); Serial.println(evUserGetCurrent());



    saveConfiguration(jsonconfig, config);
    Serial.println(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/phaseset");
    String *DataSend = new String[1];
    DataSend[0] = "startchargingcurrent : ";
    DataSend[0] += argValue[0];
    request->send_P(200, "text/plain", DataSend[0].c_str());
    sharedModbusUse = false;
    delete[] DataSend;
    delete[] argsTotal;
    delete[] argValue;
  });

  server->on("/rfidconfig", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    uint8_t *argsTotal = new uint8_t[1];
    argsTotal[0] = request->args();
    String *argValue = new String[argsTotal[0]];

    if (request->hasArg("rfid")) {
      argValue[0] = request->arg("rfid");
      config.rfidusing = atoi(argValue[0].c_str());
    }

    if (config.rfidusing == 1) {
      currentValue.RfidUseFname = "RFID";
      currentValue.RfidUseLname = "Mode";
    }
    else if (config.rfidusing == 0) {
      currentValue.RfidUseFname = "Non RFID";
      currentValue.RfidUseLname = "Mode";
    }

    saveConfiguration(jsonconfig, config);
    Serial.println(F("Print config file..."));
    printFile(jsonconfig);

    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/rfidset");
    String *DataSend = new String[1];
    DataSend[0] = "RFID Mode : ";
    DataSend[0] += argValue[0];
    request->send_P(200, "text/plain", DataSend[0].c_str());

    delete[] DataSend;
    delete[] argsTotal;
    delete[] argValue;
  });

  server->on("/scannow", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    execRfidAdd = true;
    //    request->send(401);
    request->send(200);
  });
  server->on("/cancelscanrfid", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    execRfidAdd = false;
    isCancel = true;
    //    request->send(401);
    request->send(200);
  });

  server->on("/clearscanrfid", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    execRfidAdd = false;
    isCancel = true;
    rfidReadDone = false;
    nowUid = "";
    rfidStatus = 0;
    //    request->send(401);
    request->send(200);
  });


  server->on("/logout", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(401);
  });

  server->on("/alive", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    isActive = 0;
    isInWebSite = 0;
    //     Serial.println(F("alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive"));
    //    Serial.println(F("alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive alive"));
    request->send_P(200, "text/plain", "alive");
  });

  server->on("/rfidscanpage", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    String *DataSend = new String[1];
    DynamicJsonBuffer jsonBuffer;
    JsonObject &doc = jsonBuffer.createObject();

    doc["uid"] = nowUid;
    doc["status"] = rfidStatus;

    doc.printTo(DataSend[0]);
    request->send_P(200, "text/plain", DataSend[0].c_str());
    delete[] DataSend;
  });

  server->on("/deletemember", HTTP_GET, [](AsyncWebServerRequest * request)
  {

    uint8_t *argsTotal = new uint8_t[1];
    argsTotal[0] = request->args();
    String *argValue = new String[argsTotal[0]];
    String *DataSend = new String[1];

    if (request->hasArg("uid")) {
      argValue[0] = request->arg("uid");
      delUid = argValue[0];
    }

    Serial.println(F("DELETE MEMBER : "));
    Serial.println(delUid);
#if RFIDSALE == 1
    delAccountRFID(delUid, jsonaccRFID);
#else
#endif
    isRfidDelete = false;
    rfidStatus = 3;
    rfidReadDone = false;

    DataSend[0] = "delete : ";
    DataSend[0] += delUid;
    debugln("66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666/currentupdate");
    request->send_P(200, "text/plain", DataSend[0].c_str());

    delete[] DataSend;
    delete[] argsTotal;
    delete[] argValue;
  });

  server->on("/addmember", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    uint8_t *argsTotal = new uint8_t[1];
    argsTotal[0] = request->args();
    String *argValue = new String[argsTotal[0]];
    String *DataSend = new String[1];

    if (request->hasArg("fname")) {
      argValue[0] = request->arg("fname");
      rfidfig.rfidfname = argValue[0];
    }

    if (request->hasArg("lname")) {
      argValue[1] = request->arg("lname");
      rfidfig.rfidlname = argValue[1];
    }
    Serial.print(F("First Name : "));
    Serial.print(rfidfig.rfidfname);
    Serial.print(F("Last Name : "));
    Serial.println(rfidfig.rfidlname);
    //    Serial.print(F("//////////////////////////// Total RFID BEFORE ADD : ")); Serial.print(rfidfig.rfidtotal); Serial.println(F(" ////////////////////////////")); //higher 1
#if RFIDSALE == 1
    //    if (rfidfig.rfidtotal >= 11) {
    //      Serial.println(F("Done Nothing..."));
    //    }
    //    else {
    addAccountRFID(rfidfig.rfidfname, rfidfig.rfidlname, nowUid, jsonaccRFID);
    //    }
#else
#endif
    rfidGetInputName = false;
    rfidStatus = 0;
    rfidReadDone = false;
    //    rfidSend();

    DataSend[0] = "fname : ";
    DataSend[0] += rfidfig.rfidfname;
    DataSend[0] += ", lname : ";
    DataSend[0] += rfidfig.rfidlname;
    DataSend[0] += ", uid : ";
    DataSend[0] += nowUid;
    request->send_P(200, "text/plain", DataSend[0].c_str());
    delete[] DataSend;
    delete[] argsTotal;
    delete[] argValue;
  });

  server->on("/init", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    String *DataSend = new String[1];
    DynamicJsonBuffer jsonBuffer;
    JsonObject &doc = jsonBuffer.createObject();
    //    String *ipSeperate = new String[5];
    //    uint8_t *counter = new uint8_t[1];
    //    ipSeperate[4] = config.mqttip;
    //    for (counter[0] = 0; counter[0] < 4; counter[0]++)
    //    {
    //      ipSeperate[counter[0]] = getValue(ipSeperate[4], '.', counter[0]);
    //    }
    doc["accuser"] = config.username;
    doc["accpass"] = config.userpass;
    if (config.rfidusing == 0) {
      doc["rfidmode"] = 2;
    }
    else {
      doc["rfidmode"] = 1;
    }
    doc["meter"] = config.phaseset;
    if (config.currentmax == 16) {
      doc["current"] = 1;
    }
    else {
      doc["current"] = 2;
    }
    //    doc["current"] = config.currentmax;
    doc["apssid"] = config.ssidap;
    doc["appass"] = config.passap;
    doc["stassid"] = config.ssid;
    doc["stapass"] = config.pass;
    doc["domain"] = config.domainName;
    doc["mqttid"] = config.mqttcilentid;
    //    doc["mqttip1"] = atoi(ipSeperate[counter[0]].c_str());
    //    doc["mqttip2"] = atoi(ipSeperate[counter[1]].c_str());
    //    doc["mqttip3"] = atoi(ipSeperate[counter[2]].c_str());
    //    doc["mqttip4"] = atoi(ipSeperate[counter[3]].c_str());
    doc["mqttip"] = config.mqttip;
    doc["mqttport"] = config.mqttport;
    doc["mqttuser"] = config.mqttuser;
    doc["mqttpass"] = config.mqttpass;

    doc.printTo(DataSend[0]);
    request->send_P(200, "text/plain", DataSend[0].c_str());
    //    delete[] ipSeperate;
    //    delete[] counter;
    delete[] DataSend;
  });

  server->serveStatic("/", SPIFFS, "/");
  server->serveStatic("/js/wifistalist.json", SPIFFS, "/js/wifistalist.json");
  server->serveStatic("/js/logger.json", SPIFFS, "/js/logger.json");
  server->on("/js/logger.json", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/js/logger.json", "application/json");
  });
  server->onNotFound(notFound);

  server->begin();
}
