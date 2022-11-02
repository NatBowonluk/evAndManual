
#define SHORT_PRESS_TIME 40000 // 500 milliseconds
#define buzzer 13

#if boardColour == 2
#define rfidReset 23
#define sdaPin 21
#define sclPin 22
#define i2cfreq 400000
#define resetPower 12
#define chargeButtonPin 2
//#define RFID_Reset 15
#define rgbR               32
#define rgbG               33
#define rgbB               25
#else
#define rgbR               33
#define rgbG               25
#define rgbB               26
#define connectStatus            27  //Connect in PCB =>Active Low
#define chargeStatus             14  //Charge in PCB  =>Active Low
#define errorStatus              12  //Error in PCB   =>Active Low
#define chargeLedPin 2 //2 in ev03 it become reset power only
#define chargeButtonPin 35
#define resetPower 32 // active high
#endif
//#define resetRfids 15

//#define ledButton           2  //DO2 in PCB


#define pwmFrequency     5000
#define pwmResolution       8   //Bit resolution 2^8 = 256
#define redChannel           0
#define greenChannel         1
#define blueChannel          2

#define stringsize 64
#define configsize 1024
#define filterrfidsize 200
#define filterpower 70
#define mainrfidaccsize 300
#define mainrfidpower 150
#define timesize 200
#define rfidtotaljsonsize 32
#define currentlyAccidentsize 48

#define buzzerFrequency 2750
#define buzzerChannel  3

#define stealthtime 5
//#define historydatimesize 12
//#define historynamesize 44
//#define historyhoursize 10
///////////////////////////////////////////////////////////some temporary buffer
//String ntpTimeTemp;
//String dateTempNTP;
//String timeTempNTP;
//const char* ntpServer = "pool.ntp.org";
//const long  gmtOffset_sec = 0;
//const int   daylightOffset_sec = 25200;
String serialID;
String subscribeTopic;
String publishTopic;

uint8_t durationMMTemp;

//uint16_t timmingNTPTemp[6];
//bool connectInternet = false;
String nowUid;
uint8_t evlastStat;
String delUid;
uint32_t unixBuff;
String errorTemptext = "";
const char* otaUser = "bluestone";
const char* otaPass = "bluestone";
//uint8_t monthBuffNum = 0;
uint8_t disconnectWeb = 0;
uint8_t lastState = LOW;  // the previous state from the input pin
uint8_t currentState;     // the current reading from the input pin
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
uint8_t ledStateButton = LOW;
uint8_t wifiDisconnectCount = 0;
//////////////////////////////////////////////startup flag
bool hasConfig = true;
bool hasScheduleSet = true;
bool hasloadAccidentOffSet = true;
bool hasloadHistorySet = true;
bool hasloadTotalRFIDSet = true;

float evTempShared;

uint8_t countforsettime = 0;
// bool isScheduleSet = false;
bool isRFIDAdd = false;
///////////////////////////////////////////////////////system config
const char *jsonconfig = "/config.json";
struct Config
{
  char username[stringsize];
  char userpass[stringsize];
  char ssid[stringsize];
  char pass[stringsize];
  char ssidap[stringsize];
  char passap[stringsize];
  char ip[stringsize];
  char gateway[stringsize];
  char domainName[stringsize];
  uint8_t rfidusing;
  uint8_t currentmax;
  uint8_t phaseset;
  uint8_t phasesetold;
  uint16_t port;

  uint32_t sleepSec;

  uint8_t firstSleepThisDay;
  uint8_t isInSleep; //0 no 1 yes
  uint8_t dayThatDidntReset; //if it 7, it gonna reset when device is on first sleep
  uint8_t maxDayThatDidntReset;
  uint8_t resetHourRangeStart;
  uint8_t resetHourRangeEnd;
  uint16_t criticalHeapRangeStart;
  uint16_t criticalHeapRangeEnd;
  uint8_t doneReset;//0 wait 1 done

  uint8_t currentDate;
//#if ISMQTT == 1
  char mqttcilentid[stringsize];
  char mqttuser[stringsize];
  char mqttpass[stringsize];
  char mqttip[stringsize];
  uint16_t mqttport;
//#endif
};
Config config;

/////////////////////////////////////////////////////setting time schedule
const char *jsonschedule = "/schedule.json";
struct scheduletimefromweb
{
  //  char schedule[stringsize];
  String schfulltext;
  bool schalw;
  bool schflg;
  //  uint16_t schyy;
  //  uint8_t schmt;
  //  uint8_t schdy;
  uint8_t schhr;
  uint8_t schmm;
  uint8_t schss;

  bool clearSchedule;
};
scheduletimefromweb schtime;

/////////////////////////////////////////////////////////////rfid stuff
const char *jsonaccRFID = "/accountRFID.json"; //account
const char *jsonRFIDtotal = "/totalRFID.json"; //max account
struct RfidInfo
{
  //  char rfidfname[stringsize];
  //  char rfidlname[stringsize];
  String rfidfname;
  String rfidlname;
  uint8_t rfidtotal;
  uint8_t rfidremoveid;
};
RfidInfo rfidfig;

////////////////////////////////////////////////when device accidently turn off
const char *jsonAccident = "/accidentOff.json";
struct forAccidentOff
{
  //  bool isStillCharge; // if charge on coil
  uint8_t currentlyAmp; // current that set by user
  uint8_t carLastStat; // for check charging logic
  uint8_t carNowStat; // for check charging logic
  uint8_t webChargeStatus; //0 = stanby, 1 = chargenow button, 2 = stanby charginig yellow 3 = charging, 4 = Finish, 6 = schedule, 7 = error
  bool isPressedC; // charge now button in website
  bool isChargeSettingDone; // setting false not set true set
  uint8_t isStartOrFinish; // 0 standby,1 setting charge and schedule, 2 start mode, 3 operate, 4 finish mode
  bool scheduleStart; // setting false not set true se
  uint16_t startYR; // for billing
  uint8_t startMT; // for billing
  uint8_t startDD; // for billing
  uint16_t startHH; // for billing
  uint8_t startMN; // for billing
  uint8_t startSS; // for billing
  uint16_t startChargeHR; // for billing
  uint8_t startChargeMN; // for billing
  uint8_t startChargeSS; // for billing
  String RfidUseFname;
  String RfidUseLname;
  String RfidUseUid;
  String ChargeStartEnergy;
  //  uint32_t unixStart;
  //  uint32_t unixDone;
  String FirstGuyConnectedMAC;
};
forAccidentOff currentValue;

/////////////////////////////////////////////////////////////for charging history
const char *jsonHistory = "/js/logger.json";
const char *jsonTotalHistory = "/totalHistory.json";
struct charginghistory {
  String dateCharge;
  String timeCharge;
  String nameCharge;
  String hoursCharge;
  float energyCharge;
  uint16_t totalChatge;
};
charginghistory history;

//////////////////////////////////////////////////////////for meter

struct electricmeter
{
  float voltage1;
  float current1;
  float power1;
  float voltage3[3];
  float current3[3];
  float power3[3];
  float energy;
};
electricmeter electric;


//const char* http_username = "admin";
//const char* http_password = "admin";

//////////////////////////////////////////////////////////////// now charge datetime
struct chargeTime {
  uint8_t dateStart;
  uint8_t monthStart;
  uint16_t yearStart;
  uint8_t hoursStart;
  uint8_t minuiteStart;
  uint8_t secoundStart;
};
chargeTime chargeTimeWeb;

struct nowtime {
  uint8_t dateNow;
  uint8_t monthNow;
  uint16_t yearNow;
  uint8_t hoursNow;
  uint8_t minuiteNow;
  uint8_t secoundNow;
};
nowtime timetime;
