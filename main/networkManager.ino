#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>

const char* ssid = "ruessel";
const char* password = "Elinet123";

MDNSResponder mdns;
ESP8266WiFiMulti WiFiMulti;

void connectToNetwork() {
  WiFiMulti.addAP(ssid, password);

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("");

  if (mdns.begin("com22-101", WiFi.localIP())) {
    mdns.addService("https", "tcp", 443);
    Serial.println("MDNS responder started");
  } else {
    Serial.println("MDNS.begin failed");
  }
  Serial.println(WiFi.localIP());
  Serial.println("Port: 443");
}
