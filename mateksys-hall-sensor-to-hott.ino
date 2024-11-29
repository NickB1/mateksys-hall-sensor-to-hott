#include "src/HoTTServer/HoTTServer.h"

uint8_t currentPin = A0;
uint8_t currentScaleFactor = 50;
uint8_t voltagePin = A1;
uint8_t voltageScaleFactor = 21;

uint8_t hottRxPin = 2;
uint8_t hottTxPin = 3;

HoTTServer hottServer(hottRxPin, hottTxPin);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  hottServer.registerModule(HoTTServerESC);
  hottServer.start();
}

void printDebug(float voltage, float current, float capacity, int loopDurationMs) {
  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.print(" V, current: ");
  Serial.print(current);
  Serial.print(" A, capacity: ");
  Serial.print(capacity);
  Serial.print(" mAh, loopDuration: ");
  Serial.print(loopDurationMs);
  Serial.println(" ms");
}

float analogReadMilliV(uint8_t pin) {
  return analogRead(pin) * 5000.0 / 1024.0;
}

void loop() {
  static bool led = false;
  float voltage = analogReadMilliV(voltagePin) * voltageScaleFactor / 1000.0;
  float current = analogReadMilliV(currentPin) * currentScaleFactor / 1000.0;
  static float capacity = 0;
  static float loopMillisCounter = 0;
  int loopDurationMs = millis() - loopMillisCounter;

  capacity += (current / (3.6 * (1000.0 / loopDurationMs)));
  loopMillisCounter = millis();

  //printDebug(voltage, current, capacity, loopDurationMs);

  hottServer.setVoltage(HOTT_MAIN_VOLTAGE, voltage);
  hottServer.setCurrent(HOTT_MAIN_CURRENT, current);
  hottServer.setCapacity(capacity);

  led = hottServer.processRequest();

  digitalWrite(LED_BUILTIN, led);
}
