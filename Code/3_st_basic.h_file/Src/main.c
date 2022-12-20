#include "st_basic.h"

int main(void)
{
	//Set up USART2 settings
	USART2_Init();

	//Print string to terminal
	USART2_TX_String("USART2 Connected!\n");

	while(1)
	{
		//Take the input of the terminal and output it to the terminal
		USART2_TX(USART2_RX());
	}
}
