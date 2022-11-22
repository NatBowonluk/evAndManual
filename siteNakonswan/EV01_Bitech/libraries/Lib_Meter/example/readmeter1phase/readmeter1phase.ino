#include <METER.h>

METER meter;

String voltage, current, power, energy;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");

  Serial2.begin(9600);

  meter.init(1, Serial2);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Loop");

  //  voltage = "";  current = "";  power = "";
  if (meter.readParameter1Phase(voltage, current, power))
  {
    Serial.print("Voltage = "); Serial.println(voltage);
    Serial.print("Current = "); Serial.println(current);
    Serial.print("Power = "); Serial.println(power);
  }


  // Should take time delay 10 milliseconds
  delay(10);

  energy = "";
  if (meter.readEnergy1Phase(energy))
  {
    Serial.print("Energy = ");  Serial.println(energy);
  }

  delay(1000);
}
