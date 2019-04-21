#include <ArduinoOTA.h>
#include <Math.h>

const int trigger = D6;
const int echo = D7;

boolean action = false;

int actions = 0;
int calculations = 0;

int distanceBefore;

void setup() {
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigger, LOW); //Signal abschalten

  Serial.begin(115200);
}

void loop() {
 if(calculations > -1){
  calculations++;
 }
 if(calculations == 30) {
    if(actions > 0){
      Serial.print("AKTIONS ");
      Serial.print(actions);
      Serial.println();
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


// Entfernung in cm über gewöhnlichen Ultraschallsensor mit Echo und Trigger messen
int measureDistance(){ 
 long t = 0;
 digitalWrite(trigger, LOW); 
 delayMicroseconds(3);
 noInterrupts();
 digitalWrite(trigger, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigger, LOW); 
 t = pulseIn(echo, HIGH);
 interrupts(); 
 return ((t/2) / 29.1); 
}

/*Entfernung Gleitender Durchschnitt (gewichtet)
  int getEntfernungGD() { 
  int alt=0;
 int mittel;
 int entf;
  int i;
  delay(10);
  alt=getEntfernung();
  delay(10);
  for (i=0; i<10; i++) {
   entf=getEntfernung();
   mittel=(0.8*alt) + (0.2*entf);
   alt=mittel;
   delay(10);
  }
  return (mittel);
}*/
