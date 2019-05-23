/*
 * fuzzy_operations.c
 *
 *  Created on: Jun 19, 2011
 *      Author: Krzysztof I. Borowiec
 */

#include "fuzzy_set.h"

void clearFuzzySet( struct FuzzySet *fuzzy_set )
{
	int i;	/* loop counter */
	fuzzy_set->id = 0;
	fuzzy_set->domain[0] = 0;
	fuzzy_set->domain[1] = 0;
	for( i=0; i<MF_LENG; i++ ) fuzzy_set->mf[i] = 0.0;
}
