
#if EVCONTROL == 1
void initmeter(uint8_t phase);
#else
#endif
void readmeter(uint8_t phase);

#if EVCONTROL == 1
void initmeter(uint8_t phase)
{
  meter.init(phase, Serial2);
}
void readmeter(uint8_t phase)
{
  if (phase == 1)
  {
    meter.readParameter1Phase(electric.voltage1, electric.current1, electric.power1);
    swDelay(10);
    meter.readEnergy1Phase(electric.energy);
//    debugln(String(electric.voltage1));
//    debugln(String(electric.current1));
//    debugln(String(electric.power1));
//    debugln(String(electric.energy));
  }
  else if (phase == 3)
  {
//    meter.readParameter3Phase(electric.voltage3[0], electric.voltage3[1], electric.voltage3[2], electric.current3[0], electric.current3[1], electric.current3[2], electric.power3[0], electric.power3[1], electric.power3[2]);
    
    meter.readParameter3Phase(electric.voltage3[0], electric.current3[0], electric.current3[1], electric.current3[2], electric.power3[0], electric.energy);

    electric.voltage3[1] = electric.voltage3[0];
    electric.voltage3[2] = electric.voltage3[0];

    electric.power3[1] = electric.power3[0];
    electric.power3[2] = electric.power3[0];
    
//    debug("electric.voltage3[0]"); debugln(String(electric.voltage3[0]));
//    debug("electric.voltage3[1]"); debugln(String(electric.voltage3[1]));
//    debug("electric.voltage3[2]"); debugln(String(electric.voltage3[2]));
//    debug("electric.current3[0]"); debugln(String(electric.current3[0]));
//    debug("electric.current3[1]"); debugln(String(electric.current3[1]));
//    debug("electric.current3[2]"); debugln(String(electric.current3[2]));
//    debug("electric.power3[0]"); debugln(String(electric.power3[0]));
//    debug("electric.power3[1]"); debugln(String(electric.power3[1]));
//    debug("electric.power3[2]"); debugln(String(electric.power3[2]));
    //    debug("electric.voltage3[0]");debugln(String(electric.voltage3[0]));
    swDelay(10);
    meter.readEnergy3Phase(electric.energy);
  }
}
#else
void readmeter(uint8_t phase)
{
  if (phase == 1)
  {
    ev.readParameter1Phase(electric.voltage1, electric.current1, electric.power1, electric.energy);
    
    
    swDelay(10);
    //    meter.readEnergy1Phase(electric.energy);
//    debugln(String(electric.voltage1));
//    debugln(String(electric.current1));
//    debugln(String(electric.power1));
//    debugln(String(electric.energy));
  }
  else if (phase == 3)
  {
//    ev.readParameter3Phase(electric.voltage3[0], electric.voltage3[1], electric.voltage3[2], electric.current3[0], electric.current3[1], electric.current3[2], electric.power3[0], electric.power3[1], electric.power3[2], electric.energy);
    
    ev.readParameter3Phase(electric.voltage3[0], electric.current3[0], electric.current3[1], electric.current3[2], electric.power3[0], electric.energy);
    
    electric.voltage3[1] = electric.voltage3[0];
    electric.voltage3[2] = electric.voltage3[0];

    electric.power3[1] = electric.power3[0];
    electric.power3[2] = electric.power3[0];
    
    swDelay(10);
//    debug("electric.voltage3[0]"); debugln(String(electric.voltage3[0]));
//    debug("electric.voltage3[1]"); debugln(String(electric.voltage3[1]));
//    debug("electric.voltage3[2]"); debugln(String(electric.voltage3[2]));
//    debug("electric.current3[0]"); debugln(String(electric.current3[0]));
//    debug("electric.current3[1]"); debugln(String(electric.current3[1]));
//    debug("electric.current3[2]"); debugln(String(electric.current3[2]));
//    debug("electric.power3[0]"); debugln(String(electric.power3[0]));
//    debug("electric.power3[1]"); debugln(String(electric.power3[1]));
//    debug("electric.power3[2]"); debugln(String(electric.power3[2]));
    //    debug("electric.energy"); debugln(String(electric.energy);
    //    debug("electric.voltage3[0]");debugln(String(electric.voltage3[0]));
    //    swDelay(10);
    //    meter.readEnergy3Phase(electric.energy);
  }
}
#endif
