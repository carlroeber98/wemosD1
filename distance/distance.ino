#include <ArduinoOTA.h>
#include <Math.h>
#include <RCSwitch.h>

const int SENDER = D4;
const int TRIGGER = D3;
const int ECHO = D2;

RCSwitch rfSender = RCSwitch();

boolean action = false;

int actions = 0;
int calculations = 0;

int distanceBefore;

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  rfSender.enableTransmit(SENDER);
  digitalWrite(TRIGGER, LOW);
}

void loop() {
 if(calculations > -1){
  calculations++;
 }
 if(calculations == 30) {
    if(actions > 0){
      if(actions == 1){
        rfSender.send(0xFFFFF1, 24);
      }else if (actions == 2){
        Serial.println(2);
      }else if (actions == 3){
        Serial.println(3);
      }
    }
    actions = 0;
    calculations = -1;
 }
 int distance = measureDistance();
 //Serial.println(entfernung, DEC) ;
 if(distanceBefore != NULL && !action &&
  distanceBefore - distance > 10){
    action = true;
    actions++;
    if(calculations == -1){
      calculations = 0;
    }
 }else if(action){
    action = false;
 }

 distanceBefore = distance;
 delay(50);
}


int measureDistance(){ 
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
