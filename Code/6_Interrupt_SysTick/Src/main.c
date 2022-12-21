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
	SysTick_Init();

	while(1)
	{

	}
}


void SysTick_Handler(void)
{
	//Clear flag
	EXTI->PR1 |= EXTI_PR1_PIF13_Msk;

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
