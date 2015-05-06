/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */
#include <stdint.h>
#include <stdio.h>

#include "FT_VM801P43_50.h"
#include "arduino/Arduino.h"
#include <avr/pgmspace.h>

#include "FEDashLCD.h"

FEDashLCD Display;

int main() {
   init();
   Display.init();

   while (1) {
      Display.updateDisplay();
   }

}
