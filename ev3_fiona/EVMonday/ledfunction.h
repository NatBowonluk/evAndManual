void initLED(void);
void ledState(uint8_t vehicleStatus) ;

void initLED() {
  ///////////// rfid reset pin
  //  pinMode(resetRfids, OUTPUT);
  //  pinMode(ledButton, OUTPUT);
#if boardColour == 2
#else
  pinMode(connectStatus, OUTPUT);
  pinMode(chargeStatus, OUTPUT);
  pinMode(errorStatus, OUTPUT);
#endif

  pinMode(rgbR, OUTPUT);
  pinMode(rgbG, OUTPUT);
  pinMode(rgbB, OUTPUT);

  //PWM properties to the PWM channels
  ledcSetup(redChannel, pwmFrequency, pwmResolution);
  ledcSetup(greenChannel, pwmFrequency, pwmResolution);
  ledcSetup(blueChannel, pwmFrequency, pwmResolution);

#if boardColour == 1
#else
  ledcSetup(buzzerChannel, buzzerFrequency, pwmResolution);
#endif

  //Attach the PWM channels to the corresponding GPIOs
  ledcAttachPin(rgbR, redChannel);
  ledcAttachPin(rgbG, greenChannel);
  ledcAttachPin(rgbB, blueChannel);
#if boardColour == 1
#else
  ledcAttachPin(buzzer, buzzerChannel);
#endif

}

//0 = stanby, 1 = chargenow button, 2 = stanby charginig yellow 3 = charging, 4 = Finish, 6 = schedule
void ledState(uint8_t vehicleStatus) {
//  DateTime now = rtc.now(); // Get current time
  uint16_t dd;
//  dd = now.day();
  dd = timetime.dateNow;
  swDelay(20);
  if (vehicleStatus == 6) {
    if (evlastStat == 0 || evlastStat == 1) {
      if (config.sleepSec == 0) {
        ledcWrite(redChannel, 9);
        ledcWrite(greenChannel, 235);
        ledcWrite(blueChannel, 238);
        config.isInSleep = 0;
#if boardColour == 2
#else
        digitalWrite(connectStatus, HIGH);
        digitalWrite(chargeStatus, HIGH);
        digitalWrite(errorStatus, HIGH);
#endif
      }
      else if (config.sleepSec > 0) {
        if (isActive >= config.sleepSec) {
          ledcWrite(redChannel, 0);
          ledcWrite(greenChannel, 0);
          ledcWrite(blueChannel, 0);
          //        Serial.println("ssssssssssssssssssssssssssssssssssssssssssssssss");
#if boardColour == 2
#else
          digitalWrite(connectStatus, HIGH);
          digitalWrite(chargeStatus, HIGH);
          digitalWrite(errorStatus, HIGH);
#endif

          config.isInSleep = 1;

          if ((hour() >= config.resetHourRangeStart && hour() <= config.resetHourRangeEnd) || (hour() >= 21 && hour() <= 22)) {
            if (config.firstSleepThisDay == 0) {
              config.firstSleepThisDay = 1;
              config.doneReset = 1;
              saveConfiguration(jsonconfig, config);
              swDelay(2000);

              if ((dd != 0) && (dd != config.currentDate)) {
                Serial.println(F("RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART AAAAAAAAAAAAAAA"));
                ESP.restart();
              }
              else {
                config.firstSleepThisDay = 1;
                Serial.println(F("RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART AAAAAAAAAAAAAAAA NNNNNNNNNNNNNNNNNNNN"));
              }
            }
            else if (config.firstSleepThisDay != 0) {
              Serial.println(F("firstSleepThisDay no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset"));
              if (config.currentDate != dd) {
                config.firstSleepThisDay = 0;
                if (dd != 0) {
                  config.currentDate = dd;
                  Serial.println(dd);
                  Serial.println(config.currentDate);
                  swDelay(500);
                  saveConfiguration(jsonconfig, config);
                }
                Serial.println(F("currentDate currentDate currentDate currentDate currentDate currentDate currentDate currentDate currentDate currentDate"));
              }
              else {
                Serial.println(F("nocurrentDate nocurrentDate nocurrentDate nocurrentDate nocurrentDate nocurrentDate nocurrentDate nocurrentDate"));
              }
            }
          }
          else {
            config.doneReset = 0;
            Serial.println(F("resetHourRangeEnd no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset"));
          }
        }
        else {
          ledcWrite(redChannel, 9);
          ledcWrite(greenChannel, 235);
          ledcWrite(blueChannel, 238);
          config.isInSleep = 0;
#if boardColour == 2
#else
          digitalWrite(connectStatus, HIGH);
          digitalWrite(chargeStatus, HIGH);
          digitalWrite(errorStatus, HIGH);
#endif
        }
      }
    }
    else if (evlastStat == 2 || evlastStat == 3) {
      ledcWrite(redChannel, 255);
      ledcWrite(greenChannel, 255);
      ledcWrite(blueChannel, 0);
      config.isInSleep = 0;
      isActive = 0;

      //      digitalWrite(ledButton, HIGH);
#if boardColour == 2
#else
      digitalWrite(connectStatus, LOW);
      digitalWrite(chargeStatus, HIGH);
      digitalWrite(errorStatus, HIGH);
#endif
    }
    else if (evlastStat == 5) {
      ledcWrite(redChannel, 0);
      ledcWrite(greenChannel, 0);
      ledcWrite(blueChannel, 255);
      isActive = 0;
      config.isInSleep = 0;
#if boardColour == 2
#else
      digitalWrite(connectStatus, LOW);
      digitalWrite(chargeStatus, LOW);
      digitalWrite(errorStatus, HIGH);
#endif
    }
    else if (evlastStat == 4) {
      ledcWrite(redChannel, 0);
      ledcWrite(greenChannel, 0);
      ledcWrite(blueChannel, 255);

      isActive = 0;
      config.isInSleep = 0;
#if boardColour == 2
#else
      digitalWrite(connectStatus, LOW);
      digitalWrite(chargeStatus, HIGH);
      digitalWrite(errorStatus, HIGH);
#endif
    }
    else if (evlastStat == 7) {
      isActive = 0;
      config.isInSleep = 0;
      ledcWrite(redChannel, 255);
      ledcWrite(greenChannel, 0);
      ledcWrite(blueChannel, 0);
#if boardColour == 2
#else
      digitalWrite(connectStatus, HIGH);
      digitalWrite(chargeStatus, HIGH);
      digitalWrite(errorStatus, LOW);
#endif
    }
  }
  else if (vehicleStatus == 0) {             ///////////////turquoise_stateA
    if (evlastStat == 0 || evlastStat == 1) {
      /////////////////////////////////////////////////////////////sleep mode
      if (config.sleepSec == 0) {
        ledcWrite(redChannel, 9);
        ledcWrite(greenChannel, 235);
        ledcWrite(blueChannel, 238);
        config.isInSleep = 0;
#if boardColour == 2
#else
        digitalWrite(connectStatus, HIGH);
        digitalWrite(chargeStatus, HIGH);
        digitalWrite(errorStatus, HIGH);
#endif
      }

      else if (config.sleepSec > 0) {
        if (isActive >= config.sleepSec) {
          ledcWrite(redChannel, 0);
          ledcWrite(greenChannel, 0);
          ledcWrite(blueChannel, 0);
          //        Serial.println("ssssssssssssssssssssssssssssssssssssssssssssssss");
#if boardColour == 2
#else
          digitalWrite(connectStatus, HIGH);
          digitalWrite(chargeStatus, HIGH);
          digitalWrite(errorStatus, HIGH);
#endif
          config.isInSleep = 1;

          if ((hour() >= config.resetHourRangeStart && hour() <= config.resetHourRangeEnd) || (hour() >= 21 && hour() <= 22)) {
            if (config.firstSleepThisDay == 0) {
              config.firstSleepThisDay = 1;
              config.doneReset = 1;
              saveConfiguration(jsonconfig, config);
              swDelay(2000);
              debug("////////////////////////////////////////// config.dd : "); debugln(dd);
              if ((dd != 0) && (dd != config.currentDate)) {
                Serial.println(F("RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART BBBBBBBBBBBBBBB"));
                ESP.restart();
              }
              else {
                config.firstSleepThisDay = 1;
                Serial.println(F("RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART RESTART BBBBBBBBBBBBBBB NNNNNNNNNNNNNNNNNNNN"));
              }
            }
            else if (config.firstSleepThisDay != 0) {
              Serial.println(F("firstSleepThisDay no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset"));
              if (config.currentDate != dd) {
                config.firstSleepThisDay = 0;
                debug("////////////////////////////////////////// config.dd : "); debugln(dd);
                if (dd != 0) {
                  config.currentDate = dd;
                  Serial.println(dd);
                  Serial.println(config.currentDate);
                  swDelay(500);
                  saveConfiguration(jsonconfig, config);
                }
                Serial.println(F("currentDate currentDate currentDate currentDate currentDate currentDate currentDate currentDate currentDate currentDate"));
              }
              else {
                Serial.println(F("nocurrentDate nocurrentDate nocurrentDate nocurrentDate nocurrentDate nocurrentDate nocurrentDate nocurrentDate"));
              }
            }
          }
          else {
            config.doneReset = 0;
            Serial.println(F("resetHourRangeEnd no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset no reset"));
          }
        }
        else {
          ledcWrite(redChannel, 9);
          ledcWrite(greenChannel, 235);
          ledcWrite(blueChannel, 238);
          config.isInSleep = 0;
#if boardColour == 2
#else
          digitalWrite(connectStatus, HIGH);
          digitalWrite(chargeStatus, HIGH);
          digitalWrite(errorStatus, HIGH);
#endif
        }
      }
    }
    else if (evlastStat == 2 || evlastStat == 3) {
      isActive = 0;

      ledcWrite(redChannel, 255);
      ledcWrite(greenChannel, 255);
      ledcWrite(blueChannel, 0);
      config.isInSleep = 0;
#if boardColour == 2
#else
      digitalWrite(connectStatus, LOW);
      digitalWrite(chargeStatus, HIGH);
      digitalWrite(errorStatus, HIGH);
#endif
    }
  }
  else if (vehicleStatus == 1 || vehicleStatus == 2 ||  vehicleStatus == 4) {          ////////////////yellow_stateB
    ledcWrite(redChannel, 255);
    ledcWrite(greenChannel, 255);
    ledcWrite(blueChannel, 0);
    config.isInSleep = 0;
    isActive = 0;

    //    digitalWrite(ledButton, HIGH);
#if boardColour == 2
#else
    digitalWrite(connectStatus, LOW);
    digitalWrite(chargeStatus, HIGH);
    digitalWrite(errorStatus, HIGH);
#endif
  }
  else if (vehicleStatus == 3) {               //////////////blue_stateC
    isActive = 0;
    config.isInSleep = 0;
    ledcWrite(redChannel, 0);
    ledcWrite(greenChannel, 0);
    ledcWrite(blueChannel, 255);
#if boardColour == 2
#else
    digitalWrite(connectStatus, LOW);
    digitalWrite(chargeStatus, LOW);
    digitalWrite(errorStatus, HIGH);
#endif
  }
  else if (vehicleStatus == 7) {                           //////////////////red_stateError
    ledcWrite(redChannel, 255);
    ledcWrite(greenChannel, 0);
    ledcWrite(blueChannel, 0);
    config.isInSleep = 0;
    isActive = 0;
#if boardColour == 2
#else
    digitalWrite(connectStatus, HIGH);
    digitalWrite(chargeStatus, HIGH);
    digitalWrite(errorStatus, LOW);
#endif
  }
}
