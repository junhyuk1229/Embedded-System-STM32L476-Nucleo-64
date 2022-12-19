#include "stm32l476xx.h"

int main(void)
{
	//Enable GPIOA Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	//Set GPIOA5 setting to output[1, 0]
	//Set pin to 1
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	//Set pin to 0
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1;

	//Keep looping
	while(1)
	{
		//Turn LED On
		GPIOA->BSRR |= GPIO_BSRR_BS5;
		//Delay
		for(int i=0;i<100000;i++);
		//Turn LED Off
		GPIOA->BSRR |= GPIO_BSRR_BR5;
		//Delay
		for(int i=0;i<100000;i++);
	}
}
