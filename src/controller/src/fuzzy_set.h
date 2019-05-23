/*
 * fuzzy_set.h
 *
 *  Created on: Jan 19, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#ifndef FUZZY_SET_H
#define FUZZY_SET_H

#include "my_types.h"

/* Fuzzy set structure */
typedef struct FuzzySet
{
		AccuType			domain[2];					// Low and high edges
		AccuType			mf[MF_LENG];				// Array of membership function
}FuzzySet;

#endif /* FUZZY_SET_H */
