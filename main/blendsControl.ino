
char* upTime = "08:00";
char* downTime = "22:00";

char* upCode;
char* downCode;

void setBlindsTime(char* payload) {
  strtok(payload, delimiter);
  char* action = strtok(NULL, delimiter);
  char* actionTime = strtok(NULL, delimiter);
  if (strstr(action, "UP")) {
    upTime = actionTime;
  }
  else if (strstr(action, "DOWN")) {
    downTime = actionTime;
  }
  Serial.println(upTime);
  Serial.println(downTime);
}

void checkBlindsTime(char* currentTime) {
  if (strstr(currentTime, upTime)) {
    Serial.println("UP");
  }
  else if (strstr(currentTime, downTime)) {
    Serial.println("DOWN");
  }
}

char* getBlindsTime() {
  char* values = (char*) malloc(50  * sizeof(char));
  strcat(values, "{UP:");
  strcat(values, upTime);
  strcat(values, ",");
  strcat(values, "DOWN:");
  strcat(values, downTime);
  strcat(values, "}");
  return values;
}
