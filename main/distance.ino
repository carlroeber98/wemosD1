#include <Math.h>

boolean action = false;

int actions = 0;
int calculations = 0;

int distanceBefore;

void measureDistanceLoop() {
 if(calculations > -1){
  calculations++;
 }
 if(calculations == 30) {
    if(actions > 0){
      if(actions == 1){
        Serial.println("ON");
        sendHexCode("SWITCH", 0x114551);
        delay(5);
        sendHexCode("SWITCH", 0x115511);
        delay(5);
        sendHexCode("SWITCH", 0x115451);
        delay(5);
        sendHexCode("SWITCH", 0x154555);
        Serial.println("FINISH");
      }else if (actions == 2){
        Serial.println("OFF");
        sendHexCode("SWITCH", 0x114554);
        delay(5);
        sendHexCode("SWITCH", 0x115514);
        delay(5);
        sendHexCode("SWITCH", 0x151554);
        delay(5);
        sendHexCode("SWITCH", 0x154554);
        delay(5);
        sendHexCode("SWITCH", 0x155154);
        Serial.println("FINISH");
      }else if (actions == 3){
        //goOnline(false);
        Serial.println(3);
      }
    }
    actions = 0;
    calculations = -1;
    return;
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
}
