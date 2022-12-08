#include <stm32l4xx.h>

int main(void)
{
	/*Used in for loops*/
	int i = 0;
	
	/*Enable Clock for GPIOA*/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	/*Change GPIOA5's State to Output([0, 0] = Input, [0, 1] = Output)*/
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1;

	while (1)
	{
		/*Turns LD2 On*/
		GPIOA->BSRR = GPIO_BSRR_BS5;
		
		/*Delay*/
		for (i = 0; i < 500000; i++);
		
		/*Turns LD2 Off*/
		GPIOA->BSRR = GPIO_BSRR_BR5;
		
		/*Delay*/
		for (i = 0; i < 500000; i++);
	}
}
