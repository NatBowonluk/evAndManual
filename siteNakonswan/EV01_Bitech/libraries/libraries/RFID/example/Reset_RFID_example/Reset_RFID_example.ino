#include <RFID.h>
RFID rfid;

#define reset_rfids 15

String id;
boolean success;

int count = 0;

void reset_rfid();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Start");

  // initial rfid return boolean, True = initial ok, false = initial fail
  pinMode(reset_rfids, OUTPUT);

  reset_rfid();
  bool flag_check = true;
  while (flag_check) {
    if (rfid.init())
    {
      Serial.println("RFID Ready");
      flag_check = false;
    }
    else
    {
      Serial.println("RFID Fail");
      delay(1000);
      count += 1;
      if (count <= 4) {
        reset_rfid();
        flag_check = true;
      }
      else{
        flag_check = false;
      }
    }
  }
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  /* return hex id
      getidHex(String &id, boolean &success)
  */
  // rfid.getIdHex(id, success);

  /* return decimal id
      getidDecimal(String &id, boolean &success)
  */

  rfid.getIdDecimal(id, success);
  if (success)
  {
    Serial.println();
    Serial.print("Outside id = ");
    Serial.println(id);
  }
  else {
    Serial.println("Timed out waiting for a card");
  }

  delay(1000);
}

void reset_rfid() {
  Serial.println();
  digitalWrite(reset_rfids,  HIGH);
  delay(100);
  digitalWrite(reset_rfids,  LOW);

}
