/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stddef.h>
#include <stdio.h>
#include "stm32f10x.h"
//#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "mainDefines.h"
#include "delay.h"

#include "lcd_tft_ili9481.h"
#include "lcd_tft_graphics.h"

extern _graphicParams graphicParams;
extern _alignVertical alignV;
extern _alignHorizontal alignH;

int main(void)
{
	SystemInit();

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;	//Enable GPIOC

	delay_Init();
	SysTick_Config(SystemCoreClock / (uint32_t)1000);

	//Green LED GPIOC, Pin 13
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_SetBits(GPIOC, GPIO_Pin_13);	//LED OFF
//	GPIO_ResetBits(GPIOC, GPIO_Pin_13);	//LED ON

	// DB0 - DB7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_SetBits(GPIOA, GPIO_Pin_All);

	// DB8 - DB15, LCD_: RS, CS, WR, RST
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_All);

	//stan pinow
	GPIO_SetBits(GPIOB, GPIO_Pin_7);	//LCD_RS  HIGH
	GPIO_SetBits(GPIOB, GPIO_Pin_6);	//LCD_CS  HIGH
	GPIO_SetBits(GPIOB, GPIO_Pin_5);	//LCD_WR  HIGH
	GPIO_SetBits(GPIOB, GPIO_Pin_4);	//LCD_RST HIGH

	TFT_ILI9481_InitLCD(TFT_ILI9481_WIDTH, TFT_ILI9481_HEIGHT, TFT_ILI9481_Orientation_90deg, TFT_ILI9481_COLOR_BLACK);

	ringMeterInit1();


	int16_t i = 0;
	int8_t k = 0;

	while(1)
	{

		ringMeterSetParams1(i*20, i*0.2+20, i*30, i*0.5+20, i*0.3+20, i*0.8+20, i*1.0+20);


		if (k == 0) { i += 5; }
		if (k == 1) { i -= 5; }

		if (i > 100) { i = 100; k = 1; }
		if (i < 0) { i = 0; k = 0; }

//		testLed_Reset;
//		delay_ms(50);


	}
}
