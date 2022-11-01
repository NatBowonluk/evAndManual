void initLED(void);
void ledState(uint8_t vehicleStatus) ;

void initLED() {
#if boardColour == 1
#else
  ledcSetup(buzzerChannel, buzzerFrequency, pwmResolution);
#endif
#if boardColour == 1
#else
  ledcAttachPin(buzzer, buzzerChannel);
#endif

}

//0 = stanby, 1 = chargenow button, 2 = stanby charginig yellow 3 = charging, 4 = Finish, 6 = schedule
void ledState(uint8_t vehicleStatus) {

  uint16_t dd;
  dd = timetime.dateNow;
  swDelay(20);
  if (vehicleStatus == 6) {
    if (evlastStat == 0 || evlastStat == 1) {
      if (config.sleepSec == 0) {

        config.isInSleep = 0;

      }
      else if (config.sleepSec > 0) {
        if (isActive >= config.sleepSec) {

          //        Serial.println("ssssssssssssssssssssssssssssssssssssssssssssssss");


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

          config.isInSleep = 0;

        }
      }
    }
    else if (evlastStat == 2 || evlastStat == 3) {

      config.isInSleep = 0;
      isActive = 0;


    }
    else if (evlastStat == 5) {

      isActive = 0;
      config.isInSleep = 0;

    }
    else if (evlastStat == 4) {

      isActive = 0;
      config.isInSleep = 0;

    }
    else if (evlastStat == 7) {
      isActive = 0;
      config.isInSleep = 0;

    }
  }
  else if (vehicleStatus == 0) {             ///////////////turquoise_stateA
    if (evlastStat == 0 || evlastStat == 1) {
      /////////////////////////////////////////////////////////////sleep mode
      if (config.sleepSec == 0) {

        config.isInSleep = 0;

      }

      else if (config.sleepSec > 0) {
        if (isActive >= config.sleepSec) {

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

          config.isInSleep = 0;

        }
      }
    }
    else if (evlastStat == 2 || evlastStat == 3) {
      isActive = 0;


      config.isInSleep = 0;

    }
  }
  else if (vehicleStatus == 1 || vehicleStatus == 2 ||  vehicleStatus == 4) {          ////////////////yellow_stateB

    config.isInSleep = 0;
    isActive = 0;

    //    digitalWrite(ledButton, HIGH);

  }
  else if (vehicleStatus == 3) {               //////////////blue_stateC
    isActive = 0;
    config.isInSleep = 0;

  }
  else if (vehicleStatus == 7) {                           //////////////////red_stateError

    config.isInSleep = 0;
    isActive = 0;

  }
}
