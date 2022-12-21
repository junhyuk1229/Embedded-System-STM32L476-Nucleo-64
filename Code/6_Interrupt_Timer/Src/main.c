#include "st_basic.h"

//Used to toggle LED
int LEDFlag = 0;

int main(void)
{
	//Set GPIOA5 setting to output and enable GPIOA
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);

	//Enable USART2
	USART2_Init();

	//Enable SysTick interrupt
	TIM1_INT_Init();

	while(1)
	{

	}
}


void TIM1_UP_TIM16_IRQHandler(void)
{
	//Clear flag
	TIM1->SR &= ~TIM_SR_UIF;

	if(LEDFlag == 0)
	{
		//Turn LED Off
		GPIOA->BSRR |= GPIO_BSRR_BR5;
	}
	else
	{
		//Turn LED On
		GPIOA->BSRR |= GPIO_BSRR_BS5;
	}

	LEDFlag ^= 1;

	//Send message through USART
	USART2_TX_String("1 second passed\n");
}
