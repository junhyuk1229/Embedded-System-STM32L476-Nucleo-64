#include "st_basic.h"

int main(void)
{
	int i = 0;
	ClockInit();
	
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);
	
	while (1)
	{
		/*Turns LD2 On*/
		GPIOA->BSRR = GPIO_BSRR_BS5;
		
		/*Wait for 1 second*/
		for (i = 0; i < 5000000; i++);
		
		/*Turns LD2 Off*/
		GPIOA->BSRR = GPIO_BSRR_BR5;
		
		/*Wait for 1 second*/
		for (i = 0; i < 5000000; i++);
	}
}
