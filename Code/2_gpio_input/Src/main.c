#include "stm32l4xx.h"

int main(void)
{
	//Enable GPIOA Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	//Set GPIOA5 setting to output[1, 0]
	//Set pin to 1
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	//Set pin to 0
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1;

	//Set GPIOC13 setting to input[0, 0]
	//Set pin to 0
	GPIOC->MODER &= ~GPIO_MODER_MODE13_0;
	//Set pin to 0
	GPIOC->MODER &= ~GPIO_MODER_MODE13_1;

	while(1)
	{
		//If button is not pressed
		if(GPIOC->IDR & GPIO_IDR_ID13)
		{
			//Turn LED off
			GPIOA->BSRR |= GPIO_BSRR_BR5;
		}
		//If button is pressed
		else
		{
			//Turn LED On
			GPIOA->BSRR |= GPIO_BSRR_BS5;
		}
	}
}
