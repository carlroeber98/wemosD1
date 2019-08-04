#include <ir_Midea.h>
#include <ir_Gree.h>
#include <IRtimer.h>
#include <ir_Kelvinator.h>
#include <IRsend.h>
#include <ir_Daikin.h>
#include <IRremoteESP8266.h>
#include <ir_Trotec.h>
#include <ir_Toshiba.h>
#include <ir_Magiquest.h>
#include <ir_Argo.h>
#include <ir_Haier.h>
#include <IRrecv.h>
#include <ir_LG.h>
#include <IRutils.h>
#include <ir_Fujitsu.h>
#include <ir_Mitsubishi.h>
#include <IRrecv.h>
#include <ArduinoOTA.h>

int RECV_PIN = D7; 
 
IRrecv irrecv(RECV_PIN);
decode_results results;
 
void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  Serial.printf("Started");
}
 
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(resultToSourceCode(&results));
    serialPrintUint64(results.value, HEX);
    Serial.println("");  // Blank line between entries
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
