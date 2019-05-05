uint32_t getHexCodeFromPayload(char* payload) {
  strtok(payload, delimiter);
  char* hex = strtok(NULL, delimiter);
  uint32_t val = 0;
  while (*hex) {
    // get current character then increment
    char byte = *hex++;
    // transform hex character to the 4bit equivalent number, using the ascii table indexes
    if (byte >= '0' && byte <= '9') byte = byte - '0';
    else if (byte >= 'a' && byte <= 'f') byte = byte - 'a' + 10;
    else if (byte >= 'A' && byte <= 'F') byte = byte - 'A' + 10;
    // shift 4 to make space for new digit, and add the 4 bits of the new digit
    val = (val << 4) | (byte & 0xF);
  }
  return val;
}

inline int getSeconds(uint32_t UNIXTime) {
  return UNIXTime % 60;
}

inline int getMinutes(uint32_t UNIXTime) {
  return UNIXTime / 60 % 60;
}

inline int getHours(uint32_t UNIXTime) {
  return UNIXTime / 3600 % 24;
}

char* normalize(char* timeAsString) {
  if (strlen(timeAsString) < 2) {
    return strcat("0", timeAsString);
  }
  return timeAsString;
}
