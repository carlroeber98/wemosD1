#include <ArduinoOTA.h>
#include <String.h>

const bool SERIAL_DEBUG_OUTPUT = true;
const bool WEBSOCKET_AUTH = true;

char delimiter[] = ";";

void setup() {

  Serial.begin(115200);
  
  initializePins();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("PINS INITIALIZED");

  for (uint8_t t = 5; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    //Serial.flush();
    delay(1000);
  }
  
  connectToNetwork();

  initNTPService();
  initHttpsServer();
}

void loop() {
  //websocketLoop();
  serverLoop();
  handleNtpResponse();
  updateTime();
}
