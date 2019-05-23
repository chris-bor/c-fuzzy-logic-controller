/*
 * dac_functions.h
 *
 *  Created on: Jan 27, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#ifndef DAC_FUNCTIONS_H
#define DAC_FUNCTIONS_H

#include "my_types.h"

/* Function to initialization DAC */
void dacInit( void );


/* Function to setting value to DAC */
void setDACValue( const AccuType value );

#endif /* DAC_FUNCTIONS_H */
