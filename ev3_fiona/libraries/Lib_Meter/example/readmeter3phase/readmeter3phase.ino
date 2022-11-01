#include <METER.h>

METER meter;

float voltageA, voltageB, voltageC, voltageAVG;
float currentA, currentB, currentC;
float powerA, powerB, powerC, powerTotal;
float energy;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");

  Serial2.begin(9600);

  meter.init(3, Serial2);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Loop");

  //voltage = "";  current = "";  power = "";
  /*
  if (meter.readParameter3Phase(voltage, current, power))
  {
    
    Serial.print("Voltage1 = "); Serial.println(voltage[0]);
    Serial.print("Voltage2 = "); Serial.println(voltage[1]);
    Serial.print("Voltage3 = "); Serial.println(voltage[2]);

    Serial.print("Current1 = "); Serial.println(current[0]);
    Serial.print("Current2 = "); Serial.println(current[1]);
    Serial.print("Current3 = "); Serial.println(current[2]);

    Serial.print("Power1 = "); Serial.println(power[0]);
    Serial.print("Power2 = "); Serial.println(power[1]);
    Serial.print("Power3 = "); Serial.println(power[2]);
  }
  */

  if(meter.readParameter3Phase(voltageAVG, currentA, currentB, currentC, powerTotal, energy))
  {
    Serial.print("Voltage AVG = "); Serial.println(voltageAVG);
    Serial.print("Current A = "); Serial.println(currentA);
    Serial.print("Current B = "); Serial.println(currentB);
    Serial.print("Current C = "); Serial.println(currentC);
    Serial.print("Power Total = ");   Serial.println(powerTotal);
    Serial.print("Energy = ");    Serial.println(energy);
  }
  // Should take time delay 10 milliseconds
  delay(10);

  //energy = "";
  if (meter.readEnergy3Phase(energy))
  {
    Serial.print("Energy = ");  Serial.println(energy);
  }

  delay(1000);
}
