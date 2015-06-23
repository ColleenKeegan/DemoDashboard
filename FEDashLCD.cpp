/*
 * FEDashLCD.cpp


 *
 *  Created on: May 21, 2015
 *      Author: thomaswillson
 */
#include "FEDashLCD.h"

FT801IMPL_SPI FEDashLCD::LCD;

volatile FEDashLCD::DASHBOARD_DATA FEDashLCD::DashboardData;
volatile uint8_t FEDashLCD::CAN_OVFCount = 0;
volatile uint8_t FEDashLCD::TransOVFCount = 0;
volatile uint8_t FEDashLCD::RotaryDispOVFCount = 0;
volatile FEDashLCD::DashCAN1 FEDashLCD::dashCAN1;
volatile FEDashLCD::DashCAN2 FEDashLCD::dashCAN2;
volatile FEDashLCD::DashCAN3 FEDashLCD::dashCAN3;
volatile FEDashLCD::DashCAN4 FEDashLCD::dashCAN4;
volatile FEDashLCD::WarningCANMessage FEDashLCD::warningCAN;

SIGNAL(TIMER2_OVF_vect) {
   FEDashLCD::TIMER2_OVF_INT();
}

