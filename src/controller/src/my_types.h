/*
 * my_types.h
 *
 *  Created on: Jan 29, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#ifndef MY_TYPES_H
#define MY_TYPES_H

#include "stdio.h"
#include "stdint.h"
#include "main.h"

#if( TEST == 1 )
#define MF_LENG 8 /* length of membership array = 8 levels */
#else
#define MF_LENG 32	/* length of membership array = 32 levels */
#endif
#define RESUL_LENGT 3*MF_LENG /* length of result array used to inferencing and defuzzyfication */
#define MAX_SETS 5 /* maximum number of sets in fuzzy variables */
#define MAX_ERROR_SETS 5 /* maximum number of error sets */
#define MAX_DRIVATE_SETS 5 /* maximum number of derivative sets */
#define MAX_INTEGRAL_SETS 5 /* maximum number of intergal sets */
#define MAX_INPUT_VARIA 3 /* maximum number of input linguistic variable */
#define MAX_RULES_PD 25 /* maximum number of rules for PD inference */
#define MAX_RULES_I 5 /* maximum number of rules for I inference */
#define MAX_POINS 3 /* maximum number of characteristic points of mf */
#define MIN_UNIV_OF_DISC 0 /* minimum value of university of discourse */
#define MAX_UNIV_OF_DISC 4095 /* maximum value of university of discourse */
#define ABS_UNIV_OF_DISC 4095 /* absolute value of university of discourse */
#define ZERO 0
#define ZEROF 0.0
#define MINUS_ONE -1.0
#define ONEF 1.0
#define ERROR_VARIA 0 /* Identifier for error linguistic variable */
#define DERIV_VARIA 1 /* Identifier for derivative linguistic variable */
#define INTEG_VARIA 2 /* Identifier for integral linguistic variable */
#define MAX_MEAS 50 /* Max number of measurements */

// type used to set up accuracy
typedef float AccuType; /* accuracy type */
typedef int IntType; /* int type */
typedef enum Bool Boolean; /* boolean type */
typedef enum VariaId VariaIdType; /* variable type */
typedef enum FuzzySetsId FuzzySetsIdType; /* fuzzy sets type */
//typedef enum InferId InferIdTypel; /* inference type */

static const IntType UNIV_OF_DISC[] = { 0, 4095 }; /* university of discourse */
static const IntType SHIFT_UNIV_OF_DISC[] = { -2048, 2047 }; /* shifted university of discourse */
static const AccuType GE = 1.0; /* gain for proportional input */
static const AccuType GD = 1.0; /* gain for derivative input */
static const AccuType GI = 0.02; /* gain for integral input */
static const AccuType GU = 3.0; /* gain for output */

#if( TEST == 1 )
static const AccuType DELTA_Y = 4095/6/8; /* delta y for defuzzyfication */
#else
static const AccuType DELTA_Y = 4095/6/32; /* delta y for defuzzyfication */
#endif
static const AccuType SHIFT = 4096.0/2.0; /* shifting value equal half of university of discourse */

/* Enumeration for fuzzy variable identyfier */
enum VariaId{	P = 0, /* error variable */
				D = 1, /* derivative variable */
				I = 2  /* integral variable */
};

/* Enumeration for fuzzy sets identyfier */
enum FuzzySetsId{ 	VS = 0, /* Very Small */
					S = 1, /* Small */
					M = 2, /* Middle */
					L = 3, /* Large */
					VL = 4 /* Very Large */
};

enum InferId{	MAMDA, /* Mamadani inference */
				PSP, /* product - sum - product */
				BPBSP /* bounded product - bounded sum - product */
};

/* Enumeration for boolean type */
enum Bool{  FALSE,
			TRUE
};

#endif /* MY_TYPES_H */
