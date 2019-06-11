#include <WiFiUdp.h>

WiFiUDP UDP;

IPAddress timeServerIP;
const char* NTPServerName = "times.tubit.tu-berlin.de";
const int NTP_PACKET_SIZE = 48;

byte NTPBuffer[NTP_PACKET_SIZE];

unsigned long intervalNTP = 3600000;
unsigned long prevNTP = 0;
unsigned long lastNTPResponse;

uint32_t timeUNIX = 0;

void initNTPService() {
  Serial.println("Starting UDP");
  UDP.begin(123);
  Serial.print("Local port: ");
  Serial.println(UDP.localPort());
  Serial.println();

  if (!WiFi.hostByName(NTPServerName, timeServerIP)) {
    Serial.println("DNS lookup failed. Rebooting.");
    Serial.flush();
    ESP.reset();
  }

  Serial.print("Time server IP:\t");
  Serial.println(timeServerIP);

  Serial.println("\r\nSending NTP request..");
  sendNTPpacket(timeServerIP);
  prevNTP = millis();
}

void handleNtpResponse() {
  unsigned long currentMillis = millis();
  if (currentMillis - prevNTP > intervalNTP) {
    prevNTP = currentMillis;
    Serial.println("Sending NTP request");
    sendNTPpacket(timeServerIP);
  }

  uint32_t time = getTime();                   // Check if an NTP response has arrived and get the (UNIX) time
  if (time) {                                  // If a new timestamp has been received
    timeUNIX = time;
    Serial.print("NTP response: ");
    Serial.println(timeUNIX);
    lastNTPResponse = currentMillis;
  } else if ((currentMillis - lastNTPResponse) > 10800000) {
    Serial.println("More than 3 hour since last NTP response. Rebooting.");
    Serial.flush();
    ESP.reset();
  }
}

void sendNTPpacket(IPAddress& address) {
  memset(NTPBuffer, 0, NTP_PACKET_SIZE);  // set all bytes in the buffer to 0
  // Initialize values needed to form NTP request
  NTPBuffer[0] = 0b11100011;   // LI, Version, Mode
  // send a packet requesting a timestamp:
  UDP.beginPacket(address, 123);
  UDP.write(NTPBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}


uint32_t getTime() {
  if (UDP.parsePacket() == 0) { // If there's no response (yet)
    return 0;
  }
  UDP.read(NTPBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  // Combine the 4 timestamp bytes into one 32-bit number
  uint32_t NTPTime = (NTPBuffer[40] << 24) | (NTPBuffer[41] << 16) | (NTPBuffer[42] << 8) | NTPBuffer[43];
  // Convert NTP time to a UNIX timestamp:
  // Unix time starts on Jan 1 1970. That's 2208988800 seconds in NTP time:
  const uint32_t seventyYears = 2208988800UL;
  // subtract seventy years:
  uint32_t UNIXTime = NTPTime - seventyYears + 7200;
  return UNIXTime;
}

unsigned long getLastNTPResponseTime() {
  return lastNTPResponse;
}

uint32_t getTimeUNIX(){
  return timeUNIX;
}
