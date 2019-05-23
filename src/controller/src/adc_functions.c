/*
 * adc_functions.c
 *
 *  Created on: Jan 24, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#include "adc_functions.h"
#include "analog_input.h"
#include "stm32f10x.h"
#include "my_types.h"

void adcInit( void )
{
	// enable clock
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
	// prscaler = 24Mhz/2=12MHz
	RCC_ADCCLKConfig( RCC_PCLK2_Div2 ) ;
	// initialize structure
	ADC_InitTypeDef  ADC_InitStruct;
	// single conversion mode
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	// right data alignment
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right ;
	// without external triggering
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	// independent mode; ADC1 master ADC2 off
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	// single channel
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	// without scanning ( single channel )
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	// initialization
	ADC_Init(ADC1, &ADC_InitStruct);
	// configuration channel ( group regular or injected )
//	ADC_InjectedChannelConfig( ADC1, ANALOG_INPUT_CHANNEL, 1, ANALOG_INPUT_SAMPLING_TIME );
	ADC_RegularChannelConfig(ADC1, ANALOG_INPUT_CHANNEL, 1, ANALOG_INPUT_SAMPLING_TIME);		// 13.5 cykli
	// run ADC
	ADC_Cmd( ADC1, ENABLE );
	// reset calibration's registers
	ADC_ResetCalibration( ADC1 );
	// wait until reset will be finished
	while( ADC_GetResetCalibrationStatus( ADC1 ) );
	// run calibration
	ADC_StartCalibration( ADC1 );
	// wait until calibration will be done
	while(ADC_GetCalibrationStatus (ADC1) == SET);

	// run ADC convertion
	ADC_SoftwareStartConvCmd( ADC1, ENABLE );
}

void sample( AccuType *input )
{


	/* Get converted value */
	*input = ADC_GetConversionValue(ADC1);

	/* rewrite value from ADC register to x */
//	*input =  ADC1->DR * U_REF / ADC_MAX; /* value in V */
	*input =  ADC1->DR; /* value in bits */
}
