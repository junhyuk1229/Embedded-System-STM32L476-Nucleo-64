#include "stm32l476xx.h"

typedef enum GPIO_Mode	{
	GPIO_INPUT, GPIO_OUTPUT, GPIO_ALTERNATIVE, GPIO_ANALOG,
	GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN = 0x8
} GPIO_Mode;

void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode);
void USART2_Init(void);
char USART2_RX(void);
void USART2_TX(char character);
void USART2_TX_String(const char *string);
void USART_BAUD(USART_TypeDef *USARTx, int PeriphClk, int Baudrate);

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


/*
Function Name: GPIO_Init
Input Variables: None
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

/*
Function Name: USART2_Init
Input Variables: None
Output Variables: None
Desc: Sets up initial settings for USART2 use.
*/
void USART2_Init(void)
{
	//Set PA2, PA3 mode as alternate function mode: USART2_TX/RX pins for input and output
	GPIO_Init(GPIOA, 2, GPIO_ALTERNATIVE);
	GPIO_Init(GPIOA, 3, GPIO_ALTERNATIVE);

	//Set the alternate function typed to UART
	GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);

	//Enable GPIOA2 and GPIOA3 alternative I/O settings for USART2
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	//Configure baudrate
	USART_BAUD(USART2, 4000000, 115200);

	//Overrun disabled
	USART2->CR3 |= USART_CR3_OVRDIS;

	//Configure transfer direction
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE;

	//Enable UART module
	USART2->CR1 |= USART_CR1_UE;
}

/*
Function Name: USART2_RX
Input Variables: None
Output Variables: Character that is received through the USART2
Desc: Gets a character through USART2 from device to computer
*/
char USART2_RX(void)
{
	//Loop till input buffer is filled
	while (!(USART2->ISR & USART_ISR_RXNE));
	//Return input character
	return (char)USART2->RDR;
}


/*
Function Name: USART2_TX
Input Variables: character: Character that is going to be sent through the USART2
Output Variables: None
Desc: Sends a character through USART2 from computer to device
*/
void USART2_TX(char character)
{
	//Loop till output buffer is empty
	while (!(USART2->ISR & USART_ISR_TXE));
	//Send output through USART2
	USART2->TDR = character;
}

/*
Function Name: USART2_TX_String
Input Variables: string: String that is going to be sent through the USART2
Output Variables: None
Desc: Sends a string through USART2 from computer to device
*/
void USART2_TX_String(const char *string)
{
	//Keeps looping till 'string' is empty
	//Sends char at address to USART2 then move address by 1
	while (*string != '\0') USART2_TX(*string++);
}

/*
Function Name: USART_BAUD
Input Variables: USART, Device clock time, the baudrate wanted
Output Variables: None
Desc: Sets the baudrate of the selected USART
*/
void USART_BAUD(USART_TypeDef *USARTx, int PeriphClk, int Baudrate)
{
	USARTx->BRR = (PeriphClk + Baudrate / 2) / Baudrate;
}
