#include <WAEVController.h>

WAEVCONTROLLER WA_EV;

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  Serial2.begin(115200);
  while (! Serial2);
  delay(1000);
  WA_EV.init(Serial2);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println("Start");

  if(WA_EV.restartEVController())
  {
    Serial.println("EV Controller restart OK");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
