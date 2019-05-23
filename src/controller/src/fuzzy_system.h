/*
 * fuzzy_system.h
 *
 *  Created on: Jan 6, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#ifndef FUZZY_SYSTEM_H
#define FUZZY_SYSTEM_H

#include "my_types.h"
#include "fuzzy_variable.h"

/* Fuzzy system structure */
typedef struct FuzzySystem
{
		FuzzyVariable		input_varia[ MAX_INPUT_VARIA ]; /* input fuzzy linguistic variables */
		FuzzyVariable 		outpu_varia; /* output fuzzy linguistic variable */
		FuzzySetsIdType		rule_PD[ MAX_ERROR_SETS ][ MAX_DRIVATE_SETS ]; /* array with rules for PD inference */
		FuzzySetsIdType		rule_I[ MAX_INTEGRAL_SETS ]; /* array with rules for I inference */
}FuzzySystem;

#endif /* FUZZY_SYSTEM_H */
