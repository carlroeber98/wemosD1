#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <Math.h>
#include <WebSocketClient.h>

const char* ssid = "ruessel";
const char* password = "Elinet123";
char path[] = "/";
char host[] = "192.168.0.212";
  
WebSocketClient webSocketClient;
WiFiClient client;

const int trigger = D6;
const int echo = D7;

boolean action = false;

int actions = 0;
int calculations = 0;

int distanceBefore;

void setup() {
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigger, LOW);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
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
      
      if (client.connect("192.168.0.212", 81)) {
        Serial.println("Connected");
      } else {
        Serial.println("Connection failed.");
      }
    
      // Handshake with the server
      webSocketClient.path = path;
      webSocketClient.host = host;
      if (client.connected() && webSocketClient.handshake(client)) {
        Serial.println("Handshake successful");
      } else {
        Serial.println("Handshake failed.");
      }

      if(client.connected()){
        if(actions == 1){
          Serial.println(1);
          webSocketClient.sendData("samsung;0xE0E040BF");
        }else if (actions == 2){
          Serial.println(2);
          webSocketClient.sendData("teac;0xA156E916");
        }else if (actions == 3){
          Serial.println(3);
        }else if (actions == 4){
          Serial.println(4);
        }
      }
    }
    if (!client.connected()) {  
       client.stop();
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
