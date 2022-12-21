#include "st_basic.h"

int main(void)
{
	//Set GPIOA5 setting to output and enable GPIOA
	GPIO_Init(GPIOA, 5, GPIO_ALTERNATIVE);

	//Toggle LED without looping every 1 second
	TIM2_Init();

	while(1)
	{

	}
}
