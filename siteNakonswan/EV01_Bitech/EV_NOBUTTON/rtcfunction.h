bool rtcinit(void);
void adjustRTC(uint16_t yy, uint8_t mt, uint8_t dd, uint8_t hh, uint8_t mm, uint8_t ss);
void adjustDeviceTime(uint16_t yy, uint8_t mt, uint8_t dd, uint8_t hh, uint8_t mm, uint8_t ss);
void scheduleStart(void);
void setSchedule(uint8_t hh, uint8_t mm);
void cancelSchedule(void);
uint8_t monthNumber(String monthBuff);

bool rtcinit()
{
  if (!rtc.begin())
  {
    debugln("Couldn't find RTC");
    Serial.flush();
    //    abort();
#if boardColour == 2
    delay(10000);
    iox.digitalWrite(resetPower, HIGH);
#elif boardColour == 1
    digitalWrite(resetPower, HIGH);
    delay(5000);
#else

#endif
    return false;
  }
  
  debugln("Initial RTC success");
  
  DateTime now = rtc.now(); // Get current time
  uint8_t ss;
  uint8_t mm;
  uint16_t hh;
  uint8_t dd;
  uint8_t mt;
  uint16_t yy;

  ss = now.second();
  mm = now.minute();
  hh = now.hour();
  dd = now.day();
  mt = now.month();
  yy = now.year();
  unixBuff = now.unixtime();

  debug(dd);  debug('/');  debug(mt);  debug('/');  debugln(yy);
  debug(hh);  debug(':');  debug(mm);  debug(':');  debugln(ss);

  setTime(hh, mm, ss, dd, mt, yy);

  return true;
}

void adjustRTC(uint16_t yy, uint8_t mt, uint8_t dd, uint8_t hh, uint8_t mm, uint8_t ss) {
  rtc.adjust(DateTime(yy, mt, dd, hh, mm, ss));
}

void adjustDeviceTime(uint16_t yy, uint8_t mt, uint8_t dd, uint8_t hh, uint8_t mm, uint8_t ss) {
  setTime(hh, mm, ss, dd, mt, yy);
}

void scheduleStart() {
  debugln("///////////////////////////////////////////////////////////////////start schedule");
  Serial.println(F("///////////////////////////////////////////////////////////////////start schedule"));
  currentValue.isPressedC = true;
  if (evlastStat == 0 || evlastStat == 1) {
    if (!schtime.schalw) {
      swDelay(15);
      cancelSchedule();
      currentValue.isChargeSettingDone = false;
      currentValue.isPressedC = false;
      schtime.schflg = false;
      currentValue.webChargeStatus = 0;
      Serial.println(F("///////////////////////////////////////////////////////////////////start schedule : failed : no daily"));
      debugln("///////////////////////////////////////////////////////////////////start schedule : failed : no daily");
    }
    else if (schtime.schalw) {
      //      cancelSchedule();
      swDelay(15);
      currentValue.isPressedC = false;
      currentValue.webChargeStatus = 6;
      //      currentValue.isChargeSettingDone = true
      swDelay(15);
      setSchedule(schtime.schhr, schtime.schmm);
      Serial.println(F("///////////////////////////////////////////////////////////////////start schedule : failed : do daily"));
      debugln("///////////////////////////////////////////////////////////////////start schedule : failed : do daily");
    }
  }
  if (config.rfidusing == 1) {
    if (evlastStat == 2 &&  rfidAutenChargeNowButton == 0) {
      if (!schtime.schalw) {
        swDelay(15);
        cancelSchedule();
        currentValue.isChargeSettingDone = false;
        currentValue.isPressedC = false;
        schtime.schflg = false;
        currentValue.webChargeStatus = 1;
        Serial.println(F("///////////////////////////////////////////////////////////////////start schedule : failed : no daily"));
        debugln("///////////////////////////////////////////////////////////////////start schedule : failed : no daily");
      }
      else if (schtime.schalw) {
        //      cancelSchedule();
        swDelay(15);
        currentValue.isPressedC = false;
        currentValue.webChargeStatus = 6;
        //      currentValue.isChargeSettingDone = true
        swDelay(15);
        setSchedule(schtime.schhr, schtime.schmm);
        Serial.println(F("///////////////////////////////////////////////////////////////////start schedule : failed : do daily"));
        debugln("///////////////////////////////////////////////////////////////////start schedule : failed : do daily");
      }
    }
  }
}
void setSchedule(uint8_t hh, uint8_t mm) {
  Alarm.free(id);
  id = dtINVALID_ALARM_ID;
  swDelay(15);
  Alarm.free(id1);
  id1 = dtINVALID_ALARM_ID;
  swDelay(15);
  //  Alarm.free(id2);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id3);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id4);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id5);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id6);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id7);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id8);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id9);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id10);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id11);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id12);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id13);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id14);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id15);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id16);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id17);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id18);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id19);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id20);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id21);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);

  id =  Alarm.alarmRepeat(hh, mm, 0, scheduleStart); swDelay(15);
  id1 =  Alarm.alarmRepeat(hh, mm, 1, scheduleStart); swDelay(15);
  //  id2 =  Alarm.alarmRepeat(hh, mm, 2, scheduleStart); swDelay(15); //126
  //  id3 =  Alarm.alarmRepeat(hh, mm, 3, scheduleStart); swDelay(15);
  //  id4 =  Alarm.alarmRepeat(hh, mm, 4, scheduleStart); swDelay(15);
  //  id5 =  Alarm.alarmRepeat(hh, mm, 5, scheduleStart); swDelay(15);
  //  id6 =  Alarm.alarmRepeat(hh, mm, 6, scheduleStart); swDelay(15);
  //  id7 =  Alarm.alarmRepeat(hh, mm, 7, scheduleStart); swDelay(15);
  //  id8 =  Alarm.alarmRepeat(hh, mm, 8, scheduleStart); swDelay(15); //126
  //  id9 =  Alarm.alarmRepeat(hh, mm, 9, scheduleStart); swDelay(15);
  //  id10 =  Alarm.alarmRepeat(hh, mm, 10, scheduleStart); swDelay(15);
  //  id11 =  Alarm.alarmRepeat(hh, mm, 11, scheduleStart); swDelay(15);
  //  id12 =  Alarm.alarmRepeat(hh, mm, 12, scheduleStart); swDelay(15); //126
  //  id13 =  Alarm.alarmRepeat(hh, mm, 13, scheduleStart); swDelay(15);
  //  id14 =  Alarm.alarmRepeat(hh, mm, 14, scheduleStart); swDelay(15);
  //  id15 =  Alarm.alarmRepeat(hh, mm, 15, scheduleStart); swDelay(15);
  //  id16 =  Alarm.alarmRepeat(hh, mm, 16, scheduleStart); swDelay(15);
  //  id17 =  Alarm.alarmRepeat(hh, mm, 17, scheduleStart); swDelay(15);
  //  id18 =  Alarm.alarmRepeat(hh, mm, 18, scheduleStart); swDelay(15); //126
  //  id19 =  Alarm.alarmRepeat(hh, mm, 19, scheduleStart); swDelay(15);
  //  id20 =  Alarm.alarmRepeat(hh, mm, 20, scheduleStart); swDelay(15);
  //  id21 =  Alarm.alarmRepeat(hh, mm, 21, scheduleStart); swDelay(15);
  currentValue.scheduleStart = true;
  debugln("///////////////////////////////////////////////////////////////////set schedule");
  Serial.println(F("///////////////////////////////////////////////////////////////////set schedule"));
}

//void setScheduleOnce(uint8_t hh, uint8_t mm) {
//  Alarm.free(id);
//  id = dtINVALID_ALARM_ID;
//  id =  Alarm.alarmOnce(hh, mm, 0, scheduleStart);
//}

void cancelSchedule() {
  Alarm.free(id);
  id = dtINVALID_ALARM_ID;
  swDelay(15);
  Alarm.free(id1);
  id1 = dtINVALID_ALARM_ID;
  swDelay(15);
  //  Alarm.free(id2);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id3);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id4);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id5);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id6);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id7);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id8);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id9);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id10);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id11);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id12);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id13);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id14);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id15);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id16);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id17);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id18);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id19);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id20);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  //  Alarm.free(id21);
  //  id = dtINVALID_ALARM_ID;
  //  swDelay(15);
  currentValue.scheduleStart = false;
}
uint8_t monthNumber(String monthBuff) {
  uint8_t monthBuffNum;
  if (monthBuff == "January") {
    monthBuffNum = 1;
  }
  else if (monthBuff == "February") {
    monthBuffNum = 2;
  }
  else if (monthBuff == "March") {
    monthBuffNum = 3;
  }
  else if (monthBuff == "April") {
    monthBuffNum = 4;
  }
  else if (monthBuff == "May") {
    monthBuffNum = 5;
  }
  else if (monthBuff == "June") {
    monthBuffNum = 6;
  }
  else if (monthBuff == "July") {
    monthBuffNum = 7;
  }
  else if (monthBuff == "August") {
    monthBuffNum = 8;
  }
  else if (monthBuff == "September") {
    monthBuffNum = 9;
  }
  else if (monthBuff == "October") {
    monthBuffNum = 10;
  }
  else if (monthBuff == "November") {
    monthBuffNum = 11;
  }
  else if (monthBuff == "December") {
    monthBuffNum = 12;
  }
  return monthBuffNum;
}
