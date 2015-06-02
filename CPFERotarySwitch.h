#include "arduino/Arduino.h"
#include "CPFEAVRAnalog.h"

void rotarySwitchResultHandler(uint16_t result, void *info);

class CPFERotarySwitch {
   static uint8_t positionCount;
   static int8_t currentPositionRetrieval;
   static bool pendingRetrievalRequest;

   static const uint8_t NUM_ROTARYS = 3;
   static const uint8_t RED_ADC = 0;
   static const uint8_t YELLOW_ADC = 1;
   static const uint8_t BLACK_ADC = 2;

   enum class RotarySwitches
      : uint8_t {
         RED, YELLOW, BLACK
   };

   static CPFEAVRAnalog analogPins[NUM_ROTARYS];
   static uint8_t positions[NUM_ROTARYS];

public:

   static void init(uint8_t positionCount) {
      CPFERotarySwitch::positionCount = positionCount;
      pendingRetrievalRequest = false;
      currentPositionRetrieval = -1;
   }

   static void requestUpdatedPositions() {
      if (currentPositionRetrieval < 0) {
         currentPositionRetrieval = 0;
         pendingRetrievalRequest = true;
      }
   }

   static uint8_t getPosition(RotarySwitches rotarySwitch) {
      return positions[(uint8_t) rotarySwitch];
   }

   static void runTasks() {
      if (pendingRetrievalRequest) {
         while (!CPFEAVRAnalog::adcAvailable())
            ;

         analogPins[currentPositionRetrieval].startConversion(
            &rotarySwitchResultHandler, nullptr);
      }
   }

   static void resultHandler(uint16_t result, void *info) {
      if (currentPositionRetrieval > 0) {
         positions[currentPositionRetrieval] = result / (1024 / positionCount);

         if (currentPositionRetrieval < NUM_ROTARYS - 1) {
            ++currentPositionRetrieval;
            pendingRetrievalRequest = true;
         }
      }
   }
};
