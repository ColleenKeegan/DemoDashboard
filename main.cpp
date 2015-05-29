/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "arduino/Arduino.h"
#include "FEDashLCD.h"

int main() {
   init();
   FEDashLCD::init();

//   Serial.printf("IE1: %X, IE2: %X\n", CANIE1, CANIE2);
//
//   for (int i = 0; i < 16; i++) {
//      CANPAGE = i << 4;
//      Serial.printf(
//         "[%d]: ST: %X, CONF: %X, ID1: %X, ID2: %X, IDM1: %X, IDM2: %X\n, ", i,
//         CANSTMOB, CANCDMOB, CANIDT1, CANIDT2, CANIDM1, CANIDM2);
//   }

   while (1) {
      FEDashLCD::updateDisplay();
   }
}
