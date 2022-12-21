#include "st_basic.h"

int main(void)
{
	//Set GPIOA5 setting to output and enable GPIOA
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);

	while(1)
	{
		//Turn LED off
		GPIOA->BSRR |= GPIO_BSRR_BR5;

		//Delay 1 second
		SysTickDelayMs(1000);

		//Turn LED on
		GPIOA->BSRR |= GPIO_BSRR_BS5;

		//Delay 1 second
		SysTickDelayMs(1000);
	}
}
