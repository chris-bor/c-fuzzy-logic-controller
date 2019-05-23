/*
 * adc_functions.h
 *
 *  Created on: Jan 24, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#ifndef ADC_FUNCTIONS_H
#define ADC_FUNCTIONS_H

#include "my_types.h"

// reference voltage for ADC
#define U_REF 3.3 /* for sample function if value in voltage is wanted */
// maximum value enabled to convert by ADC
#define ADC_MAX 4096 /* for sample function if value in voltage is wanted */

// initialize function for analog - digital converter
void adcInit( void );

/* function for calling sampling by ADC and move result to first
 * argument variable
 * @param input - there is saved sampled value
 */

void sample( AccuType* input );

#endif /* ADC_FUNCTIONS_H */
