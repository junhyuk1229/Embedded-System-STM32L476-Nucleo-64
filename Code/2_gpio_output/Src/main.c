#include "stm32l476xx.h"

int main(void)
{
	RCC->AHB2ENR |= (1U<<0);
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1)
	{
		GPIOA->ODR ^= (1U<<5);
		for(int i=0;i<100000;i++);
	}
}
