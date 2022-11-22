const unsigned long eventIntervalRC = 30000;
unsigned long previousTimeRC = 0;

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

int16_t getRSSISignal(int16_t RSSI);
void scanWifi(void);

void reconnectWifiSta(void);

//void OnWiFiEvent(WiFiEvent_t event);
void initwifista(void);
void initwifiap(void);

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  debugln("Connected to AP successfully!");
  execReconnectWifi = false;
  wifiDisconnectCount = 0;
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  debugln("WiFi connected");
  debugln("IP address: ");
  debugln(WiFi.localIP());

}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  execReconnectWifi = true;
  WiFi.disconnect(true);

  //  WiFi.mode(WIFI_OFF);
  //  debugln("Disconnected from WiFi access point");
  //  debug("WiFi lost connection. Reason: ");
  //  debugln(info.disconnected.reason);
  //  debugln("Trying to Reconnect");
  //  WiFi.begin(String(config.ssid).c_str(), String(config.pass).c_str(), 1, 0, 1);
}

int16_t getRSSISignal(int16_t RSSI)
{
  int16_t quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

void scanWifi() {
  inScanFunc = true;
  ssidClear();
  WiFi.scanDelete();
  int numberOfNetworks = WiFi.scanNetworks();
  printf("////////////////////////////////////////////////////////////////////////////////////numberOfNetworks : %d \n", numberOfNetworks);
  if (numberOfNetworks == -2) {
    WiFi.scanNetworks(true);
  }

  else if (numberOfNetworks) {
    Serial.print("///////////////////////////scanWifi: Executing on core : ");
    Serial.println(xPortGetCoreID());
    Serial.print("Number of networks found: ");
    Serial.println(numberOfNetworks);

    for (int i = 0; i < numberOfNetworks; i++) {
      if (WiFi.SSID(i) == "") {
        Serial.println("No add");
      }
      else {
        Serial.print("Network name: ");
        Serial.println(WiFi.SSID(i));

        Serial.print("Signal strength: ");
        Serial.println(WiFi.RSSI(i));

        Serial.print("MAC address: ");
        Serial.println(WiFi.BSSIDstr(i));
        ssidAdd(WiFi.SSID(i) , getRSSISignal(WiFi.RSSI(i)));
      }

      //    Serial.print("Encryption type: ");
      //    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
      //    Serial.println(encryptionTypeDescription);
      //    Serial.println("-----------------------");
    }
  }
  //  isScanning = false;

  //  if (WiFi.scanNetworks() == -2) {
  //    WiFi.scanNetworks(true);
  //  }
  scantoBegin = true;
  delay(1000);
  WiFi.disconnect(true);
  WiFi.begin(String(config.ssid).c_str(), String(config.pass).c_str(), 1, 0, 1);

  //  while (WiFi.status() != WL_CONNECTED) {
  //    Serial.print('.');
  //    delay(500);
  //  }

  //  Serial.println (WiFi.localIP());
  debug("Local IP: "); debugln(WiFi.localIP());
  Serial.println("wifi begin in scan, wifi begin in scan, wifi begin in scan, wifi begin in scan, wifi begin in scan, wifi begin in scan, wifi begin in scan, wifi begin in scan");
  //          Serial.println(F("Heap 1 : ")); Serial.print(ESP.getFreeHeap());
  //    printf("Heap 1 : %d , Scan wifi reconnect to STA\n", ESP.getFreeHeap());

  delay(1000);

  scantoBegin = false;
  inScanFunc = false;
}

void reconnectWifiSta() {

  unsigned long currentTimeRC = millis();

  /* This is the event */
  if (currentTimeRC - previousTimeRC >= eventIntervalRC) {
    /* Event code */
    if (!isScanning && displayPage != 2) {
      if (WiFi.status() != WL_CONNECTED) {
        //      Serial.println("scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2");
        //  WiFi.begin("bug", "bug");
        //      scanWifi();
        WiFi.begin(String(config.ssid).c_str(), String(config.pass).c_str(), 1, 0, 1);
        Serial.print("///////////////////////////////////////////////////// wifiDisconnectCount : "); Serial.println(wifiDisconnectCount);
        //          Serial.println(F("Heap 1 : ")); Serial.print(ESP.getFreeHeap());
        printf("Heap 1 : %d \n", ESP.getFreeHeap());
      }
    }
    else if (isScanning && displayPage == 2) {

    }

    /* Update the timing for the next time around */
    previousTimeRC = currentTimeRC;
  }

}

//void OnWiFiEvent(WiFiEvent_t event)
//{
//  switch (event) {
//    case SYSTEM_EVENT_STA_CONNECTED:
//      debugln("ESP32 Connected to WiFi Network");
//      break;
//    case SYSTEM_EVENT_STA_DISCONNECTED:
//      debug("///////////////////////////////////////////////////// wifiDisconnectCount : ");debugln(wifiDisconnectCount);
//      if (wifiDisconnectCount == 10) {
//        debugln("Disconnected from WiFi access point");
//        debug("WiFi lost connection. Reason: ");
//        //      debugln(info.disconnected.reason);
//        debugln("Trying to Reconnect");
//        WiFi.begin(String(config.ssid).c_str(), String(config.pass).c_str());
//        wifiDisconnectCount = 0;
//      }
//      else {
//        wifiDisconnectCount++;
//      }
//      break;
//    case SYSTEM_EVENT_STA_GOT_IP:
//      debugln("WiFi connected");
//      debugln("IP address: ");
//      debugln(WiFi.localIP());
//      break;
//    //    case SYSTEM_EVENT_AP_START:
//    //      debugln("qESP32 soft AP started");
//    //      break;
//    case SYSTEM_EVENT_AP_STOP:
//      debugln("wESP32 soft AP Restarted");
//      WiFi.softAP(String(config.ssidap).c_str(), String(config.passap).c_str(), 1, 0, 1);
//      break;
//    case SYSTEM_EVENT_AP_STACONNECTED:
//      debugln("Station connected to ESP32 soft AP");
//      break;
//    case SYSTEM_EVENT_AP_STADISCONNECTED:
//      debugln("Station disconnected from ESP32 soft AP");
//      break;
//    default: break;
//  }
//}

void initwifiap() {

  debug("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  //  WiFi.softAP("rabbit", NULL , 1, 0, 1);
  uint8_t ip0 = getValue(String(config.ip), '.', 0).toInt();
  uint8_t ip1 = getValue(String(config.ip), '.', 1).toInt();
  uint8_t ip2 = getValue(String(config.ip), '.', 2).toInt();
  uint8_t ip3 = getValue(String(config.ip), '.', 3).toInt();

  swDelay(500);

  //  IPAddress IP = WiFi.softAPIP();
  IPAddress apIP(ip0, ip1, ip2, ip3);
  IPAddress apGW(192, 168, 1, 1);
  IPAddress apSN(255, 255, 255, 0);

  if (!WiFi.softAPConfig(apIP, apGW, apSN)) {
    //  if (!WiFi.softAPConfig(apIP)) {
    debugln("AP Config Failed");
    return;
  }
  // Show the local IP
  debug("AP IP address: ");
  IPAddress IP = WiFi.softAPIP();
  debugln(IP);

  WiFi.softAP(String(config.ssidap).c_str(), String(config.passap).c_str(), 1, 0, MAXCONNECTION);
  debugln(String(config.ssidap));
  debugln(String(config.passap));

}

void initwifista() {
  //  Serial.begin(115200);

  // delete old config
  //  WiFi.disconnect(true);

  delay(1000);
  //
  //  ESP_ERROR_CHECK(nvs_flash_init());
  //  scanWifi();
  //
  //  tcpip_adapter_init();
  //
  //  wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
  //  ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
  //  ESP_ERROR_CHECK(esp_wifi_start());// starts wifi usage

  /* Remove WiFi event
    debug("WiFi Event ID: ");
    debugln(eventID);
    WiFi.removeEvent(eventID);*/
  //  String specialPrefixBoard;
  //  String specialPrefix = "";
  //  String specialPrefixRTC;
  //  String build;
  String companyName = COMPANYNAME;
  String deviceCode = DEVICECODE;
  //  String mainControl = MAINCONTROLLER;
  //  String deviceName;

  //#if RTCIC == 1
  //  specialPrefixRTC = "3231";
  //#else
  //  specialPrefixRTC = "1307";
  //#endif
  //
  //#if boardColour == 1
  //  specialPrefixBoard = "B";
  //#else
  //  specialPrefixBoard = "G";
  //#endif
  //
  //#if EVCONTROL == 1
  //  build = "HPH";
  //#else
  //  build = "HWA";
  //#endif
  //
  //  specialPrefix = "S";
  //  specialPrefix += specialPrefixBoard;
  //  specialPrefix += specialPrefixRTC;


  String deviceName = "";
  deviceName += companyName;
  deviceName += "_";
  //  deviceName += build;
  //  deviceName += "_";
  //  deviceName += mainControl;
  //  deviceName += "_";
  deviceName += deviceCode;
  //  deviceCode += RELIGION;
  //  deviceCode += RELEASEDATE;
  //  deviceCode += REV;
  //  deviceName += "_";
  //  deviceName += "";

  //  WiFi.setHostname(String(config.ssidap).c_str());

  WiFi.begin(String(config.ssid).c_str(), String(config.pass).c_str());

  //  char hostname [10];
  //
  //  strcpy (hostname, "bluestoneEV");

  if (!MDNS.begin(config.domainName)) {
    debugln("error mdns");
    return;
  }

  debugln();
  debugln();
  debugln("Wait for WiFi... ");
  delay(1000);
  //  Serial.println("scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2 scan 2");
  //  WiFi.begin("bug","bug");
  //  scanWifi();
}
