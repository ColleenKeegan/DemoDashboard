/*
 * CPFECANLib.cpp
 *
 *  Created on: May 21, 2015
 *      Author: thomaswillson
 */

#include "arduino/Arduino.h"
#include "CPFECANLib.h"

ISR(CANIT_vect,ISR_BLOCK) {
   cli();
   if (CANGIT & (1 << RXOK)) {
      CPFECANLib::rxInt();
   }
   sei();
}

CPFECANLib::CAN_MSG_RX CPFECANLib::rxIntFunc;

