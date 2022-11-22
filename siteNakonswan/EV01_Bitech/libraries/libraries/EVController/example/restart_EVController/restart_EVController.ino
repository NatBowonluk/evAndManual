#include <EVController.h>

EVCONTROLLER ev;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");

  Serial2.begin(9600);
  ev.init(2, Serial2);
  delay(1000);

  delay(1000);

  if(ev.restartEVController())
  {
    Serial.println("EV Controller restart OK");
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
