/*
 * mainDefines.h
 *
 *  Created on: 15.03.2020
 *      Author: Mateusz
 */

#include "stm32f10x_gpio.h"

#ifndef MAINDEFINES_H_

#define testLed_Pin		GPIO_Pin_13
#define testLed_Port	GPIOC
#define testLed_Set		GPIO_SetBits(testLed_Port, testLed_Pin)
#define testLed_Reset	GPIO_ResetBits(testLed_Port, testLed_Pin)
//#define testLed_Toggle	GPIO_ToggleBits(testLed_Port, testLed_Pin)




#endif /* MAINDEFINES_H_ */
