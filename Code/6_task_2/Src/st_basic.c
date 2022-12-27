#include "st_basic.h"

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

/*
Function Name: ADC1_Init
Input Variables: None
Output Variables: None
Desc: Used to set settings for ADC1 at pin PA4
*/
void ADC1_Init(void)
{
	//Enable A4 and set it for analog input / output
	GPIO_Init(GPIOA, 4, GPIO_ANALOG);

	//Connect GPIO analog switch to ADC input
	GPIOA->ASCR |= GPIO_ASCR_ASC4;

	//Enable clock access to ADC module
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

	//Set ADC common clock
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE_0;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;

	//Set to single conversion
	ADC1->CFGR &= ~ADC_CFGR_CONT;

	//Set sequencer length
	ADC1->SQR1 = 0;

	//Set ADC channel and rank
	ADC1->SQR1 |= ADC_SQR1_SQ1_0 | ADC_SQR1_SQ1_3;

	//Disable ADC deep power down
	ADC1->CR &= ~ADC_CR_DEEPPWD;

	//Enable ADC internal regulator
	ADC1->CR |= ADC_CR_ADVREGEN;

	//Start ADC calibration
	ADC1->CR |= ADC_CR_ADCAL;

	//Enable ADC
	ADC1->CR |= ADC_CR_ADEN;
}

/*
Function Name: ADC1_start_conversion
Input Variables: None
Output Variables: None
Desc: Starts ADC1 conversion
*/
void ADC1_start_conversion(void)
{
	//Start ADC conversion
	ADC1->CR |= ADC_CR_ADSTART;
}

/*
Function Name: ADC1_read
Input Variables: None
Output Variables: None
Desc: Reads ADC1 and returns value
*/
int ADC1_read(void)
{
	//Wait till conversion finishes
	while(!(ADC1->ISR |= ADC_ISR_EOC_Msk));

	//Clear end of conversion flag
	ADC1->ISR |= ADC_ISR_EOC;

	//Return value at data resistor
	return(ADC1->DR);
}

/*
Function Name: SysTickDelayMs
Input Variables: amount of milliseconds to delay
Output Variables: None
Desc: Delays for 'delay' ms.
*/
void SysTickDelayMs(int delay)
{
	//Set value to 0
	SysTick->VAL = 0;
	//Lock SysTick with 1 millisecond amount of cycles
	SysTick->LOAD = 4000 - 1;
	//Select clock source and enable SysTick
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	//Loop for delay milliseconds
	for(int i = 0; i < delay; i++)
	{
		//wait until 1ms passes
		while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	}

	//Turn off SysTick
	SysTick->CTRL = 0;
}

/*
Function Name: TIM1_Init
Input Variables: None
Output Variables: None
Desc: Enables timer 1 with 1hz
*/
void TIM1_Init(void)
{
	//Enable TIMER1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	//Divide input clock by 400
	TIM1->PSC = 400 - 1;

	//Increment every 10000 cycles
	TIM1->ARR = 10000 - 1;

	//Set initial counter to 0
	TIM1->CNT = 0;

	//Enable timer
	TIM1->CR1 = TIM_CR1_CEN;
}

/*
Function Name: TIM2_Init
Input Variables: None
Output Variables: None
Desc: Enables timer 2 to send output without looping to LD2
*/
void TIM2_Init(void)
{
	//Set output to alternative
	GPIO_Init(GPIOA, 5, GPIO_ALTERNATIVE);

	//Set the alternate function to output
	GPIOA->AFR[0] |= 1 << GPIO_AFRL_AFSEL5_Pos;

	//Enable TIMER2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	//Divide input clock by 400
	TIM2->PSC = 400 - 1;

	//Increment every 10000 cycles
	TIM2->ARR = 10000 - 1;

	//Set output compare toggle mode
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1;

	//Set Match value
	TIM2->CCR1 = 0;

	//Enable CH1 compare mode
	TIM2->CCER |= TIM_CCER_CC1E;

	//Set initial counter to 0
	TIM2->CNT = 0;

	//Enable timer
	TIM2->CR1 = TIM_CR1_CEN;
}

/*
Function Name: Button_INT_Init
Input Variables: None
Output Variables: None
Desc: Enables interrupt for user button at B1
*/
void Button_INT_Init(void)
{
	//Set pin 13 to input
	GPIO_Init(GPIOC, 13, GPIO_INPUT);

	//Enable clock to SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	//Connect PC13 to interrupt
	SYSCFG->EXTICR[3] &= ~(13 << SYSCFG_EXTICR4_EXTI13_Pos);
	SYSCFG->EXTICR[3] |= 2 << SYSCFG_EXTICR4_EXTI13_Pos;

	//Unmask EXTI13
	EXTI->IMR1 |= EXTI_IMR1_IM13;

	//Select detect falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;

	//Select detect rising edge trigger
	EXTI->RTSR1 |= EXTI_RTSR1_RT13;

	//Enable EXTI at NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}


/*
Function Name: SysTick_Init
Input Variables: None
Output Variables: None
Desc: Initializes SysTick for interrupt.
*/
void SysTick_Init(void)
{
	//Set value to 0
	SysTick->VAL = 0;
	//Lock SysTick with 1 second amount of cycles
	SysTick->LOAD = 4000000 - 1;
	//Select clock source and enable SysTick
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	//Enable SysTick interrupt
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/*
Function Name: TIM1_INT_Init
Input Variables: None
Output Variables: None
Desc: Enables timer 1 with interrupt
*/
void TIM1_INT_Init(void)
{
	//Enable TIMER1 clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	//Divide input clock by 400
	TIM1->PSC = 400 - 1;

	//Increment every 10000 cycles
	TIM1->ARR = 10000 - 1;

	//Set initial counter to 0
	TIM1->CNT = 0;

	//Enable timer
	TIM1->CR1 = TIM_CR1_CEN;

	//Enable timer update interrupt
	TIM1->DIER |= TIM_DIER_UIE;

	//Enable interrupt in NVIC
	NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}
