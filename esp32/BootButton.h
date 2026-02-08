#pragma once

#include <stdint.h>

class BootButton {
 public:
  BootButton(uint8_t pin, bool activeLow, uint32_t debounceMs);

  void begin();
  bool update();

 private:
  bool readRaw() const;

  uint8_t pin;
  bool activeLow;
  uint32_t debounceMs;
  bool lastStableState;
  bool lastReadState;
  uint32_t lastChangeMs;
};
