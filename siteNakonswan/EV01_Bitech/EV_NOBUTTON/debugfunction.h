#if RFIDSALE == 1
uint8_t checkRfidMode(void); //0 = nonrfid, 1 = rfidmode and authen pass, 2 = rfidmode but authen failed : authen with card

void rfidDebug() {
  debugln("start function");
  if (checkRFID("434567654345", jsonaccRFID) == true) {
    addAccountRFID("kae", "barry", "434567654345", jsonaccRFID);
  }
  delAccountRFID("34543456789", jsonaccRFID);
  printFile(jsonaccRFID);
  swDelay(1000);
}

uint8_t checkRfidMode() {
  //  rfidMode = config.rfidusing
  if (config.rfidusing == 0) { //non rfid mode
    startFunction = true;
    return 0;
  }
  else if (config.rfidusing == 1) {
    //scan RFID then
    rfidreadread();
    //had temporary var
    if (checkRFID(nowUid, jsonaccRFID) == true) {
      startFunction = true;
      debugln("Hello Member");
      return 1;
    }
    else {
      startFunction = false;
      debugln("You are not member");
      return 2;
    }
  }
}

void afterCheck(uint8_t stat) {
  if (stat == 0 || stat == 1) {  //////////////////////////this one represent that you can freely use program
    //start webserver
  }
  else if (stat == 2) {  ////////////////////////this one represent you use rfid authen but failed
    checkRfidMode();
  }
}

//void chargePageFunction() {
//  if (evGetMode() != 3) {
//    evSetMode(3);
//  }
//  else {
//    if (evCarGetStat() == ") {
//
//      }
//      }
//      }
#else
#endif
