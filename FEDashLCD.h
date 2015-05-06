#include <stdio.h>
#include <stdint.h>
#include "FT_VM801P43_50.h"

class FEDashLCD {

   enum class DashPages {
      Primary, ReturnToPits, Warning, Danger, WaitingForCAN
   };

   typedef struct DASHBOARD_DATA {
      union {
         uint8_t NDashPage;
         DashPages DashPage;
      } DashPage;

      float TMotor;
      float TControllerMax;
      float VBattery;
      float VMinCell;
      float VMaxCell;
      float VMeanCell;
      char sWarning[8];
      char sWarningExpl[64];
   } DASHBOARD_DATA;

   volatile DASHBOARD_DATA DashboardData;

   int16_t BootupConfigure() {
      uint32_t chipid = 0;
      Serial.println(LCD.Init(FT_DISPLAY_RESOLUTION), HEX); //configure the display to the WQVGA

      chipid = LCD.Read32(FT_ROM_CHIPID);

      /* Identify the chip */
      if (FT801_CHIPID != chipid) {
         Serial.print("Error in chip id read ");
         Serial.println(chipid, HEX);
         return 1;
      }

      return 0;
   }

   void init() {
      Serial.begin(57600);

      if (BootupConfigure()) {
         Serial.println("Init Failed");
      }

      UploadLogoToController();
      LCD.DisplayOn();

      //Display Waiting For CAN Screen
      DashboardData.DashPage.DashPage = DashPages::Danger;

      DashboardData.TMotor = 30.5;
      DashboardData.TControllerMax = 50.35;
      DashboardData.VMinCell = 3.69;
      DashboardData.VMaxCell = 4.12;
      DashboardData.VMeanCell = 4.02;
      DashboardData.VBattery = 13.24;

      strncpy((char *) DashboardData.sWarning, "CONTROL", 8);
      strncpy((char *) DashboardData.sWarningExpl, "CAN Message Timeout", 64);
   }

   void updateDisplay() {
      switch (DashboardData.DashPage.DashPage) {
      case DashPages::WaitingForCAN:
         WaitingForCAN();
         break;
      case DashPages::Primary:
         Primary();
         break;
      case DashPages::Warning:
         Warning();
         break;
      case DashPages::Danger:
         Danger();
         break;
      }
   }

   /* API to display Hello World string on the screen */
   void StartupText() {
      /* Change the below string for experimentation */
      const char Display_string[] = "Formula Electric 2015";

      LCD.ColorRGB(0xFF, 0xFF, 0xFF);
      LCD.Cmd_Text(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 8, 29, FT_OPT_CENTER,
         Display_string);
      LCD.ColorRGB(0xFF, 0x00, 0x00);
      LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 29,
         FT_OPT_CENTER, "WAITING FOR CAN DATA...");
   }

   void DisplayLogo() {
      LCD.ColorRGB(0xFF, 0xFF, 0xFF);
      LCD.Begin(FT_BITMAPS);
      LCD.Vertex2ii(FT_DISPLAYWIDTH / 4, 223, 0, 0);
      LCD.End();
   }

   void UploadLogoToController() {
      LCD.Cmd_Inflate(0);
      LCD.WriteCmdfromflash(CPRacingLogo, sizeof(CPRacingLogo));
      LCD.Finish();
      LCD.DLStart();
      LCD.BitmapHandle(0);
      LCD.BitmapSource(0);
      LCD.BitmapLayout(FT_L1, 32, 49);
      LCD.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 250, 49);
      LCD.DLEnd();
      LCD.Finish();
   }

   void Danger() {
      uint32_t boxColor;

      if (millis() % 600 > 450)
         boxColor = 0xFFFF00;
      else if (millis() % 600 > 300)
         boxColor = 0x00FF00;
      else if (millis() % 600 > 150)
         boxColor = 0x000000;
      else
         boxColor = 0xFF0000;

      LCD.DLStart();
      LCD.ColorRGB(boxColor);
      LCD.ClearColorRGB(boxColor);
      LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 31,
         FT_OPT_CENTER, "DANGER!");
      LCD.ScissorSize(75, FT_DISPLAYHEIGHT);
      LCD.ScissorXY(0, 0);
      LCD.Clear(1, 1, 1);
      LCD.ScissorXY(FT_DISPLAYWIDTH - 75, 0);
      LCD.Clear(1, 1, 1);

      LCD.DLEnd();
      LCD.Finish();
   }

   void Warning() {
      bool displayBoxes = millis() % 500 > 250;

      LCD.DLStart();

      LCD.ColorRGB(0xFFFF00);
      LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 4, 31,
         FT_OPT_CENTER, "%s", DashboardData.sWarning);
      LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT - 100, 29,
         FT_OPT_CENTER, "%s", DashboardData.sWarningExpl);

      if (displayBoxes) {
         LCD.ClearColorRGB(0xFFFF00);
         LCD.ScissorSize(75, FT_DISPLAYHEIGHT);
         LCD.ScissorXY(0, 0);
         LCD.Clear(1, 1, 1);
         LCD.ScissorXY(FT_DISPLAYWIDTH - 75, 0);
         LCD.Clear(1, 1, 1);
      }

      LCD.DLEnd();
      LCD.Finish();
   }

   void Primary() {
      LCD.DLStart();

      LCD.ColorRGB(0xFF, 0x00, 0x00);
      LCD.PrintText(5, 0, 29, 0, "Battery Info:");
      LCD.ColorRGB(0xFF, 0xFF, 0xFF);
      LCD.PrintText(5, 30, 28, 0, "VMinCell: %.2f", DashboardData.VMinCell);
      LCD.PrintText(5, 55, 28, 0, "VMaxCell: %.2f", DashboardData.VMaxCell);
      LCD.PrintText(5, 80, 28, 0, "VMeanCell: %.2f", DashboardData.VMeanCell);
      LCD.PrintText(5, 105, 28, 0, "VBattery: %.2f", DashboardData.VBattery);

      LCD.ColorRGB(0xFF, 0x00, 0x00);
      LCD.PrintText(5, 140, 29, 0, "Temp Info:");
      LCD.ColorRGB(0xFF, 0xFF, 0xFF);
      LCD.PrintText(5, 170, 28, 0, "TMotor: %.2f", DashboardData.TMotor);
      LCD.PrintText(5, 195, 28, 0, "TControllerMax: %.2f",
         DashboardData.TControllerMax);

      LCD.ColorRGB(0xFFFFFF);
      LCD.Cmd_FGColor(0xFF0000);
      LCD.Cmd_BGColor(0xFF0000);
      LCD.Cmd_Slider(FT_DISPLAYWIDTH - 50, 20, 20, 180, 0,
         100 - (uint16_t) DashboardData.TMotor, 100);
      LCD.ColorRGB(0xFF0000);
      LCD.PrintText(FT_DISPLAYWIDTH - 40, 240, 31, FT_OPT_CENTER, "M");

      LCD.ColorRGB(0xFFFFFF);
      LCD.Cmd_FGColor(0xFF0000);
      LCD.Cmd_BGColor(0xFF0000);
      LCD.Cmd_Slider(FT_DISPLAYWIDTH - 150, 20, 20, 180, 0,
         100 - (uint16_t) DashboardData.TControllerMax, 100);
      LCD.ColorRGB(0xFF0000);
      LCD.PrintText(FT_DISPLAYWIDTH - 140, 240, 31, FT_OPT_CENTER, "C");

      LCD.DLEnd();
      LCD.Finish();
   }

   void WaitingForCAN() {
      LCD.DLStart();
      StartupText();
      DisplayLogo();
      LCD.DLEnd();
      LCD.Finish();
   }

private:
   FT801IMPL_SPI LCD;

   static const uint8_t CPRacingLogo[] PROGMEM = {120, 156, 205, 212, 49, 110,
      219, 48, 20, 6, 224, 71, 209, 40, 129, 34, 0, 57, 116, 208, 100, 122, 200,
      160, 209, 163, 6, 193, 118, 111, 144, 35, 248, 8, 25, 53, 8, 49, 157, 14, 25,
      123, 133, 30, 33, 71, 96, 209, 33, 107, 143, 64, 195, 67, 87, 6, 94, 88, 128,
      21, 251, 72, 73, 137, 156, 184, 206, 210, 33, 15, 2, 52, 124, 18, 223, 251,
      41, 66, 33, 156, 45, 125, 158, 223, 191, 59, 24, 213, 53, 200, 96, 151, 65,
      49, 211, 80, 191, 81, 4, 221, 30, 59, 11, 70, 182, 192, 76, 69, 252, 13, 188,
      242, 53, 208, 160, 165, 7, 166, 75, 226, 155, 115, 94, 131, 175, 59, 15, 91,
      83, 129, 244, 155, 189, 105, 46, 214, 11, 26, 148, 244, 44, 160, 187, 154,
      199, 249, 30, 3, 181, 209, 195, 193, 52, 185, 137, 206, 29, 71, 87, 131, 155,
      232, 211, 202, 30, 244, 98, 30, 93, 244, 94, 187, 228, 7, 243, 1, 125, 145,
      124, 141, 222, 230, 157, 235, 222, 247, 102, 130, 94, 69, 151, 107, 3, 180,
      45, 123, 47, 109, 30, 231, 219, 235, 11, 55, 184, 54, 192, 124, 201, 6, 23,
      157, 231, 117, 3, 203, 63, 232, 92, 69, 111, 162, 171, 218, 60, 185, 116, 53,
      44, 127, 239, 162, 107, 224, 14, 146, 91, 147, 15, 235, 47, 113, 39, 42, 178,
      251, 165, 57, 232, 107, 137, 110, 209, 31, 209, 101, 231, 243, 102, 112, 130,
      110, 129, 90, 137, 110, 69, 239, 102, 93, 53, 232, 135, 222, 205, 224, 96,
      167, 201, 219, 85, 181, 209, 13, 9, 15, 109, 116, 190, 111, 123, 39, 118, 74,
      209, 245, 232, 243, 16, 93, 243, 31, 232, 177, 191, 251, 135, 63, 180, 196,
      2, 79, 14, 39, 156, 221, 181, 196, 129, 132, 147, 78, 149, 99, 95, 3, 241,
      32, 201, 206, 227, 99, 244, 63, 156, 223, 150, 105, 47, 183, 118, 193, 180,
      147, 84, 193, 81, 209, 206, 157, 252, 242, 134, 83, 123, 147, 110, 160, 87,
      87, 89, 166, 24, 228, 185, 152, 151, 57, 27, 57, 79, 14, 201, 39, 80, 124,
      18, 179, 98, 246, 236, 166, 119, 165, 127, 102, 217, 45, 186, 64, 47, 162,
      143, 91, 50, 208, 234, 30, 61, 131, 162, 20, 98, 158, 60, 141, 222, 39, 8,
      176, 210, 247, 25, 220, 2, 20, 151, 113, 129, 39, 239, 19, 4, 242, 121, 246,
      45, 163, 217, 71, 40, 10, 113, 5, 99, 79, 9, 2, 116, 221, 38, 32, 0, 102, 32,
      162, 15, 209, 227, 132, 225, 56, 62, 68, 31, 162, 163, 127, 111, 95, 56, 127,
      7, 255, 39, 188, 224, 100, 197, 169, 159, 223, 119, 175, 28, 35, 109, 94,
      174, 63, 246, 134, 111, 79, 247, 55, 221, 89, 198, 125, 59, 51, 159, 199,
      246, 203, 232, 127, 1, 157, 31, 141, 8};
};
