#define FinishSec 4

bool isRFIDRead = false;

uint32_t dayTemp;

//bool pingSuccess = false;

bool sharedModbusUse = false;

bool timeWAnotGet = false;

bool sharedChargeTime = false;

//config.rfidusing : 0 = off, 1 = on
bool startFunction = false; //true = program run, false = dont run
bool isDisplayChargePage = false; // flag inside server.on chargepage in page = true, else page = false

//bool isPressedC = false; // charge now button in website
//bool isChargeSettingDone = false;
//uint8_t isStartOrFinish = 3; // 0 standby,1 setting charge, 2 start mode, 3 operate, 4 finish mode

uint8_t displayPage;

String TimeDuration = "--:--:--";

uint8_t hrbuff = 0;
uint8_t mnbuff = 0;
uint8_t ssbuff = 0;

uint8_t errorMakeSureCount = 0;

bool initDone = false;
////////////////////////////////////guard
bool execMeter = false;
bool execEvSend = false;
bool execEvSetCurrent = false;
bool execRfidAdd = false;
bool execSettingSend = false;
uint8_t execSettingWait = 0; //0 = not setting page, 1 = setting page load done, 2 = paste value
bool execSetTime = false;
uint8_t execSetTimeWait = 0; //0 = not index page, 1 = index page load done, 2 = paste value
bool execRfidAuthen = false;
bool execDisconnect = false;
uint8_t execOnceSettime = 0; //0 failed, 1 get wrong value, 2 pass
bool execButtonFunc = false;
bool execHardButton = false;
//bool execFactoryReset = false;
bool execLedButtonBlink = false;
bool execRfidAuthenChargeButton = false;
bool execPageLoad = false;
bool execFactoryReset = false;
bool execReconnectWifi = false;

bool buttonFirsttime = false;
bool connectOTA = false;
bool restartNow = false;

bool rfidReadDone = false;
bool rfidGetInputName = false;

uint8_t rfidStatus = 0; //0 = scan, 1 = done, 2 = failed, 3 = don delete reload page
bool isRfidDelete = false;

bool clearSchedule = false;

uint8_t simulateCarValue;
bool simulateCar = false;

uint8_t chargingDoneCount; //10sec

uint8_t rfidAuthenStatus = 0; //0 false, 1 still read, 2 pass

uint8_t rfidAutenChargeNowButton = 0; //0 false, 1 pass, 2 scan

bool isOpenWeb = false;

uint8_t countRfid = 0;

bool isAuthen = false;

bool startReadTag = false;

bool execRfidv2 = false;

bool historyStart = false;

bool isCancel = false;

bool generalDebugFlag = false;

uint8_t rfidIndexStage = 0; //0 nothing, 1 scan, 2 done complete, 3 done failed

bool isReadMeterDone = false;

bool isScanning = false;

bool useActive = false;
uint32_t isActive = 0; // 0 = active , 1++ time using max 900 sec (15 min)

bool inScanFunc = false;

uint8_t bufferDate;

bool scantoBegin = false;

bool execSleepWake = false;

uint8_t isInWebSite = 0; // more than 15 is mean not using web

bool inRFID = false;

String vehicleStatusBufferForDebug = "";

bool fisrtStealthSetTime = false;

bool startAOffCoil = false;
