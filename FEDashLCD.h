#include <stdio.h>
#include <stdint.h>
#include <FT_VM801P43_50.h>

#include "CPFECANLib.h"
#include "float16.hpp"
#include "arduino/Arduino.h"
#include "CPFERotarySwitch.h"

static const char PROGMEM WarningMessage_ControllerTemperature[] = "CONTROLLER WARM: %.2fC";
static const char PROGMEM WarningMessage_MotorTemperature[] = "MOTOR WARM: %.2fC";
static const char PROGMEM WarningMessage_BatteryTemperature[] = "BATTERY WARM: %.2fC";
static const char PROGMEM WarningMessage_BatteryLowVoltage[] = "PACK LOW VOLTAGE: %.2fV";
static const char PROGMEM WarningMessage_LVBattery[] = "GLV LOW VOLTAGE: %.2fV";
static const char PROGMEM WarningMessage_sbRIOTemperature[] = "sbRIO WARM: %.0fC";
static const char PROGMEM WarningMessage_Precharge[] = "HV PRECHARGING: %.0fV";
static const char PROGMEM WarningMessage_Invalid[] = "INVALID MSG";
static const char PROGMEM WarningMessage_BSPD[] = "BSPD ERROR";
static const char PROGMEM WarningMessage_IMD[] = "IMD ERROR";
static const char PROGMEM WarningMessage_RemotePit[] = "Remote Request";
static const char PROGMEM WarningMessage_EStop[] = "An E-Stop is Pressed";
static const char PROGMEM WarningMessage_NoCellComms[] = "NO CELL COMMS";
static const char PROGMEM WarningMessage_MCOverSpeed[] = "MC Over Speed";
static const char PROGMEM WarningMessage_MCOverCurrent[] = "MC Over Current";
static const char PROGMEM WarningMessage_MCOverVoltage[] = "MC Over Voltage";
static const char PROGMEM WarningMessage_MCOverTemp[] = "MC Inverter Overtemp";
static const char PROGMEM WarningMessage_MCDirectionCommand[] = "MC Direction Error";
static const char PROGMEM WarningMessage_MCInverterResponseTimeout[] = "MC Inverter Timeout";
static const char PROGMEM WarningMessage_MCDesatFault[] = "MC Desat Fault";
static const char PROGMEM WarningMessage_MCHardwareOverCurrentFault[] = "MC HW Over-Current";
static const char PROGMEM WarningMessage_MCUnderVoltage[] = "MC Under Voltage";
static const char PROGMEM WarningMessage_MCCommandMessageLost[] = "MC Command Message Lost";
static const char PROGMEM WarningMessage_MCMotorOverTemp[] = "Motor Over Temp";
static const char PROGMEM WarningMessage_MCModAOverTemp[] = "Module A Overtemp";
static const char PROGMEM WarningMessage_MCModBOverTemp[] = "Module B Overtemp";
static const char PROGMEM WarningMessage_MCModCOverTemp[] = "Module C Overtemp";
static const char PROGMEM WarningMessage_MCPCBOverTemp[] = "PCB Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv1OverTemp[] = "Gate DRV 1 Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv2OverTemp[] = "Gate DRV 2 Overtemp";
static const char PROGMEM WarningMessage_MCGateDrv3OverTemp[] = "Gate DRV 3 Overtemp";
static const char PROGMEM WarningMessage_MCCurrentSensorFault[] = "MC Curr Sensor Fault";
static const char PROGMEM WarningMessage_MCResolverNotConnected[] = "Resolver Not Connected";
static const char PROGMEM WarningMessage_ShutdownLatchTripped[] = "Shutdown Latch Tripped";
static const char PROGMEM WarningMessage_UnknownBMS[] = "Unknown BMS Error";
static const char PROGMEM WarningMessage_RemoteEmergency[] = "REMOTE EMERGENCY SHUTDOWN";

static const uint8_t ROTARY_MAX_DESC_LENGTH = 32;

static const char PROGMEM RotaryRed1[] = "TRQ-1";
static const char PROGMEM RotaryRed2[] = "TRQ-2";
static const char PROGMEM RotaryRed3[] = "TRQ-3";
static const char PROGMEM RotaryRed4[] = "TRQ-4";
static const char PROGMEM RotaryRed5[] = "TRQ-5";
static const char PROGMEM RotaryRed6[] = "TRQ-6";
static const char PROGMEM RotaryRed7[] = "TRQ-7";
static const char PROGMEM RotaryRed8[] = "TRQ-8";
static const char PROGMEM RotaryRedUnused[] = "Invalid";

PGM_P const RotaryRedStringTable[] PROGMEM =
{
   RotaryRed1, RotaryRed2, RotaryRed3, RotaryRed4, RotaryRed5, RotaryRed6, RotaryRed7, RotaryRed8, RotaryRedUnused, RotaryRedUnused, RotaryRedUnused, RotaryRedUnused
};

static const char PROGMEM RotaryYellow1[] = "REGEN-1";
static const char PROGMEM RotaryYellow2[] = "REGEN-2";
static const char PROGMEM RotaryYellow3[] = "REGEN-3";
static const char PROGMEM RotaryYellow4[] = "REGEN-4";
static const char PROGMEM RotaryYellow5[] = "REGEN-5";
static const char PROGMEM RotaryYellow6[] = "REGEN-6";
static const char PROGMEM RotaryYellow7[] = "REGEN-7";
static const char PROGMEM RotaryYellow8[] = "REGEN-8";
static const char PROGMEM RotaryYellowUnused[] = "Invalid";

PGM_P const RotaryYellowStringTable[] = { RotaryYellow1, RotaryYellow2,
   RotaryYellow3, RotaryYellow4, RotaryYellow5, RotaryYellow6, RotaryYellow7,
   RotaryYellow8, RotaryYellowUnused, RotaryYellowUnused, RotaryYellowUnused,
   RotaryYellowUnused };

static const char PROGMEM RotaryBlack1[] = "Default";
static const char PROGMEM RotaryBlack2[] = "Pit";
static const char PROGMEM RotaryBlack3[] = "BrakeBal";
static const char PROGMEM RotaryBlack4[] = "Throttle";
static const char PROGMEM RotaryBlack5[] = "Code";
static const char PROGMEM RotaryBlack6[] = "MULTI-6";
static const char PROGMEM RotaryBlack7[] = "MULTI-7";
static const char PROGMEM RotaryBlack8[] = "MULTI-8";
static const char PROGMEM RotaryBlackUnused[] = "Invalid";

PGM_P const RotaryBlackStringTable[] PROGMEM =
{
   RotaryBlack1, RotaryBlack2, RotaryBlack3, RotaryBlack4, RotaryBlack5, RotaryBlack6, RotaryBlack7, RotaryBlack8, RotaryBlackUnused, RotaryBlackUnused,RotaryBlackUnused,RotaryBlackUnused
};

static const char PROGMEM MCState0[] = "SC.Initial";
static const char PROGMEM MCState1[] = "SC.History";
static const char PROGMEM MCState2[] = "SC.ShallowHistory";
static const char PROGMEM MCState3[] = "SC.Terminal";
static const char PROGMEM MCState4[] = "CurrentDrop";
static const char PROGMEM MCState5[] = "Enabled";
static const char PROGMEM MCState6[] = "Disabled";

PGM_P const MCStateStringTable[] PROGMEM =
{
   MCState0, MCState1, MCState2, MCState3, MCState4, MCState5, MCState6
};

static const char PROGMEM ShutdownState0[] = "SC.Initial";
static const char PROGMEM ShutdownState1[] = "SC.History";
static const char PROGMEM ShutdownState2[] = "SC.ShallowHistory";
static const char PROGMEM ShutdownState3[] = "AIR's & Coolant On";
static const char PROGMEM ShutdownState4[] = "AIR's On";
static const char PROGMEM ShutdownState5[] = "Precharge";
static const char PROGMEM ShutdownState6[] = "TSMS Off";

PGM_P const ShutdownStateStringTable[] PROGMEM =
{
   ShutdownState0, ShutdownState1, ShutdownState2, ShutdownState3, ShutdownState4, ShutdownState5, ShutdownState6
};

static const uint8_t PROGMEM CPRacingLogo[] = {120, 156, 205, 212, 49, 110, 219,
   48, 20, 6, 224, 71, 209, 40, 129, 34, 0, 57, 116, 208, 100, 122, 200, 160,
   209, 163, 6, 193, 118, 111, 144, 35, 248, 8, 25, 53, 8, 49, 157, 14, 25,
   123, 133, 30, 33, 71, 96, 209, 33, 107, 143, 64, 195, 67, 87, 6, 94, 88,
   128, 21, 251, 72, 73, 137, 156, 184, 206, 210, 33, 15, 2, 52, 124, 18,
   223, 251, 41, 66, 33, 156, 45, 125, 158, 223, 191, 59, 24, 213, 53, 200,
   96, 151, 65, 49, 211, 80, 191, 81, 4, 221, 30, 59, 11, 70, 182, 192, 76,
   69, 252, 13, 188, 242, 53, 208, 160, 165, 7, 166, 75, 226, 155, 115, 94,
   131, 175, 59, 15, 91, 83, 129, 244, 155, 189, 105, 46, 214, 11, 26, 148,
   244, 44, 160, 187, 154, 199, 249, 30, 3, 181, 209, 195, 193, 52, 185, 137,
   206, 29, 71, 87, 131, 155, 232, 211, 202, 30, 244, 98, 30, 93, 244, 94,
   187, 228, 7, 243, 1, 125, 145, 124, 141, 222, 230, 157, 235, 222, 247,
   102, 130, 94, 69, 151, 107, 3, 180, 45, 123, 47, 109, 30, 231, 219, 235,
   11, 55, 184, 54, 192, 124, 201, 6, 23, 157, 231, 117, 3, 203, 63, 232, 92,
   69, 111, 162, 171, 218, 60, 185, 116, 53, 44, 127, 239, 162, 107, 224, 14,
   146, 91, 147, 15, 235, 47, 113, 39, 42, 178, 251, 165, 57, 232, 107, 137,
   110, 209, 31, 209, 101, 231, 243, 102, 112, 130, 110, 129, 90, 137, 110,
   69, 239, 102, 93, 53, 232, 135, 222, 205, 224, 96, 167, 201, 219, 85, 181,
   209, 13, 9, 15, 109, 116, 190, 111, 123, 39, 118, 74, 209, 245, 232, 243,
   16, 93, 243, 31, 232, 177, 191, 251, 135, 63, 180, 196, 2, 79, 14, 39,
   156, 221, 181, 196, 129, 132, 147, 78, 149, 99, 95, 3, 241, 32, 201, 206,
   227, 99, 244, 63, 156, 223, 150, 105, 47, 183, 118, 193, 180, 147, 84,
   193, 81, 209, 206, 157, 252, 242, 134, 83, 123, 147, 110, 160, 87, 87, 89,
   166, 24, 228, 185, 152, 151, 57, 27, 57, 79, 14, 201, 39, 80, 124, 18,
   179, 98, 246, 236, 166, 119, 165, 127, 102, 217, 45, 186, 64, 47, 162,
   143, 91, 50, 208, 234, 30, 61, 131, 162, 20, 98, 158, 60, 141, 222, 39, 8,
   176, 210, 247, 25, 220, 2, 20, 151, 113, 129, 39, 239, 19, 4, 242, 121,
   246, 45, 163, 217, 71, 40, 10, 113, 5, 99, 79, 9, 2, 116, 221, 38, 32, 0,
   102, 32, 162, 15, 209, 227, 132, 225, 56, 62, 68, 31, 162, 163, 127, 111,
   95, 56, 127, 7, 255, 39, 188, 224, 100, 197, 169, 159, 223, 119, 175, 28,
   35, 109, 94, 174, 63, 246, 134, 111, 79, 247, 55, 221, 89, 198, 125, 59,
   51, 159, 199, 246, 203, 232, 127, 1, 157, 31, 141, 8};

static constexpr uint8_t WarningCANMessageID = 0xF1;
static constexpr uint8_t DashCAN1ID = 0xF0;
static constexpr uint8_t DashCAN2ID = 0xF2;
static constexpr uint8_t DashCAN3ID = 0xF3;
static constexpr uint8_t DashCAN4ID = 0xF4;
static constexpr uint8_t DashCANInputID = 0xF5;

static const CPFECANLib::MSG PROGMEM DashCAN1MSG = {
   {  DashCAN1ID}, 8, 0, 0, 0
};

static const CPFECANLib::MSG PROGMEM DashCAN1Mask = {
   {  0xFFF}, 8, 1, 1, 0
};

static const CPFECANLib::MSG PROGMEM DashCAN2MSG = {
   {  DashCAN2ID}, 8, 0, 0, 0
};

static const CPFECANLib::MSG PROGMEM DashCAN2Mask = {
   {  0xFFF}, 8, 1, 1, 0
};

static const CPFECANLib::MSG PROGMEM DashCAN3MSG = {
   {  DashCAN3ID}, 8, 0, 0, 0
};

static const CPFECANLib::MSG PROGMEM DashCAN3Mask = {
   {  0xFFF}, 8, 1, 1, 0
};

static const CPFECANLib::MSG PROGMEM DashCAN4MSG = {
   {  DashCAN4ID}, 8, 0, 0, 0
};

static const CPFECANLib::MSG PROGMEM DashCAN4Mask = {
   {  0xFFF}, 8, 1, 1, 0
};

static const CPFECANLib::MSG PROGMEM DashCANWarningMSG = {
   {  WarningCANMessageID}, 8, 0, 0, 0
};

static const CPFECANLib::MSG PROGMEM DashCANWarningMask = {
   {  0xFFF}, 8, 1, 1, 0
};

class FEDashLCD {
public:
   enum class DashPages
      : uint8_t {
         Primary, Warning, WaitingForCAN, InPits, BrakeBal, Throttle, Code
   };

   enum class WarningMessage
      : uint8_t {
         ControllerTemperature,
      MotorTemperature,
      BatteryTemperature,
      BatteryLowVoltage,
      LVBattery,
      sbRIOTemperature,
      Precharge,
      BSPD,
      IMD,
      RemoteReturnToPits,
      EStopInterrupted,
      NoCellComms,
      MCOverSpeed,
      MCOverCurrent,
      MCOverVoltage,
      MCOverTemp,
      MCDirectionCommand,
      MCInverterResponseTimeout,
      MCDesatFault,
      MCHardwareOverCurrentFault,
      MCUnderVoltage,
      MCCommandMessageLost,
      MCMotorOverTemp,
      MCModAOverTemp,
      MCModBOverTem,
      MCModCOverTemp,
      MCPCBOverTemp,
      MCGateDrv1OverTemp,
      MCGateDrv2OverTemp,
      MCDateDrv3OverTemp,
      MCCurrentSensorFault,
      MCResolverNotConnected,
      ShutdownLatchTripped,
      UnknownBMS,
      RemoteEmergency
   };

   enum class WarningSeverity
      : uint8_t {
         Okay, ShortWarning, LongWarning, ReturnToPits, Error, Danger
   };

   enum class MCControlState
      : uint8_t {
         SC_Initial,
      SC_History,
      SC_ShallowHistory,
      SC_Terminal,
      CurrentDrop,
      Enabled,
      Disabled
   };

   enum class ShutdownState
      : uint8_t {
         SC_Initial,
      SC_History,
      SC_ShallowHistory,
      SC_Terminal,
      AIR_Coolant_On,
      AIR_On,
      Precharge,
      TSMS_Off
   };

   typedef struct DASHBOARD_DATA {
      union {
         uint8_t NDashPage;
         DashPages DashPage;
      } DashPage;

      float TMotor;
      float TControllerMax;
      float VHVBattery;
      float IHVBattery;
      float nMotor;

      float TCellMax;
      float TCellMean;
      float VMinCell;
      float VMaxCell;
      float VMeanCell;

      float pBrakeF;
      float pBrakeR;
      float rBrakeBal;

      float VBattery;

      MCControlState mcControlState;
      ShutdownState shutdownState;

      WarningMessage warningMessage;
      float warningValue;
      WarningSeverity warningSeverity;

      uint8_t previousRotaryPositions[CPFERotarySwitch::NUM_ROTARYS];
      bool rotaryOverride;
      CPFERotarySwitch::RotarySwitches rotaryToShow;

   } DASHBOARD_DATA;

   static volatile DASHBOARD_DATA DashboardData;

   inline static void TIMER2_OVF_INT() { //Timer 2 OVF INT for CAN Timeout
      ++CAN_OVFCount;
      ++TransOVFCount;
      ++RotaryDispOVFCount;

      if (CAN_OVFCount > CAN_TIMER_OVF_COUNT_MAX) {
         CAN_OVFCount = 0;

         FEDashLCD::DashboardData.DashPage.DashPage =
            FEDashLCD::DashPages::WaitingForCAN;
      }

      if (TransOVFCount >= TRANS_TIMER_OVF_COUNT_MAX) {
         TransOVFCount = 0;

         //transmitDashboardInfo();
      }

      if (RotaryDispOVFCount >= ROTARY_DISP_TIMER_OVF_COUNT_MAX) {
         RotaryDispOVFCount = 0;
         DashboardData.rotaryOverride = false;
      }
   }

   static void init() {
      Serial.begin(57600);

      if (bootupConfigure()) {
         Serial.println("Init Failed");
      }

      waitingForCAN(false); //Faster boot.
      LCD.DisplayOn();

      uploadLogoToController();

      CPFECANLib::init(CPFECANLib::CAN_BAUDRATE::B1M, &CAN_RX_Int);
      initCAN_RX();

      //Display Waiting For CAN Screen
      DashboardData.DashPage.DashPage = DashPages::WaitingForCAN;

      //Init CAN timeout timer (Timer 2)
      TCCR2A = (1 << CS22) | (1 << CS21) | (1 << CS20); //Normal mode, prescale 1/1024 for 63 OVF/sec
      TIMSK2 = (1 << TOIE2); //Timer 2 OVF Interrupt

      //Init Rotary Switches
      CPFERotarySwitch::init(12);

      //Init Digital Inputs for Buttons
      DDRC &= ~((1 << 4) - 1); //Set direction
      PORTC |= ((1 << 4) - 1); //Enable pullups
   }

   static void updateDisplay() {
      //CPFERotarySwitch::runTasks();

      FEDashLCD::DashboardData.DashPage.NDashPage = dashCAN1.NDashPage;
      float16::toFloat32(&DashboardData.nMotor, swap(dashCAN1.nMotor));
      float16::toFloat32(&DashboardData.VHVBattery, swap(dashCAN1.VHVBattery));
      float16::toFloat32(&DashboardData.IHVBattery, swap(dashCAN1.IHVBattery));

      float16::toFloat32(&DashboardData.TMotor, swap(dashCAN2.TMotor));
      float16::toFloat32(&DashboardData.TControllerMax,
         swap(dashCAN2.TControllerMax));
      float16::toFloat32(&DashboardData.TCellMax, swap(dashCAN2.TCellMax));
      float16::toFloat32(&DashboardData.TCellMean, swap(dashCAN2.TCellMean));

      float16::toFloat32(&DashboardData.VBattery, swap(dashCAN3.VBattery));
      float16::toFloat32(&DashboardData.VMinCell, swap(dashCAN3.VMinCell));
      float16::toFloat32(&DashboardData.pBrakeF, swap(dashCAN3.pBrakeF));
      float16::toFloat32(&DashboardData.pBrakeR, swap(dashCAN3.pBrakeR));

      float16::toFloat32(&DashboardData.VMaxCell, swap(dashCAN4.VMaxCell));
      float16::toFloat32(&DashboardData.VMeanCell, swap(dashCAN4.VMeanCell));
      float16::toFloat32(&DashboardData.rBrakeBal, swap(dashCAN4.rBrakeBal));

      FEDashLCD::DashboardData.warningMessage = warningCAN.warningMessage;
      FEDashLCD::DashboardData.warningSeverity = warningCAN.warningSeverity;
      FEDashLCD::DashboardData.warningValue = swap(warningCAN.associatedValue);

      if (DashboardData.rotaryOverride) {
         rotaryOverride();
      } else {
         switch (DashboardData.DashPage.DashPage) {
         case DashPages::WaitingForCAN:
            waitingForCAN();
            break;
         case DashPages::Primary:
            primary();
            break;
         case DashPages::Warning:
            warning();
            break;
         case DashPages::InPits:
            inPits();
            break;
//         case DashPages::BrakeBal:
//            brakeBalPage();
//            break;
//         case DashPages::Throttle:
//            throttlePage();
//            break;
//         case DashPages::Code:
//            codePage();
//            break;
         default:
            warning();
            break;
         }
      }

   }

private:
   static const uint8_t CAN_TIMER_OVF_COUNT_MAX = 60;
   static volatile uint8_t CAN_OVFCount;

   static const uint8_t TRANS_TIMER_OVF_COUNT_MAX = 12;
   static volatile uint8_t TransOVFCount;

   static const uint8_t ROTARY_DISP_TIMER_OVF_COUNT_MAX = 60;
   static volatile uint8_t RotaryDispOVFCount;

   static FT801IMPL_SPI LCD;

   static constexpr uint8_t DashCAN1Mob = 0;
   static constexpr uint8_t DashCAN2Mob = 1;
   static constexpr uint8_t DashCAN3Mob = 2;
   static constexpr uint8_t DashCAN4Mob = 3;
   static constexpr uint8_t WarningCANMob = 4;
   static constexpr uint8_t DashCANInputMob = 5;

   typedef struct WarningCANMessage { //0xF1
      WarningSeverity warningSeverity;
      float associatedValue;
      WarningMessage warningMessage;
      uint8_t notOK;
   } WarningCANMessage;

   typedef struct DashCAN1 { //0xF0
      uint8_t NDashPage;
      uint16_t nMotor;
      uint16_t VHVBattery;
      uint16_t IHVBattery;
   } DashCAN1;
   typedef struct DashCAN2 { //0xF2
      uint16_t TMotor;
      uint16_t TControllerMax;
      uint16_t TCellMax;
      uint16_t TCellMean;
   } DashCAN2;
   typedef struct DashCAN3 { //0xF3
      uint16_t VBattery;
      uint16_t VMinCell;
      uint16_t pBrakeF;
      uint16_t pBrakeR;
   } DashCAN3;
   typedef struct DashCAN4 { //0xF4
      uint16_t VMaxCell;
      uint16_t VMeanCell;
      uint16_t rBrakeBal;
      MCControlState mcControlState;
      ShutdownState shutdownState;
   } DashCAN4;

   typedef struct DashCANInput { //0xF5
      uint8_t ButtonsArray;
      uint8_t RedRotary;
      uint8_t YellowRotary;
      uint8_t BlackRotary;
   } DashCANInput;

   static volatile DashCAN1 dashCAN1;
   static volatile DashCAN2 dashCAN2;
   static volatile DashCAN3 dashCAN3;
   static volatile DashCAN4 dashCAN4;
   static volatile WarningCANMessage warningCAN;

   static void initCAN_RX() {
      RX_DashCAN1(false);
      RX_DashCAN2(false);
      RX_DashCAN3(false);
      RX_DashCAN4(false);
      RX_WarningCAN(false);
   }

   static void transmitDashboardInfo() {
      uint8_t dataBuffer[8];
      DashCANInput *data = (DashCANInput*) dataBuffer;
      CPFECANLib::MSG msg;

      msg.identifier.standard = 0xF5;
      msg.data = (uint8_t *) &dataBuffer;
      msg.dlc = 8;
      msg.ide = 0;
      msg.rtr = 0;

      data->ButtonsArray = PINC;
      data->BlackRotary = CPFERotarySwitch::getPosition(
         CPFERotarySwitch::RotarySwitches::BLACK);
      data->YellowRotary = CPFERotarySwitch::getPosition(
         CPFERotarySwitch::RotarySwitches::YELLOW);
      data->RedRotary = CPFERotarySwitch::getPosition(
         CPFERotarySwitch::RotarySwitches::RED);

      if (data->BlackRotary
         != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::BLACK]) {
         DashboardData.rotaryOverride = true;
         DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::BLACK;
         RotaryDispOVFCount = 0;
      } else if (data->YellowRotary
         != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::YELLOW]) {
         DashboardData.rotaryOverride = true;
         DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::YELLOW;
         RotaryDispOVFCount = 0;
      } else if (data->RedRotary
         != DashboardData.previousRotaryPositions[(uint8_t) CPFERotarySwitch::RotarySwitches::RED]) {
         DashboardData.rotaryOverride = true;
         DashboardData.rotaryToShow = CPFERotarySwitch::RotarySwitches::RED;
         RotaryDispOVFCount = 0;
      }

      for (int i = 0; i < CPFERotarySwitch::NUM_ROTARYS; ++i) {
         DashboardData.previousRotaryPositions[i] =
            CPFERotarySwitch::getPosition((CPFERotarySwitch::RotarySwitches) i);
      }

      CPFECANLib::sendMsgUsingMOB(DashCANInputMob, &msg);

      //Request Updated Positions for next iteration
      CPFERotarySwitch::requestUpdatedPositions();
   }

   static void RX_DashCAN1(bool interruptMode) {
      CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN1Mob, &DashCAN1MSG,
         &DashCAN1Mask, interruptMode);
   }

   static void RX_DashCAN2(bool interruptMode) {
      CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN2Mob, &DashCAN2MSG,
         &DashCAN2Mask, interruptMode);
   }

   static void RX_DashCAN3(bool interruptMode) {
      CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN3Mob, &DashCAN3MSG,
         &DashCAN3Mask, interruptMode);
   }

   static void RX_DashCAN4(bool interruptMode) {
      CPFECANLib::enableMOBAsRX_PROGMEM(DashCAN4Mob, &DashCAN4MSG,
         &DashCAN4Mask, interruptMode);
   }

   static void RX_WarningCAN(bool interruptMode) {
      CPFECANLib::enableMOBAsRX_PROGMEM(WarningCANMob, &DashCANWarningMSG,
         &DashCANWarningMask, interruptMode);
   }

   static int16_t bootupConfigure() {
      uint32_t chipid = 0;
      Serial.println(LCD.Init(FT_DISPLAY_RESOLUTION, 0, false), HEX); //configure the display to the WQVGA

      chipid = LCD.Read32(FT_ROM_CHIPID);

      /* Identify the chip */
      if (FT801_CHIPID != chipid) {
         Serial.print("Error in chip id read ");
         Serial.println(chipid, HEX);
         return 1;
      }

      return 0;
   }

   static void uploadLogoToController() {
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

   static void warning() {
      bool displayBoxes;
      const char *severityText;
      uint32_t color = 0x00000;

      switch (DashboardData.warningSeverity) {
      case WarningSeverity::ShortWarning:
      case WarningSeverity::LongWarning:
         displayBoxes = millis() % 500 > 250;
         severityText = "WARNING!";
         color = 0xFFFF00;
         break;
      case WarningSeverity::Error:
         displayBoxes = millis() % 500 > 250;
         severityText = "ERROR!";
         color = 0xFF0000;
         break;
      case WarningSeverity::Danger:
         displayBoxes = true;
         severityText = "DANGER!";
         if (millis() % 600 > 450)
            color = 0xFFFF00;
         else if (millis() % 600 > 300)
            color = 0x00FF00;
         else if (millis() % 600 > 150)
            color = 0x000000;
         else
            color = 0xFF0000;
         break;
      case WarningSeverity::ReturnToPits:
         displayBoxes = true;
         severityText = "Return to Pits";
         color = 0xFFFF00;
         break;
      default:
         displayBoxes = true;
         severityText = "Unknown Severity";
      }

      LCD.DLStart();

      LCD.ColorRGB(color);
      LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 4, 31,
         FT_OPT_CENTER, severityText);
      LCD.PrintTextFlash(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT - 100, 29,
         FT_OPT_CENTER, warningMessageToString(DashboardData.warningMessage),
         DashboardData.warningValue);

      if (displayBoxes) {
         LCD.ClearColorRGB(color);
         LCD.ScissorSize(75, FT_DISPLAYHEIGHT);
         LCD.ScissorXY(0, 0);
         LCD.Clear(1, 1, 1);
         LCD.ScissorXY(FT_DISPLAYWIDTH - 75, 0);
         LCD.Clear(1, 1, 1);
      }

      LCD.DLEnd();
      LCD.Finish();
   }

   static void primary() {
      LCD.DLStart();

      LCD.ColorRGB(0xFF, 0x00, 0x00);
      LCD.PrintText(5, 0, 29, 0, "Battery Info:");
      LCD.ColorRGB(0xFF, 0xFF, 0xFF);
      LCD.PrintText(5, 30, 28, 0, "VMinCell: %.2f", DashboardData.VMinCell);
      LCD.PrintText(5, 55, 28, 0, "VMaxCell: %.2f", DashboardData.VMaxCell);
      LCD.PrintText(5, 80, 28, 0, "VMeanCell: %.2f", DashboardData.VMeanCell);
      LCD.PrintText(5, 105, 28, 0, "VBattery: %.2f", DashboardData.VBattery);

      LCD.ColorRGB(0xFF, 0x00, 0x00);
      LCD.PrintText(5, 135, 29, 0, "Temp Info:");
      LCD.ColorRGB(0xFF, 0xFF, 0xFF);
      LCD.PrintText(5, 165, 28, 0, "TMotor: %.2f", DashboardData.TMotor);
      LCD.PrintText(5, 190, 28, 0, "TMCMax: %.2f",
         DashboardData.TControllerMax);
      LCD.PrintText(5, 215, 28, 0, "TCellMax: %.2f", DashboardData.TCellMax);
      LCD.PrintText(5, 240, 28, 0, "TCellMean: %.2f", DashboardData.TCellMean);

      LCD.ColorRGB(0xFFFFFF);
      LCD.Cmd_FGColor(0x0000FF);
      LCD.Cmd_BGColor(0x0000FF);
      LCD.Cmd_Slider(FT_DISPLAYWIDTH - 150, 20, 20, 180, 0,
         (uint16_t) 4500 + (uint16_t) DashboardData.nMotor, 4500);
      LCD.ColorRGB(0xFF0000);
      LCD.PrintText(FT_DISPLAYWIDTH - 150, 240, 31, FT_OPT_CENTER, "SP");

      LCD.ColorRGB(0xFFFFFF);
      LCD.Cmd_FGColor(0xFF0000);
      LCD.Cmd_BGColor(0xFF0000);
      LCD.Cmd_Slider(FT_DISPLAYWIDTH - 30, 20, 20, 180, 0,
         100 - (uint16_t) DashboardData.TMotor, 100);
      LCD.ColorRGB(0xFF0000);
      LCD.PrintText(FT_DISPLAYWIDTH - 20, 240, 31, FT_OPT_CENTER, "M");

      LCD.ColorRGB(0xFFFFFF);
      LCD.Cmd_FGColor(0xFF0000);
      LCD.Cmd_BGColor(0xFF0000);
      LCD.Cmd_Slider(FT_DISPLAYWIDTH - 90, 20, 20, 180, 0,
         100 - (uint16_t) DashboardData.TControllerMax, 100);
      LCD.ColorRGB(0x0000FF);
      LCD.PrintText(FT_DISPLAYWIDTH - 80, 240, 31, FT_OPT_CENTER, "C");

      LCD.DLEnd();
      LCD.Finish();
   }

   static void waitingForCAN() {
      waitingForCAN(true);
   }

   static void waitingForCAN(bool withLogo) {
      LCD.DLStart();
      const char Display_string[] = "Formula Electric 2015";

      LCD.ColorRGB(0xFF, 0xFF, 0xFF);
      LCD.Cmd_Text(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 8, 29, FT_OPT_CENTER,
         Display_string);
      LCD.ColorRGB(0xFF, 0x00, 0x00);
      LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 29,
         FT_OPT_CENTER, "WAITING FOR CAN DATA...");

      if (withLogo) {
         LCD.ColorRGB(0xFF, 0xFF, 0xFF);
         LCD.Begin(FT_BITMAPS);
         LCD.Vertex2ii(FT_DISPLAYWIDTH / 4, 223, 0, 0);
         LCD.End();
      }

      LCD.DLEnd();
      LCD.Finish();
   }

   static void inPits() {
      LCD.DLStart();

      LCD.DLEnd();
      LCD.Finish();
   }

   static void rotaryOverride() {
      uint32_t color = 0x000000;
      uint8_t position = CPFERotarySwitch::getPosition(
         DashboardData.rotaryToShow);
      char positionDescription[ROTARY_MAX_DESC_LENGTH];

      switch (DashboardData.rotaryToShow) {
      case CPFERotarySwitch::RotarySwitches::BLACK:
         color = 0x000000;
         strncpy_P(positionDescription,
            (PGM_P) pgm_read_word(&(RotaryBlackStringTable[position])),
            ROTARY_MAX_DESC_LENGTH);
         break;
      case CPFERotarySwitch::RotarySwitches::YELLOW:
         color = 0xFFFF00;
         strncpy_P(positionDescription,
            (PGM_P) pgm_read_word(&(RotaryYellowStringTable[position])),
            ROTARY_MAX_DESC_LENGTH);
         break;
      case CPFERotarySwitch::RotarySwitches::RED:
         color = 0xFF0000;
         strncpy_P(positionDescription,
            (PGM_P) pgm_read_word(&(RotaryRedStringTable[position])),
            ROTARY_MAX_DESC_LENGTH);
         break;
      }

      LCD.DLStart();
      LCD.ClearColorRGB(color);
      LCD.Clear(1, 1, 1);

      LCD.ColorRGB(~color);
      LCD.PrintText(FT_DISPLAYWIDTH / 2, FT_DISPLAYHEIGHT / 2, 31,
         FT_OPT_CENTER, "%d", position);
      LCD.PrintText(25, 25, 31, 0, "%s", positionDescription);

      LCD.DLEnd();
      LCD.Finish();
   }

   void brakeBalPage() {

   }

   void codePage() {

   }

   void throttlePage() {

   }

   static const char * PROGMEM warningMessageToString(WarningMessage warning) {
      switch (warning) {
         case WarningMessage::BatteryLowVoltage:
         return WarningMessage_BatteryLowVoltage;
         case WarningMessage::BatteryTemperature:
         return WarningMessage_BatteryTemperature;
         case WarningMessage::ControllerTemperature:
         return WarningMessage_ControllerTemperature;
         case WarningMessage::LVBattery:
         return WarningMessage_LVBattery;
         case WarningMessage::MotorTemperature:
         return WarningMessage_MotorTemperature;
         case WarningMessage::Precharge:
         return WarningMessage_Precharge;
         case WarningMessage::sbRIOTemperature:
         return WarningMessage_sbRIOTemperature;
         case WarningMessage::IMD:
         return WarningMessage_IMD;
         case WarningMessage::BSPD:
         return WarningMessage_BSPD;
         case WarningMessage::RemoteReturnToPits:
         return WarningMessage_RemotePit;
         case WarningMessage::EStopInterrupted:
         return WarningMessage_EStop;
         case WarningMessage::NoCellComms:
         return WarningMessage_NoCellComms;
         case WarningMessage::MCOverSpeed:
         return WarningMessage_MCOverSpeed;
         case WarningMessage::MCOverCurrent:
         return WarningMessage_MCOverCurrent;
         case WarningMessage::MCOverVoltage:
         return WarningMessage_MCOverVoltage;
         case WarningMessage::MCOverTemp:
         return WarningMessage_MCOverTemp;
         case WarningMessage::MCDirectionCommand:
         return WarningMessage_MCDirectionCommand;
         case WarningMessage::MCInverterResponseTimeout:
         return WarningMessage_MCInverterResponseTimeout;
         case WarningMessage::MCDesatFault:
         return WarningMessage_MCDesatFault;
         case WarningMessage::MCHardwareOverCurrentFault:
         return WarningMessage_MCHardwareOverCurrentFault;
         case WarningMessage::MCUnderVoltage:
         return WarningMessage_MCUnderVoltage;
         case WarningMessage::MCCommandMessageLost:
         return WarningMessage_MCCommandMessageLost;
         case WarningMessage::MCMotorOverTemp:
         return WarningMessage_MCMotorOverTemp;
         case WarningMessage::MCModAOverTemp:
         return WarningMessage_MCModAOverTemp;
         case WarningMessage::MCModBOverTem:
         return WarningMessage_MCModBOverTemp;
         case WarningMessage::MCModCOverTemp:
         return WarningMessage_MCModCOverTemp;
         case WarningMessage::MCPCBOverTemp:
         return WarningMessage_MCPCBOverTemp;
         case WarningMessage::MCGateDrv1OverTemp:
         return WarningMessage_MCGateDrv1OverTemp;
         case WarningMessage::MCGateDrv2OverTemp:
         return WarningMessage_MCGateDrv2OverTemp;
         case WarningMessage::MCDateDrv3OverTemp:
         return WarningMessage_MCGateDrv3OverTemp;
         case WarningMessage::MCCurrentSensorFault:
         return WarningMessage_MCCurrentSensorFault;
         case WarningMessage::MCResolverNotConnected:
         return WarningMessage_MCResolverNotConnected;
         case WarningMessage::ShutdownLatchTripped:
         return WarningMessage_ShutdownLatchTripped;
         case WarningMessage::UnknownBMS:
         return WarningMessage_UnknownBMS;
         case WarningMessage::RemoteEmergency:
         return WarningMessage_RemoteEmergency;
      }
      return WarningMessage_Invalid;
   }

   static void CAN_RX_Int(CPFECANLib::MSG *msg, uint8_t mobNum) {
      resetTimeoutTimer();

      switch (msg->identifier.standard) {
      case DashCAN1ID:
         memcpy((void *) &dashCAN1, msg->data, sizeof(dashCAN1));
         RX_DashCAN1(true);
         break;
      case DashCAN2ID:
         memcpy((void *) &dashCAN2, msg->data, sizeof(dashCAN2));
         RX_DashCAN2(true);
         break;
      case DashCAN3ID:
         memcpy((void *) &dashCAN3, msg->data, sizeof(dashCAN3));
         RX_DashCAN3(true);
         break;
      case DashCAN4ID:
         memcpy((void *) &dashCAN4, msg->data, sizeof(dashCAN4));
         RX_DashCAN4(true);
         break;
      case WarningCANMessageID:
         memcpy((void *) &warningCAN, msg->data, sizeof(warningCAN));
         RX_WarningCAN(true);
         break;
      }

   }
   static float swap(float d) {
      float a;
      unsigned char *dst = (unsigned char *) &a;
      unsigned char *src = (unsigned char *) &d;
      dst[0] = src[3];
      dst[1] = src[2];
      dst[2] = src[1];
      dst[3] = src[0];
      return a;
   }

   static uint32_t swap(uint32_t d) {
      uint32_t a;
      unsigned char *dst = (unsigned char *) &a;
      unsigned char *src = (unsigned char *) &d;
      dst[0] = src[3];
      dst[1] = src[2];
      dst[2] = src[1];
      dst[3] = src[0];
      return a;
   }

   static uint16_t swap(uint16_t d) {
      uint16_t a;
      unsigned char *dst = (unsigned char *) &a;
      unsigned char *src = (unsigned char *) &d;
      dst[0] = src[1];
      dst[1] = src[0];
      return a;
   }

   static void resetTimeoutTimer() {
      CAN_OVFCount = 0;
   }
};
