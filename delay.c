#include <stdint.h>
#include "system_stm32f10x.h"
#include "delay.h"


volatile uint32_t timer_wait = 0;										//Zmienna do odliczenia czasu opuznienia

void SysTick_Handler()													//Uchwyt dla przerwania SysTick
{
	if(timer_wait) { timer_wait--; }
}

void delay_ms(uint32_t time)											//Funkcja do opoznienia w [ms]
{
	timer_wait = time;
	while(timer_wait) {};
}

void delay_Init()														//Funkcja inicjujaca SysTick, przerwanie co 1ms
{
	SystemCoreClockUpdate();					//Aktualizacja SystemCoreClock
	//SysTick_Config(SystemCoreClock / (uint32_t)1000);		//(taktowanie zegara CPU / liczbe wywolan na sekunde) wywolanie co 1 ms
}
