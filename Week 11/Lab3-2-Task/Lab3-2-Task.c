/*
Name: Lab3-2-Task.c
Desc: Sets up USART2 for connection with PC using PuTTY
			If n key is inputed turns off LED, if y key is inputed turns on LED
*/

#include "st_basic.h"

int main(void)
{
  /*Setup initial clock configuration*/
	ClockInit();
	/*Setup initial USART2 configuration*/
	USART2_Init();
	
	/*Enable GPIOA5 as output*/
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);
	
	while (1)
	{
		/*Read character input from USART2 and save it in 'tempChar'*/
		char tempChar = USART2_RX();
		/*If input is no, turn off light*/
		if(tempChar == 'n') GPIOA->BSRR = GPIO_BSRR_BR5;
		/*If input is yes, turn on green light*/
		else if(tempChar == 'y') GPIOA->BSRR = GPIO_BSRR_BS5;
	}
}
