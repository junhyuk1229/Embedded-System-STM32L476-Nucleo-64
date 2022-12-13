#include "st_basic.h"

int main(void)
{
    /*Setup initial clock configuration*/
	ClockInit();
	/*Setup initial USART2 configuration*/
	USART2_Init();
	
	/*Print string to USART2*/
	USART2_TX_String("Welcome to Embedded System!\n");
	
	while (1)
	{
		/*Output any input from USART2*/
		USART2_TX(USART2_RX());
	}
}
