#include <RCSwitch.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>

const int SEND_TEAC_PIN = D4;
const int SEND_SAMSUNG_PIN = D5;
const int SEND_SWITCH_PIN = D6;
const int SEND_LED_PIN = D7;

IRsend irsendTeac = IRsend(SEND_TEAC_PIN);
IRsend irsendSamsung = IRsend(SEND_SAMSUNG_PIN);
IRsend irsendLED = IRsend(SEND_LED_PIN);
RCSwitch rfSender = RCSwitch();

const char TYPE[][10] = {"samsung", "teac", "switch", "led"};

void initSender() {
  irsendTeac.begin();
  irsendSamsung.begin();
  irsendLED.begin();
  rfSender.enableTransmit(SEND_SWITCH_PIN);
}

void sendHexCode(const char * payload) {
  if (strstr(payload, TYPE[0]) != 0) {
    irsendSamsung.sendSAMSUNG(getHexCodeFromPayload((char*) payload), 32);
    Serial.println("SAMSUNG");
  }
  else if (strstr(payload, TYPE[1]) != 0) {
    irsendTeac.sendNEC(getHexCodeFromPayload((char*) payload), 32);
    Serial.println("TEAC");
  }
  else if (strstr(payload, TYPE[2]) != 0) {
    rfSender.send(getHexCodeFromPayload((char*) payload), 24);
    Serial.println("SWITCH");
  }
  else if (strstr(payload, TYPE[3]) != 0) {
    irsendLED.sendNEC(getHexCodeFromPayload((char*) payload), 32);
    Serial.println("LED");
  }
  else {
    Serial.println("Unknown type");
  }
}
