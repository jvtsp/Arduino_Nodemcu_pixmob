#include <Arduino.h>
#include <IRremote.hpp>

// SET THIS TO THE DATA PIN USED FOR THE IR TRANSMITTER
const uint16_t kIrLed = 3;

String incomingString = "";
void setup() {
  Serial.begin(115200);
  //Serial.setTimeout(0.5);
  IrSender.begin(kIrLed);
  IrSender.enableIROut(38);
}

void loop() {
  while (!Serial.available());
  incomingString = Serial.readStringUntil('['); // read the incoming byte:
  incomingString = Serial.readStringUntil(']'); // read the incoming byte:

  int newLength = incomingString.toInt();
  uint16_t newRawData[newLength] = {};
  String newVals = Serial.readStringUntil(',');
  for (int i = 0; i < newVals.length(); i++ ) {
    int intVal = newVals.substring(i, i + 1).toInt() * 700;
    newRawData[i] = intVal;
  }
  IrSender.sendRaw(newRawData, newLength, 38);  // Send a raw data capture at 38kHz.
  delay(3);
}
