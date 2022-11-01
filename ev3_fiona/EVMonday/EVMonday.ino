/*
  เขียน Temp ตัวนึง แล้ว เก็บ แก้
*/

/*
   SETUP PRE-COMPLIER
   WA : GREEN >> RTCIC 0, boardColour 0, EVCONTROL 0
   WA : BLUE >> RTCIC 1, boardColour 1, EVCONTROL 0
   PHOENIX : GREEN >> RTCIC 0, boardColour 0, EVCONTROL 1
   PHOENIX : BLUE >> RTCIC 1, boardColour 1, EVCONTROL 1
*/

//SETUP PRE-COMPILER HERE
#define ISVERYNEWBOARD 1
#define ISMQTT 0 //0 = web, 1 = mqtt
#define RFIDSALE 1//0 = no rfid, 1= rfid
#define DEBUG 1//0 = NO-DEBUG, 1 = DEBUG
#define RTCIC 1 //0 = DS1307, 1 = DS3231, if EV03 always use 1
#define boardColour 2 //0 = EVC01 REV1, 1 = EVC01 REV2, EVC01 REV3, 2 = NEW VER (WA)
#define EVCONTROL 0// 0 = WA, 1 = PHOENIX
#define DEMO 1 // 0 = product, 1 = demo
#define COMPANYNAME "BLUESTONE"
#define DEVICECODE "EVCS"
#define MAINCONTROLLER "ICES"
#define RELIGION "TH"
#define RELEASEDATE "AH"
#define REV 16
#define TIMEFORNONWEB 4
#define MAXRFIDTOTALLOG 11 //mean 10
#define MAXHISTORYLOG 50 // mean 50
#define TEMPPHOENIXOFFSET 13

#if DEMO == 1
#define MAXCONNECTION 2
#else
#define MAXCONNECTION 1
#endif

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#if RTCIC == 1
#define rtcICnum RTC_DS3231
#else
#define rtcICnum RTC_DS1307
#endif

#include <ArduinoJson.h>
#include <SPIFFS.h>

//////////////////////////////////////////web library
#include <ESPmDNS.h>
#include <WiFi.h>

#if ISMQTT == 1
#include <PubSubClient.h>
#endif
//#include <ESP32Ping.h>
#include <BarryKalmanFilter.h>
//BarryKalmanFilter barryKalmanFilter(2, 2, 0.01);
BarryKalmanFilter voltageKalmanFilter(2, 2, 0.01);
BarryKalmanFilter current1KalmanFilter(2, 2, 0.01);
BarryKalmanFilter current2KalmanFilter(2, 2, 0.01);
BarryKalmanFilter current3KalmanFilter(2, 2, 0.01);
BarryKalmanFilter powerKalmanFilter(2, 2, 0.01);
BarryKalmanFilter energyKalmanFilter(2, 2, 0.01);
////////////////////////////////////////NTP
//#include <NTPClient.h>
//#include <WiFiUdp.h>
//
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include <ESPAsyncWebServer.h>
//#include <WebSocketsServer.h>
uint16_t port = 80;
AsyncWebServer *server;
//AsyncWebServer server(80); // server port 80
//WebSocketsServer websockets(81);

////////////////////////////////////////////OTA`
#include <Update.h>

/////////////////////////////////////////RFID Library
#include "Wire.h"
#if RFIDSALE == 1
#include <RFID.h>
RFID rfid;
#else
#endif

///////////////////////////////////////iox
#if boardColour == 2
#include <IOExpander.h>
IOEXPANDER iox;
#else
#endif
///////////////////////////////////////EV Library
#if EVCONTROL == 1
#include <EVController.h>
EVCONTROLLER ev;
#include <METER.h>
METER meter;
#else
#include <WAEVController.h>
WAEVCONTROLLER ev;
#endif


///////////////////////////////////////RTC Library
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "RTClib.h"
//#include "time.h"
rtcICnum rtc;

AlarmId id;
AlarmId id1;
//AlarmId id2;
//AlarmId id3;
//AlarmId id4;
//AlarmId id5;
//AlarmId id6;
//AlarmId id7;
//AlarmId id8;
//AlarmId id9;
//AlarmId id10;
//AlarmId id11;
//AlarmId id12;
//AlarmId id13;
//AlarmId id14;
//AlarmId id15;
//AlarmId id16;
//AlarmId id17;
//AlarmId id18;
//AlarmId id19;
//AlarmId id20;
//AlarmId id21;

#if ISMQTT == 1
#include "imgbanner.h"
#include "imgcancel.h"
#include "imgcheck.h"
#include "imglogo.h"
#include "imgrfidscan.h"
#include "imgrfidwait.h"
#include "webmqttcss.h"
#include "webmqttindex.h"
#include "webmqttjs.h"
#else
#include "webjquery.h"
#include "webcss.h"
#include "webindex.h"
#include "webstatusone.h"
#include "webstatusthree.h"
#include "websetting.h"
#include "webaddrfid.h"
#include "webhistory.h"
#include "webota.h"
#endif

#include "utility.h"

#include "logicflag.h"
#include "setvariable.h"
#include "jsonsaveandload.h"
#include "mainsetup.h"
#if ISVERYNEWBOARD == 1
#include "lednewboard.h"
#else
#include "ledfunction.h"
#endif
#include "rtcfunction.h"
#include "meterfunction.h"
#include "charginglog.h"
#include "evfunction.h"
//#include "rfidplaying.h"
#include "rfidfunction.h"
#include "rfidusingfunction.h"
#include "factoryreset.h"
#include "buttonfunction.h"

//#include "webprocessfunction.h"
//#include "websocketfunction.h"

#if ISMQTT == 1
WiFiClient client;
PubSubClient mqtt(client);
#include "evmqttprocessing.hpp"
#endif
#include "wifievent.h"

#if ISMQTT == 1

#include "evmqttconfigweb.h"

#else
#include "wifiandwebsetup.h"
#endif

#include "initeverthing.h"

#include "debugfunction.h"

#include "simulateinput.h"

const unsigned long eventIntervalExecte = 1000;
unsigned long previousTimeExecte = 0;

void TaskEV( void *pvParameters );
void TaskOther( void *pvParameters );

void setup() {

  initserial();

  initChargeButton();
  delay(100);
  rtcinit();

  initSPIFFS();
  initstartupvalue();

  swDelay(200);

#if ISMQTT == 1
  //  createSerialNumber(serialID);
  createTopic("evstation", String(config.mqttcilentid), subscribeTopic);
  createTopic("evstation", "response", publishTopic);
  Serial.print(F("Serial Number : ")); Serial.println(serialID);
  Serial.print(F("Subscribe Topic : ")); Serial.println(subscribeTopic);
  Serial.print(F("Publish Topic : ")); Serial.println(publishTopic);
#endif

  initsensor();
  swDelay(200);
#if boardColour == 2
  //  Serial2.begin(9600, SERIAL_8N1, RXD1, TXD1);
  swDelay(15);
  if (iox.init(sdaPin, sclPin, i2cfreq))
  {
    Serial.println(F("IOExpander initialization success"));
  }
  swDelay(15);
  iox.pinMode(buzzer, OUTPUT);
  iox.pinMode(resetPower, OUTPUT);
#elif boardColour == 1
  digitalWrite(resetPower, LOW);
  digitalWrite(buzzer, LOW);
#elif boardColour == 0
  digitalWrite(resetPower, HIGH); // in green board, it is a enable LED
  ledcWrite(buzzerChannel, 0);
#endif
  debug("port = "); debugln(config.port);
  swDelay(1000);
  swDelay(15);
  readmeter(config.phaseset);
  swDelay(100);

  if (config.isInSleep == 1) {
    isActive = config.sleepSec + 1;
    execSleepWake = true;
  }

  rfidAutenChargeNowButton = 0;
  execDisconnect = true;

  evSetMode(3);
  delay(100);
  Serial.println(F("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
  //  timeClient.begin();
  //  timeClient.setTimeOffset(25200);
  //  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  delay(100);

  //  Serial.println("test scan");
  //  rfidreadread();
  xTaskCreatePinnedToCore(
    TaskEV
    ,  "TaskEV"   // A name just for humans
    ,  14000  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);

  xTaskCreatePinnedToCore(
    TaskOther
    ,  "TaskOther"
    ,  12000  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL
    ,  1);
}

void loop() {

}
String cid = "lordbarry";
String usermq = "barry";
String passmq = "pinkywinky";
void TaskOther(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  swDelay(15);
  readmeter(config.phaseset);
  debug("electric.energy : "); debugln(electric.energy);
  swDelay(15);
  initWifiApServer();

  swDelay(1000);
  //  rfidreadread();

  debug("Current Old : "); debugln(evUserGetCurrent());
  evAccidentReset(evlastStat, currentValue.currentlyAmp);
  debug("Current Now : "); debugln(evUserGetCurrent());

  initDone = true;


  for (;;) // A Task shall never return or exit.
  {
#if ISMQTT == 1
    //    mqttProcess("akuakshdiasbdaudh", String(config.mqttuser), String(config.mqttpass));
    //    mqttProcess("lordbarry", "barry", "pinkywinky");
    if (mqtt.connected() == false) {
      Serial.print(F("MQTT connection..."));
      //      if (mqtt.connect(config.mqttcilentid, config.mqttuser, config.mqttpass)) {
      //      if (mqtt.connect("lordbarry", "barry", "pinkywinky")) {
      if (mqtt.connect(cid.c_str(), usermq.c_str(), passmq.c_str())) {
        Serial.println(F("connected"));
        mqtt.subscribe(subscribeTopic.c_str());
      } else {
        Serial.println(F("failed"));
        swDelay(1000);
      }
    } else {
      mqtt.loop();
    }
#endif
    //    printf("TaskOther MEMORY WATERMARK = %d\n", uxTaskGetStackHighWaterMark(NULL));
    if (!execRfidv2) {
      //      if(!isReadMeterDone){
      //         readmeter(config.phaseset);
      //      }

      if (execReconnectWifi) {
        reconnectWifiSta();
      }
      if (execButtonFunc) {
        pressedOrReleased();
      }
      else if (!execButtonFunc) {
        pressedTime = 0;
        releasedTime = 0;
      }

      //      simulate();
      //      swDelay(15);

      if (restartNow)
      {
        //        if (!SPIFFS.begin(true)) {
        //          debugln("An Error has occurred while mounting SPIFFS");
        //          return;
        //        }
        //        bool formatted = SPIFFS.format();
        //        if (formatted) {
        //          debugln("\n\nSuccess formatting");
        //        } else {
        //          debugln("\n\nError formatting");
        //        }
        //
        //        debugln("Restart");
        //        delay(1000);
#if boardColour == 2
        debugln(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Update complete");
        iox.digitalWrite(resetPower, HIGH);
        delay(5000);

        ESP.restart();
#elif boardColour == 1
        debugln(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Update complete");
        digitalWrite(resetPower, HIGH);
        delay(5000);

        ESP.restart();
#else
        debugln(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Update complete");
        ESP.restart();
#endif

      }
      //      if (displayPage == 1 && rfidAutenChargeNowButton == 1) {
      //        //        yield();
      //        readmeter(config.phaseset);
      //      }
    }

  }

}

bool isSharedI2C = false;

void TaskEV(void *pvParameters)  // This is a task.
{

  sharedModbusUse = false;

  (void) pvParameters;

  while (!initDone) {
    swDelay(1000);
  }
  swDelay(200);

  for (;;) // A Task shall never return or exit.
  {
    while (sharedModbusUse) {
      swDelay(100);
    }
    if (isScanning) {
      TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
      TIMERG0.wdt_feed = 1;
      TIMERG0.wdt_wprotect = 0;
      Alarm.delay(2);
      swDelay(100);
    }
    if (!isScanning) {

      unsigned long currentTimeExecte = millis();
      //      if (currentTimeExecte - previousTimeExecte >= eventIntervalExecte) {
      if ((itIsTime(currentTimeExecte, previousTimeExecte, eventIntervalExecte)) && !inScanFunc) {
        //          Serial.println(F("Heap 1 : ")); Serial.print(ESP.getFreeHeap());
        //        printf("Heap 0 : %d \n", ESP.getFreeHeap());


        if (evlastStat == 0 || evlastStat == 1) {
          isActive++;
        }

        //        Serial.print("time sleep count : "); Serial.println(isActive);
#if boardColour == 2
        isInWebSite++;
#endif
        if (!execRfidAuthenChargeButton || !execRfidAdd) {
          swDelay(15);
//          if (!isSharedI2C) {
//            isSharedI2C = true;
if(!inRFID){
     DateTime now = rtc.now(); // Get current time
            timetime.secoundNow = now.second();
            timetime.minuiteNow = now.minute();
            timetime.hoursNow = now.hour();
            timetime.dateNow = now.day();
            timetime.monthNow = now.month();
            timetime.yearNow = now.year();
            unixBuff = now.unixtime();
//            isSharedI2C = false;
//          }

          //          }
          swDelay(15);
}
         
          readmeter(config.phaseset); //////////////new new new new
        }

        if (generalDebugFlag) {
          generalDebug();
        }
        //        printf("TaskEV MEMORY WATERMARK = %d\n", uxTaskGetStackHighWaterMark(NULL));
        //        debug("execRfidAuthenChargeButton : "); debugln(execRfidAuthenChargeButton);
        //        debug("rfidAutenChargeNowButton : "); debugln(rfidAutenChargeNowButton);
        //        debug("rfidReadDone : "); debugln(rfidReadDone);
        if (execRfidAuthenChargeButton) {
          swDelay(15);
#if RFIDSALE == 1
          rfidAuthenChargeNowFunc();
#else
#endif
        }
        if (execLedButtonBlink) {
#if boardColour == 2
#else
          LedButtonBlink();
#endif
        }

        if (execDisconnect) {
          //          serverDisconnect();
          //          readmeter(config.phaseset);
        }

        if (execMeter || execEvSend) {
          //          simulateMeter(config.phaseset);
          //          readmeter(config.phaseset);
          //          debug("electric.energy : "); debugln(electric.energy);
          //          debug("start.Energy : "); debugln(currentValue.ChargeStartEnergy );

          //          evStatus();
        }
        if (execSettingSend) {
          //          settingShow();
          execSettingSend = false;
        }
        ///////////////////////////////////////////////////////////////////////////////////
#if RFIDSALE == 1
        if (execRfidAdd) {
          if (!rfidReadDone) {
            rfidStatus = 0;
            //          startReadTag = true;
//            if (!isSharedI2C) {
//              isSharedI2C = true;
//              Serial.println("55555555555555555555555555555 start RFID");
              rfidreadread();
//              isSharedI2C = false;
//            }


            if (nowUid != "timeout") {
              if (checkRFID(nowUid, jsonaccRFID)) {
                debugln("check pass");
                Serial.print(F("//////////////////////////// Total RFID AFTER CHECK PASS : ")); Serial.print(rfidfig.rfidtotal); Serial.println(F(" ////////////////////////////"));
                if (rfidfig.rfidtotal >= MAXRFIDTOTALLOG) {
                  rfidStatus = 2;
                  rfidReadDone = false;
                }
                else {
                  rfidStatus = 1;
                }
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
                //            rfidSend();
              }
              else if (!checkRFID(nowUid, jsonaccRFID)) {
                debugln("already have this card");
                rfidStatus = 2;
                rfidReadDone = false;
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
                //            rfidSend();
              }
            }
          }
          //          rfidSend();
          //          debug("rfidReadDone : "); debugln(rfidReadDone);
          //          debug("rfidGetInputName : "); debugln(rfidGetInputName);
          //          debug("isRfidDelete : "); debugln(isRfidDelete);
          //          debug("execRfidAdd : "); debugln(execRfidAdd);
        }
#else
#endif
        //        debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////");
        //
        //        debug(timetime.dateNow);  debug('/');  debug(timetime.monthNow);  debug('/');  debugln(timetime.yearNow);
        //        debug(timetime.hoursNow);  debug(':');  debug(timetime.minuiteNow);  debug(':');  debugln(timetime.secoundNow);
        //        debug(hour()); debug(':'); debug(minute()); debug(':'); debugln(second());
        //        //      debug("MAC : "); debugln(OneConnectedApMac());
        //        debug("First : "); debugln(timetime.yearNow);
        //        debug("Local IP: "); debugln(WiFi.localIP());
        //        //          debug("config.firstSleepThisDay : "); debugln(config.firstSleepThisDay);
        //        debugln(isInWebSite);
        //
        //        debugln("////////////////////////////////////////////////////////////////////////////////////////////////////////");
        previousTimeExecte = currentTimeExecte;
      }
      if (!inScanFunc) {
        evRunning();
        Alarm.delay(2);
        swDelay(50);
      }
      else if (inScanFunc) {
        //        evRunning();
        Alarm.delay(2);
        swDelay(100);
      }
      //      Alarm.delay(2);
    }

  }
}
