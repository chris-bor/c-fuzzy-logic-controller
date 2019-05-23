/*
 * fuzzy_variable.h
 *
 *  Created on: Jan 6, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#ifndef FUZZY_VARIABLE_H
#define FUZZY_VARIABLE_H

#include "my_types.h"

/* Fuzzy linguistic variable structure */
typedef struct FuzzyVariable
{
		AccuType			univ_of_disc[2];			// Low and high edges
		FuzzySet			fuzzy_sets[ MAX_SETS ];		// Associated fuzzy sets
}FuzzyVariable;

#endif /* FUZZY_VARIABLE_H */
