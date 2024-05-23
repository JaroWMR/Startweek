#define LED_MATRIX
#define BUTTON_MATRIX
#define GYROCOMPASS

#ifdef LED_MATRIX
#include "testLedMatrix.h"
#endif //LED_MATRIX

#ifdef BUTTON_MATRIX
#include "testButtonMatrix.h"
#endif //BUTTON_MATRIX

#ifdef GYROCOMPASS
#include "testGyroCompass.h"
#endif //GYROCOMPASS