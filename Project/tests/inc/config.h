// #define LED_MATRIX
// #define BUTTON_MATRIX
// #define CIRCLE_MATRIX
// #define GENERIC_GPIO
// #define LCD
// #define SEVEN_SEGMENT
#define INTEGRATION

#ifdef INTEGRATION
#include "testIntegration.h"
#endif //INTEGRATION

#ifdef LED_MATRIX
#include "testLedMatrix.h"
#endif //LED_MATRIX

#ifdef BUTTON_MATRIX
#include "testButtonMatrix.h"
#endif //BUTTON_MATRIX

#ifdef CIRCLE_MATRIX
#include "testCircleMatrix.h"
#endif //CIRCLE_MATRIX

#ifdef GENERIC_GPIO
#include "testGenericGpio.h"
#endif //GENERIC_GPIO

#ifdef LCD
#include "testLcd.h"
#endif //LCD

#ifdef SEVEN_SEGMENT
#include "testSevenSegment.h"
#endif //SEVEN_SEGMENT
