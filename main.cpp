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

FT801IMPL_SPI FTImpl;

static const uint8_t CPRacingLogo[] PROGMEM = {120, 156, 205, 212, 49, 110, 219, 48,
   20, 6, 224, 71, 209, 40, 129, 34, 0, 57, 116, 208, 100, 122, 200, 160, 209,
   163, 6, 193, 118, 111, 144, 35, 248, 8, 25, 53, 8, 49, 157, 14, 25, 123, 133,
   30, 33, 71, 96, 209, 33, 107, 143, 64, 195, 67, 87, 6, 94, 88, 128, 21, 251,
   72, 73, 137, 156, 184, 206, 210, 33, 15, 2, 52, 124, 18, 223, 251, 41, 66,
   33, 156, 45, 125, 158, 223, 191, 59, 24, 213, 53, 200, 96, 151, 65, 49, 211,
   80, 191, 81, 4, 221, 30, 59, 11, 70, 182, 192, 76, 69, 252, 13, 188, 242, 53,
   208, 160, 165, 7, 166, 75, 226, 155, 115, 94, 131, 175, 59, 15, 91, 83, 129,
   244, 155, 189, 105, 46, 214, 11, 26, 148, 244, 44, 160, 187, 154, 199, 249,
   30, 3, 181, 209, 195, 193, 52, 185, 137, 206, 29, 71, 87, 131, 155, 232, 211,
   202, 30, 244, 98, 30, 93, 244, 94, 187, 228, 7, 243, 1, 125, 145, 124, 141,
   222, 230, 157, 235, 222, 247, 102, 130, 94, 69, 151, 107, 3, 180, 45, 123,
   47, 109, 30, 231, 219, 235, 11, 55, 184, 54, 192, 124, 201, 6, 23, 157, 231,
   117, 3, 203, 63, 232, 92, 69, 111, 162, 171, 218, 60, 185, 116, 53, 44, 127,
   239, 162, 107, 224, 14, 146, 91, 147, 15, 235, 47, 113, 39, 42, 178, 251,
   165, 57, 232, 107, 137, 110, 209, 31, 209, 101, 231, 243, 102, 112, 130, 110,
   129, 90, 137, 110, 69, 239, 102, 93, 53, 232, 135, 222, 205, 224, 96, 167,
   201, 219, 85, 181, 209, 13, 9, 15, 109, 116, 190, 111, 123, 39, 118, 74, 209,
   245, 232, 243, 16, 93, 243, 31, 232, 177, 191, 251, 135, 63, 180, 196, 2, 79,
   14, 39, 156, 221, 181, 196, 129, 132, 147, 78, 149, 99, 95, 3, 241, 32, 201,
   206, 227, 99, 244, 63, 156, 223, 150, 105, 47, 183, 118, 193, 180, 147, 84,
   193, 81, 209, 206, 157, 252, 242, 134, 83, 123, 147, 110, 160, 87, 87, 89,
   166, 24, 228, 185, 152, 151, 57, 27, 57, 79, 14, 201, 39, 80, 124, 18, 179,
   98, 246, 236, 166, 119, 165, 127, 102, 217, 45, 186, 64, 47, 162, 143, 91,
   50, 208, 234, 30, 61, 131, 162, 20, 98, 158, 60, 141, 222, 39, 8, 176, 210,
   247, 25, 220, 2, 20, 151, 113, 129, 39, 239, 19, 4, 242, 121, 246, 45, 163,
   217, 71, 40, 10, 113, 5, 99, 79, 9, 2, 116, 221, 38, 32, 0, 102, 32, 162, 15,
   209, 227, 132, 225, 56, 62, 68, 31, 162, 163, 127, 111, 95, 56, 127, 7, 255,
   39, 188, 224, 100, 197, 169, 159, 223, 119, 175, 28, 35, 109, 94, 174, 63,
   246, 134, 111, 79, 247, 55, 221, 89, 198, 125, 59, 51, 159, 199, 246, 203,
   232, 127, 1, 157, 31, 141, 8,};

/* Api to bootup FT801, verify FT801 hardware and configure display/audio pins */
/* Returns 0 in case of success and 1 in case of failure */
int16_t BootupConfigure() {
   uint32_t chipid = 0;
   Serial.println(FTImpl.Init(FT_DISPLAY_RESOLUTION), HEX); //configure the display to the WQVGA

   chipid = FTImpl.Read32(FT_ROM_CHIPID);

   /* Identify the chip */
   if (FT801_CHIPID != chipid) {
      Serial.print("Error in chip id read ");
      Serial.println(chipid, HEX);
      return 1;
   }

   return 0;
}

/* API to display Hello World string on the screen */
void StartupText() {
   /* Change the below string for experimentation */
   const char Display_string[] = "Formula Electric 2015";

   FTImpl.ColorRGB(0xFF, 0xFF, 0xFF);
   FTImpl.Cmd_Text(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 8, 29, FT_OPT_CENTER,
      Display_string);
   FTImpl.ColorRGB(0xFF, 0x00, 0x00);
   FTImpl.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 29,
      FT_OPT_CENTER, "WAITING FOR CAN DATA...");
}

void DisplayLogo() {
   FTImpl.ColorRGB(0xFF, 0xFF, 0xFF);
   FTImpl.Begin(FT_BITMAPS);
   FTImpl.Vertex2ii(FT_DISPLAYWIDTH / 4, 223, 0, 0);
   FTImpl.End();
}

void UploadLogoToController() {
   FTImpl.Cmd_Inflate(0);
   FTImpl.WriteCmdfromflash(CPRacingLogo, sizeof(CPRacingLogo));
   FTImpl.Finish();
   FTImpl.DLStart();
   FTImpl.BitmapHandle(0);
   FTImpl.BitmapSource(0);
   FTImpl.BitmapLayout(FT_L1, 32, 49);
   FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 250, 49);
   FTImpl.DLEnd();
   FTImpl.Finish();
}

/* bootup the module and display "Hello World" on screen */
void setup() {
   /* Initialize serial print related functionality */
   Serial.begin(57600);

   if (BootupConfigure()) {
      Serial.println("Init Failed");
   }

   UploadLogoToController();

   //Display Waiting For CAN Screen
   FTImpl.DLStart();
   StartupText();
   DisplayLogo();
   FTImpl.DLEnd();
   FTImpl.Finish();
   FTImpl.DisplayOn();
}

int main() {
   init();
   setup();

}
