void initChargeButton(void);
void pressedOrReleased(void);
#if boardColour == 2
#else
void LedButtonBlink(void);
#endif
void initChargeButton() {
#if boardColour == 2
//  pinMode(chargeButtonPin, INPUT_PULLUP);
#else
  pinMode(buzzer, OUTPUT);
  pinMode(resetPower, OUTPUT);
  pinMode(chargeLedPin, OUTPUT);
  pinMode(chargeButtonPin, INPUT);
#endif
}

unsigned long previousMillisBT = 0;
const long intervalBT = 1000;
//unsigned long inpressedOrReleasedMillis = 0;
//unsigned long inpressedOrReleasedCount = 0;

void pressedOrReleased() {
  currentState = digitalRead(chargeButtonPin);
  //  inpressedOrReleasedMillis

  if (lastState == HIGH && currentState == LOW) {      // button is pressed
    pressedTime = millis();
    //    pressedTime = inpressedOrReleasedCount;
  }
  else if (lastState == LOW && currentState == HIGH) { // button is released
    releasedTime = millis();
    //    releasedTime = inpressedOrReleasedCount;

    long pressDuration = releasedTime - pressedTime;
    debugln(releasedTime);
    debugln(pressedTime);
    debugln(pressDuration);

    if (!buttonFirsttime) {
      buttonFirsttime = true;
    }
    else if (buttonFirsttime) {
      if ( pressDuration < SHORT_PRESS_TIME ) {
        if (execSleepWake) {
          isActive = 0;
          config.isInSleep = 0;
          saveConfiguration(jsonconfig, config);
          debugln("A short press is detected : sleep awake");
        }
        else if (!execSleepWake) {
          debugln("A short press is detected : sleep awake : not doing");
        }
        if (!execHardButton) {
          debugln("A short press is detected : for charge now : not in avaiable stage");
        }
        else if (execHardButton) {
#if boardColour == 2
#else
          digitalWrite(chargeLedPin, LOW);
          currentValue.isPressedC = true;
          currentValue.isChargeSettingDone = true;
#endif
          debugln("A short press is detected : for charge now : Press Button and Setting >> Done");
        }
      }
      else if ( pressDuration >= SHORT_PRESS_TIME ) {
        if (!execFactoryReset) {
          debugln("A long press is detected : for factory reset : not avaiable");
        }
        else if (execFactoryReset) {
          debugln("A long press is detected : for factory reset");
          execLedButtonBlink = false;
          factoryReset();
        }
      }
    }
  }
  // save the the last state
  //  inpressedOrReleasedCount++;
  lastState = currentState;
}
#if boardColour == 2
#else
void LedButtonBlink() {
  unsigned long currentMillisBT = millis();
  //  if (currentMillisBT - previousMillisBT >= intervalBT) {
  if (itIsTime(currentMillisBT, previousMillisBT, intervalBT)) {

    previousMillisBT = currentMillisBT;
    if (ledStateButton == LOW) {
      ledStateButton = HIGH;
    } else {
      ledStateButton = LOW;
    }
    digitalWrite(chargeLedPin, ledStateButton);
  }
}
#endif
