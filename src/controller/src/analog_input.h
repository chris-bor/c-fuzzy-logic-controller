/*
 * analog_input.h
 *
 *  Created on: Jan 24, 2011
 *  Author: Krzysztof I. Borowiec
 *  Version 1.0
 */

#ifndef ANALOG_INPUT_H_
#define ANALOG_INPUT_H_

// definitions for analog input
// port
#define ANALOG_INPUT_PORT				GPIOA
// clock for port
#define ANALOG_INPUT_PORT_CLK			RCC_APB2Periph_GPIOA
// pin for port
#define ANALOG_INPUT_PIN				GPIO_Pin_3
// channel for ADC
#define ANALOG_INPUT_CHANNEL			ADC_Channel_3
#define ANALOG_INPUT_SAMPLING_TIME		ADC_SampleTime_1Cycles5
//#define ANALOG_INPUT_SAMPLING_TIME		ADC_SampleTime_13Cycles5


// initialization function for analog input
void analogInputInit( void );

#endif /* ANALOG_INPUT_H_ */
