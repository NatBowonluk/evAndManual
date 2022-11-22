#include <WAEVController.h>

WAEVCONTROLLER WA_EV;

String vehicleStatus;
uint8_t maxCurrentPermissible;
uint8_t charginCurrentViaCP;
uint8_t statusInputs;
uint8_t statusOutputs;
uint16_t timeConnectHH;
uint8_t timeConnectMM;
uint8_t timeConnectSS;
uint16_t timeChargingHH;
uint8_t timeChargingMM; 
uint8_t timeChargingSS;
uint8_t errorStatus;
uint8_t en;
String value;

String voltageA, voltageB, voltageC;
String currentA, currentB, currentC;
String powerA, powerB, powerC;
String energy;

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  Serial2.begin(115200);
  while (! Serial2);
  delay(1000);
  WA_EV.init(Serial2);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  /*
  // setting max current 6A - 32A
  // Phoenix set address 22000 setting the maximum permissible charging current
  // WA Set max current between 6 and 32 A Command 0x3A 0x30 Max Current 0x00
  if(WA_EV.setSettingMaxCurrentPermissible(32))
  {
    Serial.println("Set maximum permissible charging current OK");
  }
  Serial.println();
  vTaskDelay(10 / portTICK_PERIOD_MS);
 */
  
  /*
  // Phoenix set address 22000 setting the maximum permissible charging current
  // Read EVSE max current configuration (Amperes in HEX) Command 0x3A 0x32 0x30 0x00
  if(WA_EV.getSettingMaxCurrentPermissible(maxCurrentPermissible))
  {
    Serial.print("Max current permissilbe = "); Serial.println(maxCurrentPermissible);
  }
  Serial.println();
  vTaskDelay(10 / portTICK_PERIOD_MS);
  */
  
  /*
  // Phoenix address 4000 set Configuring the enable charging process function
  // WA mode 0 alway enable HW dependent(auto mode) Command 0x31 0x30 0x30 0x00, mode 3 Charging enable SW dependent (manual mode) Command 0x31 0x30 0x31 0x00
  // value is 0, 3
  if(WA_EV.setEn(3))
  {
    Serial.println("Set OK");
  }
  Serial.println();
  vTaskDelay(10 / portTICK_PERIOD_MS);
  */
  
  /*
  // Phoenix address 4000 get Configuring the enable charging process function
  // WA Read remote configuration manual/auto Command 0x31 0x32 0x30 0x00
  // en = 0 is mode 0, en = 3 is mode 3
  if(WA_EV.getEn(en))
  {
    Serial.print("en = ");
    Serial.println(en);
  }
  Serial.println();
  vTaskDelay(10 / portTICK_PERIOD_MS);
  */

  /*
  // Phoenix address 20000 set enabling the charging process
  // WA Force remote OFF (only SW dependent) Command 0x31 0x31 0x30 0x00, Force remote ON (only SW dependent) Command 0x31 0x31 0x31 0x00
  // pass value "ON" is enable or "OFF" is disable
  if(WA_EV.setCoil20000("OFF"))
  {
    Serial.print("Set OK"); 
  }
  Serial.println();
  vTaskDelay(10 / portTICK_PERIOD_MS);
  */

  /*
  // Phoenix address 20000 get enabling the charging process
  // WA Read EVSEready register Command 0x31 0x33 0x31 0x00
  if(WA_EV.getCoil20000(value))
  {
    Serial.print("value coil 20000 = ");   Serial.println(value);
  }
  Serial.println();
  vTaskDelay(10 / portTICK_PERIOD_MS);
  */

  // meter
  // Set phase configuration meter
  // pass 1 is set 1 phase
  // pass 3 is set 3 phase
  if(WA_EV.setPhaseMeter(1))
  {
    Serial.println("Set phase meter OK");
  }
  Serial.println();
}

void loop() { 

  vehicleStatus = "";
  if(WA_EV.getVehicleStatus(vehicleStatus))
  {
    Serial.print("Vehicle status = ");  Serial.println(vehicleStatus);
  }
  Serial.println();
  vTaskDelay(10 / portTICK_PERIOD_MS);
  

  if(WA_EV.getChargingCurrentViaCP(charginCurrentViaCP))
  {
    Serial.print("Charging current specification via signal CP = ");  Serial.println(charginCurrentViaCP);
  }
  Serial.println();
  vTaskDelay(10 / portTICK_PERIOD_MS);

  if(WA_EV.getErrorStatus(errorStatus))
  {
    // Error status
    Serial.print("Error status = ");  Serial.println(errorStatus, HEX);
    switch(errorStatus)
    {
      case 0x30 :
        Serial.println("_FAULT");
        break;
      case 0x31 :
        Serial.println("_NORMAL");
        break;
      case 0x32 :
        Serial.println("_OV");
        break;
      case 0x33 :
        Serial.println("_UV");
        break;
      case 0x34 :
        Serial.println("_EOL");
        break;
      case 0x35 :
        Serial.println("_RESET");
        break;
      case 0x36 :
        Serial.println("_ST");
        break;
      case 0x37 :
        Serial.println("_TBC");
        break;
    }
  }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  Serial.println();
  
  /*
  vehicleStatus = "";
  Serial.println("All status");
  if(WA_EV.getAllStatus(vehicleStatus, charginCurrentViaCP,statusInputs, statusOutputs, timeConnectHH , timeConnectMM, timeConnectSS
   , timeChargingHH, timeChargingMM, timeChargingSS, errorStatus))
  {
    Serial.print("Vehicle status = ");  Serial.println(vehicleStatus);
    Serial.println();

    Serial.print("Charging current specification via signal CP = ");  Serial.println(charginCurrentViaCP);
    Serial.println();

    Serial.print("Status of digital inpus = ");  Serial.println(statusInputs, BIN);

    Serial.print("Status of digital outputs = ");  Serial.println(statusOutputs, BIN);
    Serial.println();

    Serial.print("Time connect Hours = ");
    Serial.println(timeConnectHH);
    Serial.print("Time connect Minutes = ");
    Serial.println(timeConnectMM);
    Serial.print("Time connect Second = ");
    Serial.println(timeConnectSS);
    Serial.println();

    Serial.print("Time charging Hours = ");
    Serial.println(timeChargingHH);
    Serial.print("Time charging Minutes = ");
    Serial.println(timeChargingMM);
    Serial.print("Time charging Second = ");
    Serial.println(timeChargingSS);
    Serial.println();

    // Error status
    Serial.print("Error status = ");  Serial.println(errorStatus, HEX);
    switch(errorStatus)
    {
      case 0x30 :
        Serial.println("_FAULT");
        break;
      case 0x31 :
        Serial.println("_NORMAL");
        break;
      case 0x32 :
        Serial.println("_OV");
        break;
      case 0x33 :
        Serial.println("_UV");
        break;
      case 0x34 :
        Serial.println("_EOL");
        break;
      case 0x35 :
        Serial.println("_RESET");
        break;
      case 0x36 :
        Serial.println("_ST");
        break;
      case 0x37 :
        Serial.println("_TBC");
        break;
    }
  }
  */

  
  if(WA_EV.readParameter1Phase(voltageA, currentA, powerA, energy))
  {
    Serial.print("Voltage = "); Serial.println(voltageA);
    Serial.print("Current = "); Serial.println(currentA);
    Serial.print("Power = ");   Serial.println(powerA);
    Serial.print("Energy = ");  Serial.println(energy);
  }
  vTaskDelay(10 / portTICK_PERIOD_MS);
  Serial.println();
  
  
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
