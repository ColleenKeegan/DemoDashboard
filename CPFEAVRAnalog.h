/*
 * CPFEAVRAnalog.h
 *
 *  Created on: Jun 1, 2015
 *      Author: thomaswillson
 */

#ifndef CPFEAVRANALOG_H_
#define CPFEAVRANALOG_H_

#include <stdint.h>
#include "arduino/Arduino.h"

class CPFEAVRAnalog {

public:
   typedef void (*ConversionHandler)(uint16_t result, void *info);

   CPFEAVRAnalog(uint8_t analogPinNumber) {
      this->analogPinNumber = analogPinNumber;
      handler = nullptr;
      handlerInfo = nullptr;
      currentConversion = nullptr;
   }
   void startConversion(ConversionHandler *handler, void *handlerInfo) {
      if (adcAvailable()) {
         this->handler = handler;
         this->handlerInfo = handlerInfo;
         currentConversion = this;

         ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
         ADMUX = (1 << REFS0) | (analogPinNumber & 0xF); // Set ADC reference to AVCC
         ADCSRA = (1 << ADEN) | (1 << ADIE); // Enable ADC, ADC Interrupt Enable
         ADCSRA |= (1 << ADSC);  // Start A2D Conversions
      }
   }

   static bool adcAvailable() {
      return currentConversion ? false : true;
   }

   inline static void adcInterruptHandler() {
      if (!adcAvailable()) {
         uint16_t result;
         result = ADCL;
         result |= ADCH << 8;

         currentConversion->handler(result, currentConversion->handlerInfo);
         currentConversion = nullptr;
      }
   }

private:
   uint8_t analogPinNumber;
   ConversionHandler *handler;
   void *handlerInfo;

   static CPFEAVRAnalog *currentConversion;
};

#endif /* CPFEAVRANALOG_H_ */
