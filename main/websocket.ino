#include <ESP8266WebServer.h>

const char* socket_user = "angular";
const char* socket_password = "FLpRduPT9gmfsCW6tHh2WDuTa2JqgJ";

int connectedClients = 0;

WebSocketsServer webSocket = WebSocketsServer(81);

void startWebSocket() {
  if (WEBSOCKET_AUTH) {
    webSocket.setAuthorization(socket_user, socket_password);
  }
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  if (SERIAL_DEBUG_OUTPUT) {
    Serial.println("WebSocket started");
  }
  char* test = getBlindsTime();
  Serial.println(test);
  free(test);
}

void websocketLoop() {
  webSocket.loop();
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      //Serial.printf("[%u] Disconnected!\r\n", num);
      connectedClients--;
      Serial.print("Connected clients:");
      Serial.print(connectedClients);
      Serial.println();
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Client from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        //webSocket.sendTXT(num, getBlindsTime());
      }
      connectedClients++;
      Serial.print("Connected clients:");
      Serial.print(connectedClients);
      Serial.println();

      break;
    case WStype_TEXT:
      if (strstr((const char *) payload, "blinds") != 0) {
        setBlindsTime((char *) payload);
      }
      else {
        sendHexCode((const char *) payload);
        // send callback to all connected clients
        webSocket.broadcastTXT(payload, length);
      }
      break;
    case WStype_BIN:
      //Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);

      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;

    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }
}
