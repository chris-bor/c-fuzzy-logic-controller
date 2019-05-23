/*
 * dac_function.c
 *
 *  Created on: Jan 27, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#include "stm32f10x.h"
#include "dac_functions.h"
#include "stm32f10x_dac.h"
#include "my_types.h"

void dacInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure; /* initialization structure for GPIO */
	DAC_InitTypeDef DAC_InitStructure; /* initialization structure for DAC */

	/* Enable clock for GPIOA */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );

	/* Setting PA4 as AIN */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; /* analog mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; /* number of pin */
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	/* Enable clock for DAC */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC, ENABLE );

	/* DAC configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None; /* trigger off */
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable; /* output buffer on */
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; /* wave generator off */

	/* DAC initialization */
	DAC_Init( DAC_Channel_1, &DAC_InitStructure );

	/* Enable DAC */
	DAC_Cmd( DAC_Channel_1,  ENABLE );
}


void setDACValue( const AccuType value )
{
	/* Set the specified data holding register value for DAC channel 1 */
	DAC_SetChannel1Data( DAC_Align_12b_R, ( uint16_t )value );

}
