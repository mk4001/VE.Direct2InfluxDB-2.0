void ReadVEData() {
  while (SERIAL_PORT.available()) {
    myve.rxData(SERIAL_PORT.read());
  }
  yield();
}

void EverySecond() {
  static unsigned long prev_millis;

  if (millis() - prev_millis > 1000) {
    PrintData();
    prev_millis = millis();
  }
}

void PrintData() {
  for (int i = 0; i < myve.veEnd; i++) {
    //Serial.print(myve.veName[i]);
    //Serial.print(" = ");
    //Serial.print(myve.veValue[i]);
    //Serial.print(" | ");
    parseSerialData(myve.veName[i], myve.veValue[i]);
  }
}


// hex frame callback function
void HexCallback(const char* buffer, int size, void* data) {
  char tmp[100];
  memcpy(tmp, buffer, size * sizeof(char));
  tmp[size] = 0;
  Serial.print("received hex frame: ");
  Serial.println(tmp);
}

// log helper
void LogHelper(const char* module, const char* error) {
  Serial.print(module);
  Serial.print(":");
  Serial.println(error);
}