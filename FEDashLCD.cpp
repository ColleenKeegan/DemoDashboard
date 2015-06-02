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

SIGNAL(TIMER2_OVF_vect) { //CAN Message Timeout
   FEDashLCD::TIMER2_OVF_INT();
}

SIGNAL(TIMER1_OVF_vect) { //Transmission Timing
   FEDashLCD::TIMER1_OVF_INT();
}

