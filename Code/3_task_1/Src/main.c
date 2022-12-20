#include "st_basic.h"

int main(void)
{
	//Set GPIOC13 setting to input and enable GPIOC
	GPIO_Init(GPIOC, 13, GPIO_INPUT);

	//Set up USART2 settings
	USART2_Init();

	while(1)
	{
		//If button is not pressed
		if(GPIOC->IDR & GPIO_IDR_ID13)
		{
			//Print text
			USART2_TX_String("The button is not pressed\n");
		}
		//If button is pressed
		else
		{
			//Print text
			USART2_TX_String("The button is pressed\n");
		}
		//Delay
		for(int i = 0; i < 1000000; i++);
	}
}
