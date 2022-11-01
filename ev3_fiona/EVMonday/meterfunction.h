uint8_t meterZeroCount = 0;
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
    register float readValue[4];
    meter.readParameter1Phase(electric.voltage1, electric.current1, electric.power1);
    swDelay(10);
    meter.readEnergy1Phase(electric.energy);
  }
  else if (phase == 3)
  {
    //        meter.readParameter3Phase(electric.voltage3[0], electric.voltage3[1], electric.voltage3[2], electric.current3[0], electric.current3[1], electric.current3[2], electric.power3[0], electric.power3[1], electric.power3[2]);
    meter.readParameter3Phase(electric.voltage3[0], electric.current3[0], electric.current3[1], electric.current3[2], electric.power3[0], electric.energy);
    swDelay(10);
    meter.readEnergy3Phase(electric.energy);

  }
}
#else
void readmeter(uint8_t phase)
{
  if (phase == 1)
  {
    //    ev.readParameter1Phase(electric.voltage1, electric.current1, electric.power1, electric.energy);
    register float readValue[4];
    ev.readParameter1Phase(readValue[0], readValue[1], readValue[2], electric.energy);

    if ((readValue[0] == 0) && (meterZeroCount < 4)) {
      meterZeroCount++;
      return;
    }

    register float real_value[4];

    for (register uint8_t i = 0; i < 4; i++) {
      real_value[i] = readValue[i];
      Serial.print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa :");
      Serial.println(readValue[i]);
    }
    // add a noise to the reference value and use as the measured value
    register float measured_value[4];

    for (register uint8_t i = 0; i < 4; i++) {
      measured_value[i] = real_value[i] + random(-100, 100) / 100.0;
    }
    // calculate the estimated value with Kalman Filter
    register float estimated_value[4];

    //    for (register uint8_t i = 0; i < 4; i++) {
    //      estimated_value[i] = barryKalmanFilter.updateEstimate(measured_value[i]);
    //    }

    estimated_value[0] = voltageKalmanFilter.updateEstimate(measured_value[0]);
    //estimated_value[1] = c2.updateEstimate(measured_value[i]);
    //estimated_value[1] = c.updateEstimate(measured_value[i]);
    estimated_value[1] = current1KalmanFilter.updateEstimate(measured_value[1]);
    estimated_value[2] = powerKalmanFilter.updateEstimate(measured_value[2]);
    estimated_value[3] = energyKalmanFilter.updateEstimate(measured_value[3]);

    electric.voltage1 = abs(estimated_value[0]);

    Serial.println(estimated_value[1]);
    Serial.println("/////////////////////////////////////////////////////////////////////////////////////////////////////");

    if (readValue[0] < 0.03) {
      electric.current1 = 0;
      electric.power1 = 0;
      // electric.energy = 0;
    }
    else {
      electric.current1 = abs(estimated_value[1]);
      electric.power1 = abs(estimated_value[2]);
    }
    //    electric.energy = abs(estimated_value[3]);

    meterZeroCount = 0;
    //    debugln(String(electric.voltage1));
    //    debugln(String(electric.current1));
    //    debugln(String(electric.power1));
    //    debugln(String(electric.energy));
  }
  else if (phase == 3)
  {
    //    ev.readParameter3Phase(electric.voltage3[0], electric.voltage3[1], electric.voltage3[2], electric.current3[0], electric.current3[1], electric.current3[2], electric.power3[0], electric.power3[1], electric.power3[2], electric.energy);

    //    ev.readParameter3Phase(electric.voltage3[0], electric.current3[0], electric.current3[1], electric.current3[2], electric.power3[0], electric.energy);
    register float readValue[6];
    ev.readParameter3Phase(readValue[0], readValue[1], readValue[2], readValue[3], readValue[4], electric.energy);
    swDelay(10);
    //    meter.readEnergy3Phase(readValue[5]);

    if ((readValue[0] == 0) && (meterZeroCount < 4)) {
      meterZeroCount++;
      return;
    }

    //    if (readValue[1] < 0.03 || readValue[2] < 0.03 || readValue[3] < 0.03) {
    //      readValue[1] = 0;
    //      readValue[2] = 0;
    //      readValue[3] = 0;
    //      readValue[4] = 0;
    //    }
    register float real_value[6];

    for (register uint8_t i = 0; i < 6; i++) {
      real_value[i] = readValue[i];
      Serial.print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa :");
      Serial.println(readValue[i]);
    }
    // add a noise to the reference value and use as the measured value
    register float measured_value[6];

    for (register uint8_t i = 0; i < 6; i++) {
      measured_value[i] = real_value[i] + random(-100, 100) / 100.0;
    }
    // calculate the estimated value with Kalman Filter
    register float estimated_value[6];

    estimated_value[0] = voltageKalmanFilter.updateEstimate(measured_value[0]);
    estimated_value[1] = current1KalmanFilter.updateEstimate(measured_value[1]);
    estimated_value[2] = current2KalmanFilter.updateEstimate(measured_value[2]);
    estimated_value[3] = current3KalmanFilter.updateEstimate(measured_value[3]);
    estimated_value[4] = powerKalmanFilter.updateEstimate(measured_value[4]);
    estimated_value[5] = energyKalmanFilter.updateEstimate(measured_value[5]);

    electric.voltage3[0] = abs(estimated_value[0]);
    if (readValue[0] < 0.03 ) {
      //      readValue[1], readValue[2], readValue[3]
     electric.current3[0] = 0;
     electric.current3[1] = 0;
     electric.current3[2] = 0;
      electric.power3[0] = 0;
    }
    else {
       if (readValue[1] < 0.5) {
        electric.current3[0] = 0;
      }
      else{
         electric.current3[0] = abs(estimated_value[1]);
      }
      if (readValue[2] < 0.5) {
        electric.current3[1] = 0;
      }
      else{
         electric.current3[1] = abs(estimated_value[2]);
      }
      if (readValue[3] < 0.5) {
        electric.current3[2] = 0;
      }
      else{
         electric.current3[2] = abs(estimated_value[3]);
      }
      electric.power3[0] = abs(estimated_value[4]);
      //      // electric.energy = 0;
    }
    //    electric.energy = abs(estimated_value[5]);

    electric.voltage3[1] = abs(electric.voltage3[0]);
    electric.voltage3[2] = abs(electric.voltage3[0]);

    electric.power3[1] = abs(electric.power3[0]);
    electric.power3[2] = abs(electric.power3[0]);

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


    meterZeroCount = 0;
  }
}
#endif
