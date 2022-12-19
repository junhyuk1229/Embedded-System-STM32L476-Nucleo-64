#include "stm32l4xx.h"

int main(void)
{
	RCC->AHB2ENR |= (1U<<0);
	RCC->AHB2ENR |= (1U<<2);

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	while(1)
	{
		if(GPIOC->IDR & (1U<<13))
		{
			GPIOA->BSRR |= (1U<<5);
		}
		else
		{
			GPIOA->BSRR |= (1U<<21);
		}
	}
}
