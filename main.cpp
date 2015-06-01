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
   init(); //Arduino Timer Initialization, uses Timer 0 for millis() and other timing functions.

   FEDashLCD::init();

   while (1) {
      FEDashLCD::updateDisplay();
   }
}
