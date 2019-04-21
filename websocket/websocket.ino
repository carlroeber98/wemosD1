#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <IRrecv.h>
#include <ir_Samsung.h>
#include <ir_NEC.h>
#include <WebSocketsServer.h>

const char* ssid = "ruessel";
const char* password = "Elinet123";

const char* socket_user = "angular";
const char* socket_password = "FLpRduPT9gmfsCW6tHh2WDuTa2JqgJ";

WebSocketsServer webSocket = WebSocketsServer(81);

const char TYPE[][10] = {"samsung", "teac", "switch"};

const int SEND_TRIGGER = D4;
const int RESEIVE_ECHO = D7;
const int SEND_TEAC_PIN = D5; 
const int SEND_SAMSUNG_PIN = D6; 
const int SEND_SWITCH_PIN = D3; 

char delimiter[] = ";";

boolean action = false;
int actions = 0;
int calculations = 0;
int distanceBefore;

int connectedClients = 0;

MDNSResponder mdns;
ESP8266WiFiMulti WiFiMulti;

IRsend irsendTeac = IRsend(SEND_TEAC_PIN);
IRsend irsendSamsung = IRsend(SEND_SAMSUNG_PIN);

uint32_t getHexCode(char* payload){
  strtok(payload, delimiter);
  char* hex = strtok(NULL, delimiter);
  uint32_t val = 0;
  while (*hex) {
      // get current character then increment
      char byte = *hex++; 
      // transform hex character to the 4bit equivalent number, using the ascii table indexes
      if (byte >= '0' && byte <= '9') byte = byte - '0';
      else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
      else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
      // shift 4 to make space for new digit, and add the 4 bits of the new digit 
      val = (val << 4) | (byte & 0xF);
  }
  return val;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      connectedClients--;
      break;
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      connectedClients++;
      webSocket.sendTXT(num, "Connected", strlen("Connected"));
    }
    break;
    case WStype_TEXT:
      if (strstr((const char *)payload, TYPE[0]) != 0) {
        irsendSamsung.sendSAMSUNG(getHexCode((char*) payload), 32);
      }
      else if (strstr((const char *)payload, TYPE[1]) != 0) {
        irsendTeac.sendNEC(getHexCode((char*) payload), 32);
      }
      else if (strstr((const char *)payload, TYPE[2]) != 0) {
        // send to 433kHz sender
      }
      else {
        Serial.println("Unknown type");
      }
      // send data to all connected clients
      webSocket.broadcastTXT(payload, length);
      break;
      
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);

      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
      
    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}

int measureDistance(){ 
 long t = 0;
 digitalWrite(SEND_TRIGGER, LOW); 
 delayMicroseconds(3);
 noInterrupts();
 digitalWrite(SEND_TRIGGER, HIGH);
 delayMicroseconds(10);
 digitalWrite(SEND_TRIGGER, LOW); 
 t = pulseIn(RESEIVE_ECHO, HIGH);
 interrupts(); 
 return ((t/2) / 29.1); 
}

void setup() {
  Serial.begin(115200);
  irsendTeac.begin();
  irsendSamsung.begin();
  pinMode(SEND_TRIGGER, OUTPUT);
  pinMode(RESEIVE_ECHO, INPUT);
  digitalWrite(SEND_TRIGGER, LOW);
  Serial.println("PINS INITIALIZED");

  for(uint8_t t = 5; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP(ssid, password);
  
  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("");

  if (mdns.begin("com22-101", WiFi.localIP())) {
    mdns.addService("ws", "tcp", 81);
    Serial.println("MDNS responder started");
  } else {
    Serial.println("MDNS.begin failed");
  }

  //webSocket.setAuthorization(socket_user,socket_password);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  Serial.println("WebSocket started");
  Serial.println(WiFi.localIP());
}

void loop() {
  webSocket.loop();
  /*
  if(calculations > -1){
    calculations++;
  }
  if(calculations == 30) {
      if(actions > 0){
        Serial.print("AKTIONS ");
        Serial.print(actions);
        Serial.println();
        if(actions == 1){
           irsendSamsung.sendSAMSUNG(0xE0E040BF, 32);
        }else if (actions == 2) {
          irsendTeac.sendNEC(0xA156E916, 32);
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
   delay(50);*/

}