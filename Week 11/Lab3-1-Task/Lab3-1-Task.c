/*
Name: Lab3-1-Task.c
Desc: Sets up USART2 for connection with PC using PuTTY
			Sends button's state every 1 second
*/

#include "st_basic.h"

int main(void)
{
  /*Setup initial clock configuration*/
	ClockInit();
	/*Setup initial USART2 configuration*/
	USART2_Init();
	
	/*Enable GPIOC13 as input with pulldown*/
	GPIO_Init(GPIOC, 13, GPIO_INPUT_PULLDOWN);
	
	while (1)
	{
		/*If button is not pressed*/
		if(GPIOC->IDR & GPIO_IDR_ID13)
		{
			/*Output string*/
			USART2_TX_String("Not Pressed\n");
		/*If button is pressed*/
		}
		else
		{
			/*Output string*/
			USART2_TX_String("Pressed\n");
		}
		/*Wait 1 s*/
		Delay(1000);
	}
}
