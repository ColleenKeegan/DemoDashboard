/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */

#include "FT_VM801P43_50.h"
#include "arduino/Arduino.h"

FT801IMPL_SPI FTImpl(FT_CS_PIN, FT_INT_PIN);

/* Api to bootup FT801, verify FT801 hardware and configure display/audio pins */
/* Returns 0 in case of success and 1 in case of failure */
int16_t BootupConfigure() {
   uint32_t chipid = 0;
   FTImpl.Init(FT_DISPLAY_RESOLUTION); //configure the display to the WQVGA

   delay(20); //for safer side
   chipid = FTImpl.Read32(FT_ROM_CHIPID);

   /* Identify the chip */
   if (FT801_CHIPID != chipid) {
      Serial.print("Error in chip id read ");
      Serial.println(chipid, HEX);
      return 1;
   }

   /* Set the Display & audio pins */
   //FTImpl.SetDisplayEnablePin(FT_DISPENABLE_PIN);
   // FTImpl.SetAudioEnablePin(FT_AUDIOENABLE_PIN);
   FTImpl.DisplayOn();
   //FTImpl.AudioOn();
   return 0;
}

/* API to display Hello World string on the screen */
void HelloWorld() {
   /* Change the below string for experimentation */
   const char Display_string[] = "Formula Electric 2015";

   /* Display list to display "Hello World" at the centre of display area */
   FTImpl.DLStart(); //start the display list. Note DLStart and DLEnd are helper apis, Cmd_DLStart() and Display() can also be utilized.
   FTImpl.ColorRGB(0xFF, 0xFF, 0xFF); //set the color of the string to white color
   FTImpl.Cmd_Text(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 29, FT_OPT_CENTER,
      Display_string); //display "Hello World at the center of the screen using inbuilt font handle 29 "
   FTImpl.DLEnd();   //end the display list
   FTImpl.Finish(); //render the display list and wait for the completion of the DL
}

/* bootup the module and display "Hello World" on screen */
void setup() {
   /* Initialize serial print related functionality */
   Serial.begin(9600);

   /* Set the Display Enable pin*/
   Serial.println("--Start Application--");
   if (BootupConfigure()) {
      //error case - do not do any thing
   } else {
      HelloWorld();
   }
   Serial.println("--End Application--");
}

int main() {
   setup();

   delay(2000);

   FTImpl.Clear();

   while (1)
      ;
}
