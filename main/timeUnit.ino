#include <time.h>

unsigned long prevTimeCheck = 0;

unsigned long currentTime = 0;

String upTime = String("08:00");
String downTime = String("22:00");

char* upCode;
char* downCode;

void setAction(char* action, char* actionTime) {
  if (strstr(action, "UP")) {
    upTime = (String) actionTime;
  }
  else if (strstr(action, "DOWN")) {
    downTime = (String) actionTime;
  }
  Serial.println(upTime);
  Serial.println(downTime);
}

void updateTime(){
  unsigned long currentMillis = millis();
  
  uint32_t preCurrentTime = getTimeUNIX() + (currentMillis - getLastNTPResponseTime()) / 1000;
  if (preCurrentTime != currentTime) {
    //time_t now = time(NULL);
    //Serial.println(now);
    currentTime = preCurrentTime;
    //checkBlindsTime(); //check every second
  }
}

void checkBlindsTime() {
  if (currentTime == 2222222) {
    char* hoursAsString = (char*) malloc( 2 * sizeof(int));
    char* minutesAsString = (char*) malloc( 2 * sizeof(int));
    char* secondsAsString = (char*) malloc( 2 * sizeof(int));
    sprintf(hoursAsString, "%d", (int) getHours(currentTime));
    sprintf(minutesAsString, "%d", (int) getMinutes(currentTime));
    sprintf(secondsAsString, "%d", (int) getSeconds(currentTime));

    hoursAsString = normalize(hoursAsString);
    minutesAsString = normalize(minutesAsString);
    secondsAsString = normalize(secondsAsString);

    Serial.printf("\rLocal normalized time: %s:%s:%s", hoursAsString, minutesAsString, secondsAsString);
    Serial.println();
    //checkBlindsTime(String(String(hoursAsString) + ":" +  String(minutesAsString)));
    free(hoursAsString);
    free(minutesAsString);
    free(secondsAsString);

    Serial.println(currentTime);
     
    /*if (currentTime.equals(upTime)) {
      Serial.println("UP");
    }
    else if (currentTime.equals(downTime)) {
      Serial.println("DOWN");
    }*/
  }
}

String getActionTime() {
  return String("{UP:" + upTime + "," + "DOWN:" + downTime + "}");
}
