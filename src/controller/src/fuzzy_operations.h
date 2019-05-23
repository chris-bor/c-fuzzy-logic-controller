/*
 * fuzzy_operations.h
 *
 *  Created on: Jan 19, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#ifndef FUZZY_OPERATIONS_H
#define FUZZY_OPERATIONS_H

#include "fuzzy_set.h"
#include "fuzzy_system.h"


/* Sets fuzzy set
 * function sets parameters drafted as arguments
 * @param1 pointer to fuzzy set
 * @param2 id
 * @param3 domain[0]
 * @param4 domain[1]
 * @param5 value of mf
 */
void setFuzzySet( struct FuzzySet *,
		const AccuType, const AccuType,
		const AccuType);


/* Calculate position characteristic points of fuzzy sets
 *******	  *		 *		*	   ********
 *		*	 * *	* *	   * *	  *
 *		 *	*	*  *   *  *   *	 *
 *		  **	 **		**	   **
 *		  **	 **		**     **
 *		 *  *	*  *   *  *   *  *
 *		*    * *	* *	   * *    *
 *     *      *      *      *      *
 0   682.5   1365  2047.5 2730   3412.5    4095
  VS          S      M      L           VL
 * @param1 table for calculated points
 * @param2 number of sets
 * @param3 universe of discourse
 */
void calcDomaPoins( AccuType *, const IntType, const IntType *);


/* Calculate values of memberhip functions for fuzzy set respectively to points.
 * Can create shouldered( trapezoidal ) and triangular mfs
 *  @param1 pointer to fuzzy set
 *  @param2 array of characteristic points of mf
 *  @param3 values of mf for characteristic points
 *  @param4 number of characteristic points
 */
void createMfCurve( struct FuzzySet *, AccuType *, AccuType *valus, const IntType );


/* Function to interpolation mf by linearity intervals
 * @param1 pointer to fuzzy set
 */
void interMfCurve( struct FuzzySet *);


/* Function to initialization of fuzzy system
 *  @param1 pointer to fuzzy system
 */
void initFuzzySystem( struct FuzzySystem * );


/* Initialize fuzzy rule structures
 * @param pointer to fuzzy set
 */
void initRules( FuzzySystem * system );


/* Function for checking if input value is in domain range of fuzzy set's of fuzzy variable
 * @retur TRUE or FALSE
 * @param input value which is checked
 * @param pointer to fuzzy set
 */
inline Boolean isInDomain( AccuType, FuzzySet * );


/* Function for inference; runs for all rules at fuzzy system
 * @param pointer to fuzzy system
 * @param proportional input
 * @param derivative input
 * @param integral input
 */
void mamdaInfer( FuzzySystem *, AccuType, AccuType, AccuType, AccuType *);


/* Function to getting value of membership function
 *
 */
AccuType getMembeValue( AccuType input, FuzzySet * set );


/* Function to calculate position input in mf array
 * @retur posiotion
 * @param input value
 * @param pointer to fuzzy set
 */
inline IntType getPosit( AccuType input, FuzzySet * set );


/* Function to calculation minimum of two AccuType arguments
 * @param first argument
 * @param second argument
 */
inline AccuType minf( AccuType a, AccuType b );


/* Function to calculation maximum of two AccuType arguments
 * @param first argument
 * @param second argument
 */
inline AccuType maxf( AccuType a, AccuType b );


/* Function to calculate maximum fuzzy operation
 * @param weight
 * @param pointer to fuzzy set
 * @param array of results
 * @param university of discourse to calculate position begin and end of fuzzy mf at result array
 */
void minMaxOpera( AccuType w, FuzzySet *set, AccuType *resul);

/* Function for calculation defuzzyfication ( returns crips value )
 * @param returned crips value
 * @param pointer to array where is calculate COG defuzzyfication
 */
AccuType defuzCOG( AccuType * );


/* Function to psp inference
 * @param pointer to fuzzy system,
 * @param proportional input,
 * @param derivative input,
 * @param integral input,
 * @param array with results of inference
 */
void pspInfer( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul );


/* Function to calculate psp inference
 * @param input weight of rule
 * @param pointer to input set
 * @param array with results
 */
void produSumOpera( AccuType w, FuzzySet *set, AccuType *resul );


/* Function to bpbsp inference
 * @param pointer to fuzzy system,
 * @param proportional input,
 * @param derivative input,
 * @param integral input,
 * @param array with results of inference
 */
void bpbspInfer( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul );


/* Function to calculate boundary product and boundary sum
 * @param input weight
 * @param pointer to fuzzy set
 * @param array with results of calculation
 */
void bpbsOpera( AccuType w, FuzzySet *set, AccuType *resul );


/* Function for Mamdani inference PID controller; runs for all rules at fuzzy system
 * @param pointer to fuzzy system
 * @param proportional input
 * @param derivative input
 * @param integral input
 */
AccuType mamdaInferPID( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul );


/* Function to clearing result array
 * @param pointer to result array which is cleaned
 */
void clearResulArray( AccuType *resul );


/* Function for psp inference PID controller; runs for all rules at fuzzy system
 * @param pointer to fuzzy system
 * @param proportional input
 * @param derivative input
 * @param integral input
 */
AccuType pspInferPID( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul );

/* Function for bpbsp inference PID controller; runs for all rules at fuzzy system
 * @param pointer to fuzzy system
 * @param proportional input
 * @param derivative input
 * @param integral input
 */
AccuType bpbspInferPID( FuzzySystem * system, AccuType propo, AccuType deriv, AccuType integ, AccuType *resul );


/* Function to shifting DOWN value of argument which is passed
 * @param pointer to input value
 */
inline void shiftDown( AccuType *input );


/* Function to shifting UP value of argument which is passed
 * @param pointer to input value
 */
inline void shiftUp( AccuType *input );


/* Function to shifting domain and university of discourse
 *@param pointer to fuzzy system
 */
void shiftDomainAndUnivOfDisc( FuzzySystem *system );


#endif /* FUZZY_OPERATIONS_H */
