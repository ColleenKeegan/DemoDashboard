#include <AVRLibrary/arduino/Arduino.h>
#include <AVRLibrary/CPFEAVRAnalog.h>

void rotarySwitchResultHandler(uint16_t result, void *info);

class CPFERotarySwitch {
public:
	static const uint8_t NUM_ROTARYS = 3;

	enum class RotarySwitches
		: uint8_t {
			YELLOW, RED, BLACK
	};
private:

	static uint8_t positionCount;
	static int8_t currentPositionRetrieval;
	static bool pendingRetrievalRequest;

	static const uint8_t RED_ADC = 1;
	static const uint8_t YELLOW_ADC = 0;
	static const uint8_t BLACK_ADC = 2;

	static CPFEAVRAnalog analogPins[NUM_ROTARYS];
	static uint8_t positions[NUM_ROTARYS];

public:

	static void init(uint8_t positionCount) {
		CPFERotarySwitch::positionCount = positionCount;
		pendingRetrievalRequest = false;
		currentPositionRetrieval = -1;

		for (int i = 0; i < NUM_ROTARYS; ++i) {
			analogPins[i].analogPinNumber = i;
		}
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

			analogPins[currentPositionRetrieval].startConversion(&rotarySwitchResultHandler, nullptr);
		}
	}

	static void resultHandler(uint16_t result, void *info) {
		Serial.printf("%d, %d\n", result, currentPositionRetrieval);

		if (currentPositionRetrieval >= 0) {
			positions[currentPositionRetrieval] = (uint8_t)(positionCount - (float)result / (1024.0 / (float)positionCount));

			if (positions[currentPositionRetrieval] > 11) {
				positions[currentPositionRetrieval] = 11;
			}

			Serial.printf("%d\n", positions[currentPositionRetrieval]);

			if (currentPositionRetrieval < NUM_ROTARYS - 1) {
				++currentPositionRetrieval;
				pendingRetrievalRequest = true;
			} else {
				currentPositionRetrieval = 0;
				pendingRetrievalRequest = true;
			}
		}
	}
};
