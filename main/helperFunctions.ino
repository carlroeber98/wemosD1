uint32_t getHexCodeFromChar(char* code) {
  uint32_t val = 0;
  while (*code) {
    // get current character then increment
    char byte = *code++;
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
    char* copy = (char*) malloc( 2 * sizeof(int));
    strcpy(copy, timeAsString);
    timeAsString = "";
    strcat(timeAsString, "0");
    strcat(timeAsString, copy);
    free(copy);
    return timeAsString;
  }
  return timeAsString;
}
