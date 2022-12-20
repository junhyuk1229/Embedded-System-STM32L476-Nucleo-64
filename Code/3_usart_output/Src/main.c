#include "stm32l476xx.h"

typedef enum GPIO_Mode	{
	GPIO_INPUT, GPIO_OUTPUT, GPIO_ALTERNATIVE, GPIO_ANALOG,
	GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN = 0x8
} GPIO_Mode;

void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode);

int main(void)
{
	//Enable clock access to GPIOA for PA2 as TX line
	//Set PA2, PA3 mode as alternate function mode
	GPIO_Init(GPIOA, 2, GPIO_ALTERNATIVE);

	//Set the alternate function typed to UART
	GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);

	//Enable clock access to USART2
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	//Configure baudrate
	USART2->BRR = (4000000 + 115200 / 2) / 115200;

	//Enable Transfer
	USART2->CR1 = USART_CR1_TE;
	//Enable USART module
	USART2->CR1 |= USART_CR1_UE;

	while(1)
	{
		//Loop till output buffer is empty
		while (!(USART2->ISR & USART_ISR_TXE));
		//Send output through USART2 char 'I'
		USART2->TDR = 'I';
		//Delay
		for(int i = 0; i < 1000000; i++);
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
