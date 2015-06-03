/*
 * CPFERotarySwitch.cpp
 *
 *  Created on: Jun 1, 2015
 *      Author: thomaswillson
 */

#include <stdint.h>
#include "CPFERotarySwitch.h"

uint8_t CPFERotarySwitch::positionCount;
int8_t CPFERotarySwitch::currentPositionRetrieval;
bool CPFERotarySwitch::pendingRetrievalRequest;
CPFEAVRAnalog CPFERotarySwitch::analogPins[NUM_ROTARYS];
uint8_t CPFERotarySwitch::positions[NUM_ROTARYS];

void rotarySwitchResultHandler(uint16_t result, void *info) {
   CPFERotarySwitch::resultHandler(result, info);
}
