#include "st_basic.h"

int main(void)
{
	//Enable USART2
	USART2_Init();

	//Enable SysTick interrupt
	TIM1_INT_Init();

	//Enable SysTick interrupt
	SysTick_Init();

	while(1)
	{

	}
}


void SysTick_Handler(void)
{
	//Send message through USART
	USART2_TX_String("0.5 second passed\n");
}


void TIM1_UP_TIM16_IRQHandler(void)
{
	//Clear flag
	TIM1->SR &= ~TIM_SR_UIF;

	//Send message through USART
	USART2_TX_String("1 second passed\n");
}
