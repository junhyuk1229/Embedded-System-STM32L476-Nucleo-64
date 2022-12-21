#include "st_basic.h"

int main(void)
{
	//Set GPIOA5 setting to output and enable GPIOA
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);

	//Enable TIMER1
	TIM1_Init();

	while(1)
	{
		//Turn LED off
		GPIOA->BSRR |= GPIO_BSRR_BR5;

		//Delay 1 second
		while(!(TIM1->SR & TIM_SR_UIF));

		//Clear flag
		TIM1->SR &= ~TIM_SR_UIF;

		//Turn LED on
		GPIOA->BSRR |= GPIO_BSRR_BS5;

		//Delay 1 second
		while(!(TIM1->SR & TIM_SR_UIF));

		//Clear flag
		TIM1->SR &= ~TIM_SR_UIF;
	}
}
