#include <RCSwitch.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>

const int STATE_LED = D3;
const int SEND_TEAC_PIN = D4;
const int SEND_SAMSUNG_PIN = D5;
const int SEND_SWITCH_PIN = D6;
const int SEND_LED_PIN = D7;

IRsend irsendTeac = IRsend(SEND_TEAC_PIN);
IRsend irsendSamsung = IRsend(SEND_SAMSUNG_PIN);
IRsend irsendLED = IRsend(SEND_LED_PIN);
RCSwitch rfSender = RCSwitch();

const char TYPE[][10] = {"SAMSUNG", "TEAC", "SWITCH", "LED"};

void initializePins() {
  irsendTeac.begin();
  irsendSamsung.begin();
  irsendLED.begin();
  rfSender.enableTransmit(SEND_SWITCH_PIN);
  //pinMode(STATE_LED, OUTPUT);
}

void sendHexCode(const char* type, char* code) {
  Serial.println(code);
  Serial.println(getHexCodeFromChar(code));
  if (strstr(type, TYPE[0]) != 0) {
    irsendSamsung.sendSAMSUNG(getHexCodeFromChar(code), 32);
    Serial.println("SAMSUNG");
  }
  else if (strstr(type, TYPE[1]) != 0) {
    irsendTeac.sendNEC(getHexCodeFromChar(code), 32);
    Serial.println("TEAC");
  }
  else if (strstr(type, TYPE[2]) != 0) {
    rfSender.send(getHexCodeFromChar(code), 24);
    Serial.println("SWITCH");
  }
  else if (strstr(type, TYPE[3]) != 0) {
    irsendLED.sendNEC(getHexCodeFromChar(code), 32);
    Serial.println("LED");
  }
  else {
    Serial.println("Unknown type");
  }
}
