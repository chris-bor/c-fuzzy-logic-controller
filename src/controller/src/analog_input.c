/*
 * analog_input.c
 *
 *  Created on: Jan 24, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#include "stm32f10x.h"
#include "analog_input.h"

void analogInputInit( void )
{
	// enable clk
	RCC_APB2PeriphClockCmd( ANALOG_INPUT_PORT_CLK, ENABLE );
	// initialize structure
	GPIO_InitTypeDef  GPIO_InitStructure;
	// pin
	GPIO_InitStructure.GPIO_Pin = ANALOG_INPUT_PIN;
	// mode analog input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	// initialization
	GPIO_Init( ANALOG_INPUT_PORT, &GPIO_InitStructure );
}
