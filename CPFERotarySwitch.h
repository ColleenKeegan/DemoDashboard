#include "arduino/Arduino.h"

class CPFERotarySwitch {
   uint8_t pin;
   uint8_t positionCount;

public:
   CPFERotarySwitch(uint8_t pin, uint8_t positionCount) {
      this->pin = pin;
      this->positionCount = positionCount;
   }

   uint8_t getPosition() {
      int adcResult = analogRead(pin);

      return adcResult / (1024 / positionCount);
   }
};
