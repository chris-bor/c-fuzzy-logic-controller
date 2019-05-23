/**
*****************************************************************************
**
**  File        : main.c
**
**	Author		: Krzysztof I. Borowiec
**
**  Abstract    :
**  	ADC pracuje w trybie POJEDYNCZYM i zglasza przerwania.
**  	DMA transferuje dane do pamieci.
**  	MCU przetwarza dane.
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32F10x Standard Peripherals Library
*****************************************************************************
*/

/* Includes */
#include <stddef.h>
#include <limits.h>		// maximum values of variables
#include <stdio.h>
#include <time.h>	// for time checking
//#include <stdbool.h>
//#include "stm32f10x.h"
//#include "main.h"
//#include "analog_input.h"
//#include "adc_functions.h"
#include "my_types.h"
#include "fuzzy_set.h"
#include "fuzzy_operations.h"

/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
struct FuzzySet *ERR;
/* Private function prototypes */
/* Private functions */
//void clearFDB( FDB * );
/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

int main(void)
{
    // miejsce przechowania wartosci z przetwornika
	// uint16_t ADCVal[1];
	// variable for controller input value
	AccuType x = 0.0;

	// variables for time checking
//	clock_t c1, c2;
	clock_t c_start = 0.0, c_stop = 0.0, c_start_one_loop = 0.0, c_stop_one_loop = 0.0;
	float c_total_time = 0.0, c_one_loop = 0.0;

	c_start = clock();

	// initialization analog input
//	analogInputInit();

	// initialize ADC
//	adcInit();

	/* Infinite loop */
	while (1)
	{
		// check time for overall time
		c_start_one_loop = clock();

//		sample( &x );

		clearFuzzySet( &ERR );
//		ERR->id = 5;

		c_stop_one_loop = clock();
		c_one_loop = (float)(c_stop_one_loop-c_start_one_loop)/(float)CLOCKS_PER_SEC;
		c_stop = clock();
		c_total_time = (float)(c_stop-c_start)/(float)CLOCKS_PER_SEC;
	}
}
