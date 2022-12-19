#include "stm32l4xx.h"

typedef enum GPIO_Mode	{
	GPIO_INPUT, GPIO_OUTPUT, GPIO_ALTERNATIVE, GPIO_ANALOG,
	GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN = 0x8
} GPIO_Mode;

void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode);

int main(void)
{
	//Set GPIOA5 setting to output and enable GPIOA
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);
	//Set GPIOC13 setting to input and enable GPIOC
	GPIO_Init(GPIOC, 13, GPIO_INPUT);

	while(1)
	{
		//If button is not pressed
		if(GPIOC->IDR & GPIO_IDR_ID13)
		{
			//Turn LED off
			GPIOA->BSRR |= GPIO_BSRR_BR5;
		}
		//If button is pressed
		else
		{
			//Turn LED On
			GPIOA->BSRR |= GPIO_BSRR_BS5;
		}
	}
}

/*
Function Name: GPIO_Init
Input Variables: GPIO_PORT, pin number, mode of GPIO
Output Variables: None
Desc: Sets up initial settings for GPIO.
*/
void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode)
{
	//Used to set GPIO's state(bit 0 and 1)
	unsigned int modeIn32Bit = ((unsigned int)(mode & 3) << (2 * pin));
	//Used to set pull up and pull down resistors(bit 2 and 3)
	unsigned int pullUpDown = ((unsigned int)(mode >> 2) << (2 * pin));

	//Enable clock for GPIO
	RCC->AHB2ENR |= (1 << (((unsigned int)port - GPIOA_BASE) >> 10));

	//Change GPIO's state to output or input
	port->MODER |= modeIn32Bit;
	port->MODER &= (modeIn32Bit | ~((unsigned int)3 << (2 * pin)));

	//Change input GPIO's state to pull up or pull down or no resistor
	port->PUPDR |= pullUpDown;
	port->PUPDR &= (pullUpDown | ~((unsigned int)3 << (2 * pin)));
}
