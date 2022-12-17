/*
Name: Lab3-2-Task+.c
Desc: Sets up USART2 for connection with PC using PuTTY
			When 'off' or 'on' is inputed, turns LED off or on accordingly
*/

#include "st_basic.h"
/*Used for malloc in line 27*/
#include <stdlib.h>

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
		/*Make a char address to save input string*/
		char *inputString = (char *)malloc(sizeof(char) * 5);
		/*Read character input from USART2 and save it in 'tempChar'*/
		USART2_RX_String(inputString);
		/*If input is no, turn off light*/
		if(strcmp(inputString, "off") == 0) GPIOA->BSRR = GPIO_BSRR_BR5;
		/*If input is yes, turn on green light*/
		else if(strcmp(inputString, "on") == 0) GPIOA->BSRR = GPIO_BSRR_BS5;
		/*Free memory space to get next input string*/
		free(inputString);
	}
}
