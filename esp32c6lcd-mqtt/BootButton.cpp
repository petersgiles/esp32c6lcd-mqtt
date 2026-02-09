#include "BootButton.h"

#include <Arduino.h>

BootButton::BootButton(uint8_t pin, bool activeLow, uint32_t debounceMs)
    : pin(pin),
      activeLow(activeLow),
      debounceMs(debounceMs),
      lastStableState(false),
      lastReadState(false),
      lastChangeMs(0) {}

void BootButton::begin() {
  pinMode(pin, activeLow ? INPUT_PULLUP : INPUT);
  lastStableState = readRaw();
  lastReadState = lastStableState;
  lastChangeMs = millis();
}

bool BootButton::update() {
  bool currentRead = readRaw();
  uint32_t now = millis();

  if (currentRead != lastReadState) {
    lastReadState = currentRead;
    lastChangeMs = now;
  }

  if ((now - lastChangeMs) >= debounceMs && currentRead != lastStableState) {
    lastStableState = currentRead;
    return lastStableState;
  }

  return false;
}

bool BootButton::readRaw() const {
  int level = digitalRead(pin);
  return activeLow ? (level == LOW) : (level == HIGH);
}
