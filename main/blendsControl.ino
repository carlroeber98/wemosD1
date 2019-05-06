
String upTime = String("08:00");
String downTime = String("22:00");

char* upCode;
char* downCode;

void setBlindsTime(char* payload) {
  strtok(payload, delimiter);
  char* action = strtok(NULL, delimiter);
  char* actionTime = strtok(NULL, delimiter);
  if (strstr(action, "UP")) {
    upTime = (String) actionTime;
  }
  else if (strstr(action, "DOWN")) {
    downTime = (String) actionTime;
  }
  Serial.println(upTime);
  Serial.println(downTime);
}

void checkBlindsTime(String currentTime) {
  Serial.println(currentTime);
  if (currentTime.equals(upTime)) {
    Serial.println("UP");
  }
  else if (currentTime.equals(downTime)) {
    Serial.println("DOWN");
  }
}

String getBlindsTime() {
  return String("{UP:" + upTime + "," + "DOWN:" + downTime + "}");
}
