String ipToString(IPAddress ip);
void swDelay(uint value);
bool itIsTime(uint32_t t, uint32_t t0, uint16_t dt);
String getValue(String data, char separator, int index);
String OneConnectedApMac(void);
void minTotalToSplit(unsigned int const &total, unsigned int &minutesCut, unsigned int &hoursCut);
void textToHex(const char* input, char* output);
void createSerialNumber(String &serialNumber);
void createTopic(String const &mainTopic, String const &subTopic, String &topic);

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

void swDelay(uint value) {
  vTaskDelay(value / portTICK_PERIOD_MS);
}

bool itIsTime(uint32_t t, uint32_t t0, uint16_t dt) {
  return ((t >= t0) && (t - t0 >= dt)) ||         // The first disjunct handles the normal case
            ((t < t0) && (t + (~t0) + 1 >= dt));  //   while the second handles the overflow case
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String OneConnectedApMac() {
  char str[20];

  wifi_sta_list_t wifi_sta_list;
  tcpip_adapter_sta_list_t adapter_sta_list;

  memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
  memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

  esp_wifi_ap_get_sta_list(&wifi_sta_list);
  tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

  for (int i = 0; i < adapter_sta_list.num; i++) {

    tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];

    //    debug("station nr ");
    //    debugln(i);

    //    debug("MAC: ");

    //    for (int i = 0; i < 6; i++) {
    //
    //      debugf("%02X", station.mac[i]);
    //      if (i < 5)debug(":");
    //    }
    //    if (i == 0) {
    //      return "NoMac";
    //    }
    //    else {
    sprintf(str, "%02X:%02X:%02X:%02X:%02X:%02X", station.mac[0], station.mac[1], station.mac[2], station.mac[3], station.mac[4], station.mac[5]);
    //    }
    //    debug(String(str));

    //    debug("\nIP: ");
    //    debugln(ip4addr_ntoa(&(station.ip)));
  }
  //  debug("Size of str : ");
  //  debugln(sizeof(String(str)));
  for (int i = 0; i < String(str).length(); i++) {
    if (str[i] == ':') {
      i = String(str).length();
      return String(str);
    }
    if (i == ((String(str).length()) - 1)) {
      return "NoMac";
    }
  }
}

void minTotalToSplit(unsigned int const &total, unsigned int &minutesCut, unsigned int &hoursCut)
{
  minutesCut = total % 60;
  hoursCut = total / 60;
}

void textToHex(const char* input, char* output)
{
  int *loop = new int[1];
  int *i = new int[1];

  i[0] = 0;
  loop[0] = 0;

  while (input[loop[0]] != '\0')
  {
    sprintf((char*)(output + i[0]), "%02X", input[loop[0]]);
    loop[0] += 1;
    i[0] += 2;
  }
  //insert NULL at the end of the output string
  output[i[0]++] = '\0';

  delete[] loop;
  delete[] i;
}

void createSerialNumber(String &serialNumber) {
  uint32_t *chipId = new uint32_t;
  uint32_t *counter = new uint32_t;
  chipId[0] = 0;
  for (counter[0] = 0; counter[0] < 17; counter[0] = counter[0] + 8) {
    chipId[0] |= ((ESP.getEfuseMac() >> (40 - counter[0])) & 0xff) << counter[0];
  }

  String *prefix = new String[1];
  prefix[0] = "BS";
  prefix[0] += "EV02";
  prefix[0] += chipId[0];
  uint32_t *len = new uint32_t[1];
  len[0] = prefix[0].length();
  char *hexStr = new char[(len[0] * 2) + 1];
  textToHex(prefix[0].c_str(), hexStr);
  String *result = new String[1];
  //  printf("hex_str: %s\n", hexStr);
  result[0] = hexStr;
  serialNumber = result[0];

  delete[] len;
  delete[] hexStr;
  delete[] prefix;
  delete[] chipId;
  delete[] counter;
  delete[] result;
}

void createTopic(String const &mainTopic, String const &subTopic, String &topic) {
  String *temp = new String[1];

  temp[0] = mainTopic;
  temp[0] += "/";
  temp[0] += subTopic;

  topic = temp[0];

  delete[] temp;
}
