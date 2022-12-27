#include "st_basic.h"

//Used to toggle LED
int fallTrigFlag = 0;

int main(void)
{
	//Set GPIOA5 setting to output and enable GPIOA
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);

	//Enable USART2
	USART2_Init();

	//Enable button interrupt
	Button_INT_Init();

	while(1)
	{

	}
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR1 & EXTI_PR1_PIF13_Msk) != 0)
	{
		//Clear flag
		EXTI->PR1 |= EXTI_PR1_PIF13_Msk;

		if(fallTrigFlag == 0)
		{
			//Turn LED Off
			GPIOA->BSRR |= GPIO_BSRR_BR5;

			//Send message through USART
			USART2_TX_String("Rising Edge Detected\n");
		}
		else
		{
			//Turn LED On
			GPIOA->BSRR |= GPIO_BSRR_BS5;

			//Send message through USART
			USART2_TX_String("Falling Edge Detected\n");
		}

		fallTrigFlag ^= 1;

	}
}
