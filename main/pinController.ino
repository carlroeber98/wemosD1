#include <RCSwitch.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>

const int TRIGGER = D2;
const int ECHO = D3;
const int SEND_TEAC_PIN = D4;
const int SEND_SAMSUNG_PIN = D5;
const int SEND_SWITCH_PIN = D6;
const int SEND_LED_PIN = D7;
const int RESEIVER = D8;

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
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIGGER, LOW);
}

void getAndSendHexCode(const char* type, char* code) {
  sendHexCode(type, getHexCodeFromChar(code));
}

void sendHexCode(const char* type, uint32_t code){
  Serial.println(code, HEX);
  if (strstr(type, TYPE[0]) != 0) {
    irsendSamsung.sendSAMSUNG(code, 32);
    Serial.println("SAMSUNG");
  }
  else if (strstr(type, TYPE[1]) != 0) {
    irsendTeac.sendNEC(code, 32);
    Serial.println("TEAC");
  }
  else if (strstr(type, TYPE[2]) != 0) {
    rfSender.send(code, 24);
    Serial.println("SWITCH");
  }
  else if (strstr(type, TYPE[3]) != 0) {
    irsendLED.sendNEC(code, 32);
    Serial.println("LED");
  }
  else {
    Serial.println("Unknown type");
  }
}

int measureDistance() { 
   long t = 0;
   digitalWrite(TRIGGER, LOW); 
   delayMicroseconds(3);
   noInterrupts();
   digitalWrite(TRIGGER, HIGH);
   delayMicroseconds(10);
   digitalWrite(TRIGGER, LOW); 
   t = pulseIn(ECHO, HIGH);
   interrupts(); 
   return ((t/2) / 29.1); 
}
