#if RFIDSALE == 1
//void rfidAuthen(void);
void rfidAuthenChargeNowFunc(void);
//currentValue
//  String RfidUseFname;
//  String RfidUseLname;
//  String RfidUseUid;

void rfidAuthenChargeNowFunc() {
  //  rfidAutenChargeNowButton = 0;
  //  rfidreadread();
  if (!rfidReadDone) {
    //    rfidStatus = 0;
    if (!execDisconnect) {

    }
    else if (execDisconnect) {
      rfidreadread();
    }

    if (nowUid != "timeout" && nowUid != "") {
      if (checkRFID(nowUid, jsonaccRFID)) {
#if boardColour == 2
        iox.digitalWrite(buzzer, HIGH);
#elif boardColour == 1
        digitalWrite(buzzer, HIGH);
#else
        ledcWrite(buzzerChannel, 135);
#endif
        vTaskDelay(100 / portTICK_PERIOD_MS);
#if boardColour == 2
        iox.digitalWrite(buzzer, LOW);
#elif boardColour == 1
        digitalWrite(buzzer, LOW);
#else
        ledcWrite(buzzerChannel, 0);
#endif
        vTaskDelay(100 / portTICK_PERIOD_MS);
#if boardColour == 2
        iox.digitalWrite(buzzer, HIGH);
#elif boardColour == 1
        digitalWrite(buzzer, HIGH);
#else
        ledcWrite(buzzerChannel, 135);
#endif
        vTaskDelay(100 / portTICK_PERIOD_MS);
#if boardColour == 2
        iox.digitalWrite(buzzer, LOW);
#elif boardColour == 1
        digitalWrite(buzzer, LOW);
#else
        ledcWrite(buzzerChannel, 0);
#endif
        debugln("dont have this card in data");
        if (execRfidAuthenChargeButton) {
          rfidAutenChargeNowButton = 0;
          rfidReadDone = false;
          debugln("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        }
        else if (!execRfidAuthenChargeButton) {
          debugln("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
          return;
        }
        //            rfidSend();
      }
      else if (!checkRFID(nowUid, jsonaccRFID)) {
        debugln("authen pass : have this card in system");
        rfidAutenChargeNowButton = 1;
        debugln("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
#if boardColour == 2
        iox.digitalWrite(buzzer, HIGH);
#elif boardColour == 1
        digitalWrite(buzzer, HIGH);
#else
        ledcWrite(buzzerChannel, 135);
#endif
        vTaskDelay(100 / portTICK_PERIOD_MS);
#if boardColour == 2
        iox.digitalWrite(buzzer, LOW);
#elif boardColour == 1
        digitalWrite(buzzer, LOW);
#else
        ledcWrite(buzzerChannel, 0);
#endif
        //         currentValue.RfidUseFname, currentValue.RfidUseLname, currentValue.RfidUseUid,
        //            rfidSend();
      }
    }
  }
}
#else
#endif
