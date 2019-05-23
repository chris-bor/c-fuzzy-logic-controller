/*
 * fuzzy_operations.c
 *
 *  Created on: Jan 19, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#include "fuzzy_operations.h"
#include "fuzzy_set.h"
#include "math.h"
#include "fuzzy_system.h"
#include "fuzzy_variable.h"
#include "my_types.h"

void setFuzzySet( struct FuzzySet * fuzzy_set,
		const AccuType min_domain, const AccuType max_domain,
		const AccuType mf_value)
{
	int i;	/* loop counter */
	fuzzy_set->domain[0] = min_domain;
	fuzzy_set->domain[1] = max_domain;
	for( i=0; i<MF_LENG; i++ )
		fuzzy_set->mf[i] = mf_value;
}


void calcDomaPoins( AccuType *table_poins, const IntType local_no_sets, const IntType *local_univ_of_disc )
{
	int i;	/* loop counter */
	AccuType term = ( float )( local_univ_of_disc[1] - local_univ_of_disc[ 0 ] ) /
					( float )( local_no_sets+1 ); /* scope of single term */
	for( i=0; i<( local_no_sets+2 ); i++ ) /* calculations of each fuzzy set scope term */
	{
		table_poins[i] = roundf( i*term );
	}
}


void createMfCurve( struct FuzzySet *fuzzy_set, AccuType *poins, AccuType *valus, const IntType no_poins )
{
	int i; /* counter loop */
	int cell_posi; /* cell position in domain where is valus( poins ) */
	AccuType scope; /* range of fuzzy set */

	scope = fuzzy_set->domain[1] - fuzzy_set->domain[0];
	for( i=0; i<no_poins; i++ )
	{
		cell_posi = ( int ) (( poins[i] - fuzzy_set->domain[0] )/scope*( MF_LENG-1 )); /* cell position */
		fuzzy_set->mf[ cell_posi ] = valus[ i ];
	}
	return;
}

void interMfCurve( struct FuzzySet *fuzzy_set )
{
	int i; /* loop counter */
	int j = 0, k = 0; /* auxiliary pointers */
	int range_from = 0, range_to = 0; /* range of interpolated area */
	float range;

	for( i=0; i<MF_LENG; i++ ) /* for all mf array */
	{
		if( ( fuzzy_set->mf[ i ] != MINUS_ONE ) && ( i!=j ) ) /* if find point different to -1.0 */
		{
			range_from = j+1;
			range_to = i;
			for( k=range_from; k<range_to+1; k++ ) /* nie podoba mi siê tu +1 dla mf_leng */
			{
				range = ( float )( k-j )/( i-j );/* ??? co to nie dzia³a bo same 0.0 s¹ */
				fuzzy_set->mf[ k ] = fuzzy_set->mf[ j ] +
						range*( fuzzy_set->mf[ i ] - fuzzy_set->mf[ j ] );
			}
			j=i; /* move to next range */
		}
	}

	return;
}

void initFuzzySystem( struct FuzzySystem * fuzzy_system)
{
	int i, ii; /* loop counter */
	AccuType doma_poins[ MAX_ERROR_SETS+2 ];	/* Table with characteristic points of error variable*/
	AccuType poins[ MAX_POINS ]; /* array for characteristic points of mf */
	AccuType valus[ MAX_POINS ]; /* array for values of characteristic points of mf */
	/* Calculate position characteristic points for error variable*/
	calcDomaPoins( doma_poins, MAX_SETS, UNIV_OF_DISC );

	for( ii=0; ii<MAX_INPUT_VARIA; ii++) /* variable loop */
	{
		for( i=0; i<MAX_ERROR_SETS; i++ ) /* set loop */
		{
			setFuzzySet( &fuzzy_system->input_varia[ii].fuzzy_sets[i], ZEROF, ZEROF, MINUS_ONE ); /* set error mfs for -1 */
			fuzzy_system->input_varia[ii].fuzzy_sets[ i ].domain[0] = doma_poins[ i ]; /* set begin of domain */
			fuzzy_system->input_varia[ii].fuzzy_sets[ i ].domain[1] = doma_poins[ i+2 ]; /* set end of domain */
		}
		fuzzy_system->input_varia[ ii ].univ_of_disc[ 0 ] = UNIV_OF_DISC[ 0 ];
		fuzzy_system->input_varia[ ii ].univ_of_disc[ 1 ] = UNIV_OF_DISC[ 1 ];
	}
	for( i=0; i<MAX_ERROR_SETS; i++ ) /* set loop */
	{
		setFuzzySet( &fuzzy_system->outpu_varia.fuzzy_sets[i], ZEROF, ZEROF, MINUS_ONE ); /* set error mfs for -1 */
		fuzzy_system->outpu_varia.fuzzy_sets[ i ].domain[0] = doma_poins[ i ]; /* set begin of domain */
		fuzzy_system->outpu_varia.fuzzy_sets[ i ].domain[1] = doma_poins[ i+2 ]; /* set end of domain */
	}
	fuzzy_system->outpu_varia.univ_of_disc[ 0 ] = UNIV_OF_DISC[ 0 ];
	fuzzy_system->outpu_varia.univ_of_disc[ 1 ] = UNIV_OF_DISC[ 1 ];

	/* Left shouldered mf VS */
	poins[ 0 ] = doma_poins[ 0 ];
	valus[ 0 ] = ONEF;
	poins[ 1 ] = doma_poins[ 1 ];
	valus[ 1 ] = ONEF;
	poins[ 2 ] = doma_poins[ 2 ];
	valus[ 2 ] = ZEROF;
	for( ii=0; ii<MAX_INPUT_VARIA; ii++ )
	{
		createMfCurve( &fuzzy_system->input_varia[ ii ].fuzzy_sets[ 0 ], poins, valus, MAX_POINS ); /* create mf */
		interMfCurve( &fuzzy_system->input_varia[ ii ].fuzzy_sets[ 0 ] ); /* interpolate mf */
	}
	createMfCurve( &fuzzy_system->outpu_varia.fuzzy_sets[ 0 ], poins, valus, MAX_POINS ); /* create mf */
	interMfCurve( &fuzzy_system->outpu_varia.fuzzy_sets[ 0 ] ); /* interpolate mf */

	/* Triangular mf : S M L */
	valus[ 0 ] = ZEROF;
	valus[ 1 ] = ONEF;
	valus[ 2 ] = ZEROF;
	for( ii=0; ii<MAX_INPUT_VARIA; ii++ )
	{
		for( i=1; i<( MAX_ERROR_SETS-1 ); i++ )
		{
			poins[ 0 ] = doma_poins[ i+0 ];
			poins[ 1 ] = doma_poins[ i+1 ];
			poins[ 2 ] = doma_poins[ i+2 ];
			createMfCurve( &fuzzy_system->input_varia[ ii ].fuzzy_sets[ i ], poins, valus, MAX_POINS );
			interMfCurve( &fuzzy_system->input_varia[ ii ].fuzzy_sets[ i ] ); /* interpolate mf */
		}
	}
	for( i=1; i<( MAX_ERROR_SETS-1 ); i++ )
	{
		poins[ 0 ] = doma_poins[ i+0 ];
		poins[ 1 ] = doma_poins[ i+1 ];
		poins[ 2 ] = doma_poins[ i+2 ];
		createMfCurve( &fuzzy_system->outpu_varia.fuzzy_sets[ i ], poins, valus, MAX_POINS );
		interMfCurve( &fuzzy_system->outpu_varia.fuzzy_sets[ i ] ); /* interpolate mf */
	}

	/* Right shouldered mf VL */
	poins[ 0 ] = doma_poins[ MAX_ERROR_SETS-1 ];
	valus[ 0 ] = ZEROF;
	poins[ 1 ] = doma_poins[ MAX_ERROR_SETS ];
	valus[ 1 ] = ONEF;
	poins[ 2 ] = doma_poins[ MAX_ERROR_SETS+1 ];
	valus[ 2 ] = ONEF;
	for( ii=0; ii<MAX_INPUT_VARIA; ii++ )
	{
		createMfCurve( &fuzzy_system->input_varia[ ii ].fuzzy_sets[ MAX_ERROR_SETS-1 ], poins, valus, MAX_POINS );
		interMfCurve( &fuzzy_system->input_varia[ ii ].fuzzy_sets[ MAX_ERROR_SETS-1 ] ); /* interpolate mf */
	}
	createMfCurve( &fuzzy_system->outpu_varia.fuzzy_sets[ MAX_ERROR_SETS-1 ], poins, valus, MAX_POINS );
	interMfCurve( &fuzzy_system->outpu_varia.fuzzy_sets[ MAX_ERROR_SETS-1 ] ); /* interpolate mf */

	/* initialize rules */
	initRules( fuzzy_system );
	return;
}

void initRules( FuzzySystem * system )
{
	int col=0, row=0; /* loop sounter */
	const FuzzySetsIdType temp_rules_PD[ MAX_ERROR_SETS ][ MAX_DRIVATE_SETS ] = {
			{ M,   L, VL, VL, VL },
			{ S,   M,  L,  L, VL },
			{ VS,  S,  M,  L, VL },		/* array of rules for PD controller */
			{ VS,  S,  S,  M,  L },
			{ VS, VS, VS,  S,  M },
	};
	const FuzzySetsIdType temp_rules_I[ MAX_INTEGRAL_SETS ] = { VL, L, M, S, VS }; /* array of rules for I controller */

	for( row=0; row<MAX_DRIVATE_SETS; row++ ) /* rewrite rules */
	{
		for( col=0; col<MAX_ERROR_SETS; col++ )
		{
			system->rule_PD[ col ][ row ] = temp_rules_PD[ col ][ row ];
		}
	}

	for( col=0; col<MAX_INTEGRAL_SETS; col++ ) /* rewrite rules */
	{
		system->rule_I[ col ] = temp_rules_I[ col ];
	}
	return;
}

inline Boolean isInDomain( AccuType input, FuzzySet *set )
{
	if( ( set->domain[ 0 ] == SHIFT_UNIV_OF_DISC[ 0 ] ) && ( input <= set->domain[ 0 ] ) ) /* check if input is smaller then SHIFT_UNIV_OF_DISC[0] */
			return TRUE;
	else if( ( set->domain[ 1 ] == SHIFT_UNIV_OF_DISC[ 1 ] ) && ( input >= set->domain[ 1 ] ) ) /* check if input is bigger then SHIFT_UNIV_OF_DISC[1] */
			return TRUE;

	if( input > set->domain[ 0 ]	&& input < set->domain[ 1 ]	)
		return TRUE;
	else
		return FALSE;
}


#ifdef INFER_TYPE_MAMDA
void mamdaInfer( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul )
{
	int row=0, col=0; /* loop counter */
	AccuType w1=ZEROF, w2=ZEROF, w=ZEROF; /* weights for rules */

	/* multiply by gains input values */
	propo = propo * GE;
	deriv = deriv * GD;
	integ = integ* GI;

	/* clear resul array before inference */
	clearResulArray( resul );

	/* inference for PD rules */
	for( row=0; row<MAX_DRIVATE_SETS; row++ ) /* for all rows */
	{
		for( col=0; col<MAX_ERROR_SETS; col++ ) /* for all columns */
		{
			if( isInDomain( propo, &system->input_varia[ P ].fuzzy_sets[ col ] ) ) /* check first predicate */
			{
				if( isInDomain( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] ) ) /* check second predicate */
				{
					/* fuzzyfication */
					w1 = getMembeValue( propo, &system->input_varia[ P ].fuzzy_sets[ col ] );
					w2 = getMembeValue( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] );
					w = minf( w1, w2 ); /* minimum of two weights; operation no 3 */
					minMaxOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_PD[ MAX_DRIVATE_SETS - 1 - row ] [ col ] ], resul );
				}
			}
		}
	}
	return;
}
#endif

AccuType getMembeValue( AccuType input, FuzzySet * set )
{
	IntType posit; /* position in MF array */

	posit = getPosit( input, set );
	return ( set->mf[ posit ] );
}

inline IntType getPosit( AccuType input, FuzzySet * set )
{
	AccuType scope =  set->domain[ 1 ] - set->domain[ 0 ]; /* scope of domain */

	if( ( set->domain[ 0 ] == SHIFT_UNIV_OF_DISC[ 0 ] ) && ( input <= set->domain[ 0 ] ) ) /* check if input is smaller then SHIFT_UNIV_OF_DISC[0] */
		return 0;
	else if( ( set->domain[ 1 ] == SHIFT_UNIV_OF_DISC[ 1 ] ) && ( input >= set->domain[ 1 ] ) ) /* check if input is bigger then SHIFT_UNIV_OF_DISC[1] */
		return ( MF_LENG - 1 );

	return ( int )roundf( ( ( input - set->domain[ 0 ] )/scope*( MF_LENG-1 ) ) ); /* calculate the position*/
}

inline AccuType minf( const AccuType a, const AccuType b )
{
	if( a < b )
	{
		return a;
	}
	else
	{
		return b;
	}
}

inline AccuType maxf( const AccuType a, const AccuType b )
{
	if( a > b )
	{
		return a;
	}
	else
	{
		return b;
	}
}

#if ( defined INFER_TYPE_MAMDA || defined INFER_TYPE_MAMDA_PID )
void minMaxOpera( AccuType w, FuzzySet *set, AccuType *resul)
{
	int i=0; /* loop counter */
	int resul_count; /* position in table of results */
	AccuType scope = MAX_UNIV_OF_DISC - MIN_UNIV_OF_DISC; /* scope of result table */
	AccuType temp_resul = ZEROF; /* temporary result */

	resul_count = ( int ) roundf( ( set->domain[ 0 ] + SHIFT ) / scope * RESUL_LENGT ); /* calculate begin of set at result array */

	for( i=0 ; i<MF_LENG; i++, resul_count++ )
	{
		 temp_resul = minf( w, set->mf[ i ]); /* minimum operation of weight and mf */
		 resul[ resul_count ] = maxf( temp_resul, resul[ resul_count ] ); /* maximum operation of actual value of result and new temporary value */
	}
	return;
}
#endif

AccuType defuzCOG( AccuType * resul_array )
{
	int i = ZERO; /* loop counter */
	AccuType sum_mf_y = ZEROF; /* sums of mf multiplied by y value */
	AccuType sum_mf = ZEROF; /* sums of mf values */

	for( i=0; i<RESUL_LENGT; i++ )
	{
		if( resul_array[ i ] > ZEROF ) /* if mf is more then 0 */
		{
			sum_mf_y += resul_array[ i ] * ( i );
			sum_mf += resul_array[ i ];
		}
	}
	return ( (sum_mf_y/sum_mf)*ABS_UNIV_OF_DISC/( RESUL_LENGT - 1) - SHIFT );//( ( ( sum_mf_y/sum_mf ) / RESUL_LENGT * ABS_UNIV_OF_DISC ) - SHIFT );
}

#ifdef INFER_TYPE_PSP
void pspInfer( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul )
{
	int row=0, col=0; /* loop counter */
	AccuType w1=ZEROF, w2=ZEROF, w=ZEROF; /* weights for rules */

	/* multiply by gains input values */
	propo = propo * GE;
	deriv = deriv * GD;
	integ = integ* GI;

	/* clear resul array before inference */
	clearResulArray( resul );

	/* inference for PD rules */
	for( row=0; row<MAX_DRIVATE_SETS; row++ ) /* for all rows */
	{
		for( col=0; col<MAX_ERROR_SETS; col++ ) /* for all columns */
		{
			if( isInDomain( propo, &system->input_varia[ P ].fuzzy_sets[ col ] ) ) /* check first predicate */
			{
				if( isInDomain( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] ) ) /* check second predicate */
				{
					/* fuzzyfication */
					w1 = getMembeValue( propo, &system->input_varia[ P ].fuzzy_sets[ col ] );
					w2 = getMembeValue( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] );
					w = w1*w2; /* product of two weights; operation no 3 */
					produSumOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_PD[ MAX_DRIVATE_SETS - 1 - row ] [ col ] ], resul );
				}
			}
		}
	}
}
#endif


#if( defined INFER_TYPE_PSP || defined INFER_TYPE_PSP_PID )
void produSumOpera( AccuType w, FuzzySet *set, AccuType *resul )
{
	int i=0; /* loop counter */
	int resul_count; /* position in table of results */
	AccuType scope = MAX_UNIV_OF_DISC - MIN_UNIV_OF_DISC; /* scope of result table */
	AccuType temp_resul = ZEROF; /* temporary result */

	resul_count = ( int ) roundf( ( set->domain[ 0 ] + SHIFT ) / scope * RESUL_LENGT ); /* calculate begin of set at result array */

	for( i=0 ; i<MF_LENG; i++, resul_count++ )
	{
		 temp_resul = w*( set->mf[ i ] ); /* minimum operation of weight and mf */
		 resul[ resul_count ] = maxf( temp_resul, resul[ resul_count ] ); /* maximum operation of actual value of result and new temporary value */
	}
	return;
}
#endif


#ifdef INFER_TYPE_BPBSP
void bpbspInfer( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul )
{
	int row=ZERO, col=ZERO; /* loop counter */
	AccuType w1=ZEROF, w2=ZEROF, w=ZEROF; /* weights for rules */

	/* multiply by gains input values */
	propo = propo * GE;
	deriv = deriv * GD;
	integ = integ* GI;

	/* clear resul array before inference */
	clearResulArray( resul );

	/* inference for PD rules */
	for( row=0; row<MAX_DRIVATE_SETS; row++ ) /* for all rows */
	{
		for( col=0; col<MAX_ERROR_SETS; col++ ) /* for all columns */
		{
			if( isInDomain( propo, &system->input_varia[ P ].fuzzy_sets[ col ] ) ) /* check first predicate */
			{
				if( isInDomain( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] ) ) /* check second predicate */
				{
					/* fuzzyfication */
					w1 = getMembeValue( propo, &system->input_varia[ P ].fuzzy_sets[ col ] );
					w2 = getMembeValue( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] );
					w = w1*w2; /* product of two weights; operation no 3 */
					bpbsOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_PD[ MAX_DRIVATE_SETS - 1 - row ] [ col ] ], resul );
				}
			}
		}
	}
	return;
}
#endif

#if( defined INFER_TYPE_BPBSP || defined INFER_TYPE_BPBSP_PID )
void bpbsOpera( AccuType w, FuzzySet *set, AccuType *resul )
{
	int i=ZERO; /* loop counter */
	int resul_count; /* position in table of results */
	AccuType scope = MAX_UNIV_OF_DISC - MIN_UNIV_OF_DISC; /* scope of result table */
	AccuType temp_resul = ZEROF; /* temporary result */

	resul_count = ( int ) roundf( ( set->domain[ 0 ] + SHIFT ) / scope * RESUL_LENGT ); /* calculate begin of set at result array */

	for( i=0 ; i<MF_LENG; i++, resul_count++ )
	{
		 temp_resul = maxf( ZEROF, ( w+( set->mf[ i ] ) - ONEF ) ); /* minimum operation of weight and mf */
		 resul[ resul_count ] = minf( ONEF, ( temp_resul+resul[ resul_count ] ) ); /* maximum operation of actual value of result and new temporary value */
	}
	return;
}
#endif


void clearResulArray( AccuType *resul )
{
	int i; /* loop counter */
	/* clear result array */
	for( i=0; i<RESUL_LENGT; i++ )
		resul[ i ] = ZEROF;
}


/********************************  PID CONTROLLER ******************************/
#ifdef INFER_TYPE_MAMDA_PID
AccuType mamdaInferPID( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul )
{
	int row=0, col=0; /* loop counter */
	AccuType w1=ZEROF, w2=ZEROF, w=ZEROF; /* weights for rules */
	AccuType upd=ZEROF, ui=ZEROF; /* scores of defuzzyfication */

	/* multiply by gains input values */
	propo = propo * GE;
	deriv = deriv * GD;
	integ = integ* GI;

	/* clear resul array before inference */
	clearResulArray( resul );

	/* inference for PD rules */
	for( row=0; row<MAX_DRIVATE_SETS; row++ ) /* for all rows */
	{
		for( col=0; col<MAX_ERROR_SETS; col++ ) /* for all columns */
		{
			if( isInDomain( propo, &system->input_varia[ P ].fuzzy_sets[ col ] ) ) /* check first predicate */
			{
				if( isInDomain( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] ) ) /* check second predicate */
				{
					/* fuzzyfication */
					w1 = getMembeValue( propo, &system->input_varia[ P ].fuzzy_sets[ col ] );
					w2 = getMembeValue( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] );
					w = minf( w1, w2 ); /* minimum of two weights; operation no 3 */
					minMaxOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_PD[ MAX_DRIVATE_SETS - 1 - row ] [ col ] ], resul );
				}
			}
		}
	}

	/* defuzzyfication COG method for PD*/
	upd = defuzCOG( resul );

	/* clear resul array before inference */
	clearResulArray( resul );

		/* inference for I rules */
		for( col=0; col<MAX_INTEGRAL_SETS; col++ )
		{
			if( isInDomain( integ, &system->input_varia[ I ].fuzzy_sets[ col ] ) )
			{
				w = getMembeValue( integ, &system->input_varia[ I ].fuzzy_sets[ col ] );
				minMaxOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_I[ col ] ], resul );
			}
		}

		/* defuzzyfication COG method for I*/
		ui = defuzCOG( resul );

	return( upd+ui );
}
#endif


#ifdef INFER_TYPE_PSP_PID
AccuType pspInferPID( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul )
{
	int row=0, col=0; /* loop counter */
	AccuType w1=ZEROF, w2=ZEROF, w=ZEROF; /* weights for rules */
	AccuType upd=ZEROF, ui=ZEROF; /* scores of defuzzyfication */

	/* multiply by gains input values */
	propo = propo * GE;
	deriv = deriv * GD;
	integ = integ* GI;

	/* clear resul array before inference */
	clearResulArray( resul );

	/* inference for PD rules */
	for( row=0; row<MAX_DRIVATE_SETS; row++ ) /* for all rows */
	{
		for( col=0; col<MAX_ERROR_SETS; col++ ) /* for all columns */
		{
			if( isInDomain( propo, &system->input_varia[ P ].fuzzy_sets[ col ] ) ) /* check first predicate */
			{
				if( isInDomain( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] ) ) /* check second predicate */
				{
					/* fuzzyfication */
					w1 = getMembeValue( propo, &system->input_varia[ P ].fuzzy_sets[ col ] );
					w2 = getMembeValue( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] );
					w = w1*w2; /* product of two weights; operation no 3 */
					produSumOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_PD[ MAX_DRIVATE_SETS - 1 - row ] [ col ] ], resul );
				}
			}
		}
	}

	/* defuzzyfication COG method for I*/
	upd = defuzCOG( resul );

	/* clear resul array before inference */
	clearResulArray( resul );

	/* inference for I rules */
	for( col=0; col<MAX_INTEGRAL_SETS; col++ )
	{
		if( isInDomain( integ, &system->input_varia[ I ].fuzzy_sets[ col ] ) )
		{
			w = getMembeValue( integ, &system->input_varia[ I ].fuzzy_sets[ col ] );
			produSumOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_I[ col ] ], resul );
		}
	}

	/* defuzzyfication COG method for I*/
	ui = defuzCOG( resul );

	return( upd+ui );
}
#endif


#ifdef INFER_TYPE_BPBSP_PID
AccuType bpbspInferPID( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul )
{
	int row=ZERO, col=ZERO; /* loop counter */
	AccuType w1=ZEROF, w2=ZEROF, w=ZEROF; /* weights for rules */
	AccuType upd=ZEROF, ui=ZEROF; /* scores of defuzzyfication */

	/* multiply by gains input values */
	propo = propo * GE;
	deriv = deriv * GD;
	integ = integ* GI;

	/* clear resul array before inference */
	clearResulArray( resul );

	/* inference for PD rules */
	for( row=0; row<MAX_DRIVATE_SETS; row++ ) /* for all rows */
	{
		for( col=0; col<MAX_ERROR_SETS; col++ ) /* for all columns */
		{
			if( isInDomain( propo, &system->input_varia[ P ].fuzzy_sets[ col ] ) ) /* check first predicate */
			{
				if( isInDomain( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] ) ) /* check second predicate */
				{
					/* fuzzyfication */
					w1 = getMembeValue( propo, &system->input_varia[ P ].fuzzy_sets[ col ] );
					w2 = getMembeValue( deriv, &system->input_varia[ D ].fuzzy_sets[ row ] );
					w = w1*w2; /* product of two weights; operation no 3 */
					bpbsOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_PD[ MAX_DRIVATE_SETS - 1 - row ] [ col ] ], resul );
				}
			}
		}
	}

	/* defuzzyfication COG method for I*/
	upd = defuzCOG( resul );

	/* clear resul array before inference */
	clearResulArray( resul );

	/* inference for I rules */
	for( col=0; col<MAX_INTEGRAL_SETS; col++ )
	{
		if( isInDomain( integ, &system->input_varia[ I ].fuzzy_sets[ col ] ) )
		{
			w = getMembeValue( integ, &system->input_varia[ I ].fuzzy_sets[ col ] );
			bpbsOpera( w, &system->outpu_varia.fuzzy_sets[ system->rule_I[ col ] ], resul );
		}
	}

	/* defuzzyfication COG method for I*/
	ui = defuzCOG( resul );

	return( upd+ui );
}
#endif

inline void shiftDown( AccuType *input )
{
	*input = *input - SHIFT;
}

inline void shiftUp( AccuType *input )
{
	*input = *input + SHIFT;
}

void shiftDomainAndUnivOfDisc( FuzzySystem *system )
{
	int k, kk; /* loop counter */

	for( kk=0; kk<MAX_INPUT_VARIA; kk++ )
	{
		system->input_varia[ kk ].univ_of_disc[ 0 ] -= SHIFT;
		system->input_varia[ kk ].univ_of_disc[ 1 ] -= SHIFT;
		for( k=0; k<MAX_SETS; k++ )
		{
			system->input_varia[ kk ].fuzzy_sets[ k ].domain[ 0 ] -=SHIFT;
			system->input_varia[ kk ].fuzzy_sets[ k ].domain[ 1 ] -=SHIFT;
		}
	}
	system->outpu_varia.univ_of_disc[ 0 ] -= SHIFT;
	system->outpu_varia.univ_of_disc[ 1 ] -= SHIFT;
	for( k=0; k<MAX_SETS; k++ )
	{
		system->outpu_varia.fuzzy_sets[ k ].domain[ 0 ] -= SHIFT;
		system->outpu_varia.fuzzy_sets[ k ].domain[ 1 ] -= SHIFT;
	}
}
