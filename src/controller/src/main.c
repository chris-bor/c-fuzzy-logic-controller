 /**
*****************************************************************************
**
**  File        : main.c
**
**	Author		: Krzysztof I. Borowiec
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32F10x Standard Peripherals Library
**  Version 1.0
*****************************************************************************
*/

/* Includes */
#include <stddef.h>
#include <limits.h>// maximum values of variables
#include <stdio.h>
#include <time.h> // for time checking
#include <math.h> // for mathematical calculations
#include <main.h>
#include "stm32f10x.h"
#include "analog_input.h"
#include "adc_functions.h"
#include "dac_functions.h"
#include "my_types.h"
#include "fuzzy_set.h"
#include "fuzzy_operations.h"
#include "fuzzy_variable.h"
#include "fuzzy_system.h"

/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
static struct FuzzySystem fuzzy_system; /* Fuzzy system */
static AccuType resul[ RESUL_LENGT ];
/* Private function prototypes */
/* Private functions */
/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

int main(void)
{
	// variable for controller input value
	AccuType e0 = ZEROF, e1 = ZEROF; /* proportional input */
	AccuType d1 = ZEROF; /* derivative input */
	AccuType i1 = ZEROF; /* integral input */
	AccuType u1 = ZEROF; /* value after defuzzifing */
	int k;
	AccuType mean_e = ZEROF;
	AccuType scale = 0.8/3.0; /* scale for scaling to range from 0V to 2.5V */

	/* Initialization analog input */
	analogInputInit();

	/* Initialize ADC */
	adcInit();

	/* Initialize DAC */
	dacInit();

	/* Initialization fuzzy system */
	initFuzzySystem( &fuzzy_system );

	/* Shift domain and university of discourse */
	shiftDomainAndUnivOfDisc( &fuzzy_system );

	/* sample input as old error */
	for(k=0;k<MAX_MEAS;k++){
	sample( &e0 );
	mean_e+=e0;}
	e0 = mean_e/(float)MAX_MEAS;

	/* shift down */
	shiftDown( &e0 );

	/* Infinite loop */
	while (1)
	{
		/* sample input as new error*/
		mean_e = ZEROF;
		for(k=0;k<MAX_MEAS;k++){
		sample( &e1 );
		mean_e+=e1;}
		e1 = mean_e/(float)MAX_MEAS;

		/* shift down */
		shiftDown( &e1 );

		/* calculate derivative input */
		d1 = e1-e0;

		/* calculate integral input */
		i1 =+ e1;

//		e1 = -500;
//		d1 = 100;
//		i1 = 0;


#ifdef INFER_TYPE_MAMDA
		/* Mamdanie inference */
		mamdaInfer( &fuzzy_system, e1, d1, i1, resul );
#elif INFER_TYPE_PSP
		/* psp inference */
		pspInfer( &fuzzy_system, e1, d1, i1, resul );
#elif INFER_TYPE_BPBSP
		/* bpbsp inferenfe */
		bpbspInfer( &fuzzy_system, e1, d1, i1, resul );

		/***PID inference ***/
#elif INFER_TYPE_MAMDA_PID
		/* Mamdani inference for PID controller */
		u1 = mamdaInferPID( &fuzzy_system, e1, d1, i1, resul );
#elif INFER_TYPE_PSP_PID
		/* psp inference for PID controller */
		u1 = pspInferPID( &fuzzy_system, e1, d1, i1, resul );
#elif INFER_TYPE_BPBSP_PID
		/* bpbsp inference for PID controller */
		u1 = bpbspInferPID( &fuzzy_system, e1, d1, i1, resul );
#endif

#if( defined INFER_TYPE_MAMDA || defined INFER_TYPE_PSP || defined INFER_TYPE_BPBSP )
		/* defuzzyfication COG method for PD*/
		u1 = defuzCOG( resul );
#endif

		u1*=GU;
		/* shift up */
		shiftUp( &u1 );
		//u1 = 4095.0 - u1;

		//przemno¿enie na zakres 1.5V
		u1 = u1*scale;

		/* Set u1 value to DAC */
//		for(u1=0;u1<4000;u1+=500){
		setDACValue( u1 );

		e0 = e1; /* set new error as old error */

	}

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}


