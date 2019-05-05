#include <ArduinoOTA.h>
#include <WebSocketsServer.h>

const bool SERIAL_DEBUG_OUTPUT = true;
const bool WEBSOCKET_AUTH = false;

char delimiter[] = ";";

void setup() {

  if (SERIAL_DEBUG_OUTPUT) {
    Serial.begin(115200);
  }

  initSender();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("PINS INITIALIZED");

  for (uint8_t t = 5; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    //Serial.flush();
    delay(1000);
  }
  connectToNetwork();

  initNTP();
  startWebSocket();
}

void loop() {
  websocketLoop();
  calculateTime();
}
