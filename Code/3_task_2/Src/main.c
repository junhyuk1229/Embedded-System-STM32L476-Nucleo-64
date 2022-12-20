#include "st_basic.h"

int main(void)
{
	//Set GPIOA5 setting to output and enable GPIOA
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);

	//Set up USART2 settings
	USART2_Init();

	while(1)
	{
		char tempChar = USART2_RX();
		//If the input is 'n'
		if(tempChar == 'n')
		{
			//Turn LED off
			GPIOA->BSRR |= GPIO_BSRR_BR5;
		}
		//If the input is 'y'
		else if(tempChar == 'y')
		{
			//Turn LED On
			GPIOA->BSRR |= GPIO_BSRR_BS5;
		}
	}
}
