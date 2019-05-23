/*
 * fuzzy_set.h
 *
 *  Created on: Jun 19, 2011
 *      Author: Krzysztof I. Borowiec
 */

#ifndef FUZZY_SET_H
#define FUZZY_SET_H

#include "my_types.h"

/* Fuzzy set structure */
typedef struct FuzzySet
{
		IntType				id;							// Identifier name
		AccuType			domain[2];					// Low and high edges
		AccuType			mf[MF_LENG];				// Array of membership function
		struct FuzzySet		*next;						// Pointer to next set
};

#endif /* FUZZY_SET_H */
