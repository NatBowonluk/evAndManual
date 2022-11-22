void notFound(AsyncWebServerRequest *request);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type)
  {
    case WStype_DISCONNECTED:
      isOpenWeb = false;
      printf("[%u] Disconnected!\n", num);
      execDisconnect = true;
      execRfidAdd = false;
      rfidAutenChargeNowButton = 0;
      break;
    case WStype_CONNECTED: {
        debugln("connected connected connected connected connected connected connected connected connected connected");
        if (displayPage == 4) {
          //          execRfidAdd = true;
          execRfidAdd = false;
        }
        isOpenWeb = true;
        execDisconnect = false;
        disconnectWeb = 0;
        IPAddress ip = websockets.remoteIP(num);
        //            execRfidAuthenChargeButton = false;
        printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        if (execSettingWait == 1) {
          execSettingWait = 2;
          swDelay(100);
          execSettingSend = true;
        }
        if (execSetTimeWait == 1) {
          execSetTimeWait = 2;
          swDelay(15);
          execSetTime = true;
        }
      }
      break;
    case WStype_TEXT:
      //      debugln("message message message message message message message message message message message");
      printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      debugln(message);

      recvFunction(message);
  }
}

void serverDisconnect() {
  if (rfidAuthenStatus == 1) {
    if (disconnectWeb == 30) {
      disconnectWeb = 0;
      rfidAuthenStatus = 0;
    }
    else {
      disconnectWeb++;
    }
  }
  debug("disconnectWeb Count : "); debug(disconnectWeb); debug("  rfidAuthenStatus : "); debugln(rfidAuthenStatus);
}
