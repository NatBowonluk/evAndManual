void callback(char* topic, byte* payload, unsigned int length);
void mqttSetup(bool const &activate, String const &serverIP, String const &mqttPort);
void mqttProcess(String const &clientID, String const &user, String const &pass);

void callback(char* topic, byte* payload, unsigned int length) {
  //  DynamicJsonBuffer *jsonBuffer = new DynamicJsonBuffer[1];

  String *messageTemp = new String[1]; //0 = payloadtemp
  unsigned int *counter = new unsigned int[1];
  const char **commandTemp = new const char*[1];

  Serial.print(F("Message arrived on topic: "));
  Serial.println(topic);
  Serial.print(F("Message: "));

  for (counter[0] = 0; counter[0] < length; counter[0]++) {
    Serial.print((char)payload[counter[0]]);
    messageTemp[0] += (char)payload[counter[0]];
  }

  Serial.println(F(""));

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(messageTemp[0]);

  commandTemp[0] = root["command"];

  if (!commandTemp[0])
  {
    Serial.println(F("NOT IN COMMAND"));
    delete[] commandTemp;
    delete[] messageTemp;
    delete[] counter;
    return;
  }

  String *commandEV = new String[1];

  commandEV[0] = commandTemp[0];

  if (commandEV[0] == "set") {
    Serial.println(F("SET"));
    bool *allowEV = new bool[1];
    float *energyEV = new float[1];
    unsigned int *periodTotalMinEV = new unsigned int[1];
    unsigned int *periodHours = new unsigned int[1];
    unsigned int *periodMinutes = new unsigned int[1];

    allowEV[0] = root["allow"];
    energyEV[0] = root["energy"];
    periodTotalMinEV[0] = root["period"];
    minTotalToSplit(periodTotalMinEV[0], periodMinutes[0], periodHours[0]);

    //set EV function start here

    delete[] allowEV;
    delete[] energyEV;
    delete[] periodTotalMinEV;
    delete[] periodHours;
    delete[] periodMinutes;
  }
  if (commandEV[0] == "start") {
    Serial.println(F("START"));
    //start charging function here
  }
  if (commandEV[0] == "reset") {
    Serial.println(F("RESET"));
    //reset what???
  }
  if (commandEV[0] == "current") {
    Serial.println(F("CURRENT"));
    uint8_t *changingCurrentEV = new uint8_t[1];

    changingCurrentEV[0] = root["amp"];
    //set changing current funcion here
  }
  if (commandEV[0] == "alive") {
    Serial.println(F("ALIVE"));
    DynamicJsonBuffer jsonAlive;
    JsonObject& rootAlive = jsonAlive.createObject();

    String *sendingData = new String[1];
    rootAlive.printTo(sendingData[0]);
    mqtt.publish(publishTopic.c_str(), sendingData[0].c_str());

    delete[] sendingData;
  }
  if (commandEV[0] == "config") {
    Serial.println(F("CONFIG"));
  }

  //  delete[] jsonBuffer;
  delete[] commandEV;
  delete[] commandTemp;
  delete[] messageTemp;
  delete[] counter;
}
void mqttSetup(bool const &activate, String const &serverIP, unsigned int const &mqttPort) {
  if (!activate) {
    Serial.println(F("MQTT is not ACTIVATE"));
    return;
  }
  mqtt.setServer(serverIP.c_str(), mqttPort);
  mqtt.setCallback(callback);
}

void mqttProcess(String const &clientID, String const &user, String const &pass) {
  WiFi.mode(WIFI_MODE_APSTA);
  if (mqtt.connected() == false) {
    Serial.print(F("MQTT connection.."));
    if (mqtt.connect(clientID.c_str(), user.c_str(), pass.c_str())) {
      Serial.println(F("connected"));
      mqtt.subscribe(subscribeTopic.c_str());
    } else {
      Serial.print(F("failed, rc="));
      Serial.println(mqtt.state());
      swDelay(1000);
    }
  } else {
    mqtt.loop();
  }
}
