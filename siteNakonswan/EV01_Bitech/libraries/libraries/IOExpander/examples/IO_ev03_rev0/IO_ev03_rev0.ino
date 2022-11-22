#include <IOExpander.h>

IOEXPANDER iox;

// i2c pin
#define sda 21
#define scl 22
#define freq 400000

// input pin
#define DI1 0
#define DI2 1
#define DI3 2
#define DI4 3

// output pin
#define power_ev 7
#define connectStatus 6
#define chargeStatus 5
#define errorStatus 4
#define en_power 12
#define buzzer 13
#define ev_remote 14
#define ev_reset 15
#define DO1 8
#define DO2 9

void setup() {
  Serial.begin(115200);
  Serial.println("Start");

  // Initialization
  if (iox.init(sda, scl, freq))
  {
    Serial.println("IOExpander initialization success");
  }
  // configure pinMode
  iox.pinMode(DI1, INPUT_PULLUP);
  iox.pinMode(DI2, INPUT_PULLUP);
  iox.pinMode(DI3, INPUT_PULLUP);
  iox.pinMode(DI4, INPUT_PULLUP);
  
  iox.pinMode(power_ev, OUTPUT);
  iox.pinMode(connectStatus, OUTPUT);
  iox.pinMode(chargeStatus, OUTPUT);
  iox.pinMode(errorStatus, OUTPUT);
  iox.pinMode(en_power, OUTPUT);
  iox.pinMode(buzzer, OUTPUT);
  iox.pinMode(ev_remote, OUTPUT);
  iox.pinMode(ev_reset, OUTPUT);
  iox.pinMode(DO1, OUTPUT);
  iox.pinMode(DO2, OUTPUT);

  // on led power
  iox.digitalWrite(power_ev, HIGH);
  iox.digitalWrite(connectStatus, HIGH);
  iox.digitalWrite(chargeStatus, HIGH);
  iox.digitalWrite(errorStatus, HIGH);
  /*
  iox.digitalWrite(en_power, HIGH);
  iox.digitalWrite(buzzer, HIGH);
  iox.digitalWrite(ev_remote, HIGH);
  iox.digitalWrite(ev_reset, HIGH);
  iox.digitalWrite(DO1, HIGH);
  iox.digitalWrite(DO2, HIGH);
  */

  // read input switch
  if(iox.digitalRead(DI1))
  {
    Serial.print("DI1 : "); Serial.println("HIGH");
  }
  else
  {
    Serial.print("DI1 : "); Serial.println("LOW");
  }
}

void loop() {
 
  
}
