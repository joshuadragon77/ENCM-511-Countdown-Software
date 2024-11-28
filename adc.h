#ifndef ADC_HEADERS
#define ADC_HEADERS

#include "vscodefix.h"

#include "block.h"

#include <xc.h>

/**
 * Gets the current ADC reading from the PIC24's 10 bit SAR ADC
 * 
 * A value that ranges from 0 to 1024.
 * 
 */
uint16_t getADCReading();


/**
 * Gets the current ADC reading from the PIC24's 10 bit SAR ADC
 * 
 * A value that ranges from 0.0f to 1.0f in decimal percentage form.
 */

float getPercentageReading();

void initADC();

#endif