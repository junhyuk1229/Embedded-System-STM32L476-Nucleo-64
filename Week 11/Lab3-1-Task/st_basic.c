#include "st_basic.h"

static unsigned int sysMillis = 0;

/*
Function Name: ClockInit
Input Variables: None
Output Variables: None
Desc: Sets up initial settings for the clock to be used.
*/
void ClockInit(void)
{
	/*Increase the delay by 4 wait states(5 clock cycles) to read the flash*/
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
	
	/*Enable PLLR that can be used as the system clock*/
	/*Divide the 16MHz input clock by 2(to 8MHz), multiply by 20(to 160MHz),*/
	/*divide by 2(to 80MHz)*/
	/*Set PLL input source to HSI*/
	RCC->PLLCFGR = RCC_PLLCFGR_PLLREN | (20 << RCC_PLLCFGR_PLLN_Pos)
							 | RCC_PLLCFGR_PLLM_0 | RCC_PLLCFGR_PLLSRC_HSI;
	
	/*Turn on HSI oscillator and PLL circuit.*/
	RCC->CR |= RCC_CR_PLLON | RCC_CR_HSION;
	
	/*Be sure that the wait state of the flash changed,*/
	/*PLL circuit is locked, and HSI is stabilized*/
	while (!((FLASH->ACR & FLASH_ACR_LATENCY_4WS)
				&& (RCC->CR & RCC_CR_PLLRDY)
				&& (RCC->CR & RCC_CR_HSIRDY)));
	
	/*Set the system clock source from PLL*/
	RCC->CFGR = RCC_CFGR_SW_PLL;
	
	/*Turn off MSI to reduce power consumption*/
	RCC->CR &= ~RCC_CR_MSION;
	
	/*Change SysTick to run the SysTick_Handler function every 1ms*/
	SysTick->LOAD = SysTick->CALIB & SysTick_LOAD_RELOAD_Msk;
	
	/*Enable SysTick exception request and enable the SysTick timer*/
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


/*
Function Name: GPIO_Init
Input Variables: None
Output Variables: None
Desc: Sets up initial settings for GPIO.
*/
void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode)
{
  /*Used to set GPIO's state(bit 0 and 1)*/
	unsigned int modeIn32Bit = ((unsigned int)(mode & 3) << (2 * pin));
  /*Used to set pull up and pull down resistors(bit 2 and 3)*/
	unsigned int pullUpDown = ((unsigned int)(mode >> 2) << (2 * pin));
	
  /*Enable clock for GPIO*/
	RCC->AHB2ENR |= (1 << (((unsigned int)port - GPIOA_BASE) >> 10));
	
  /*Change GPIO's state to output or input*/
	port->MODER |= modeIn32Bit;
	port->MODER &= (modeIn32Bit | ~((unsigned int)3 << (2 * pin)));
	
  /*Change input GPIO's state to pull up or pull down or no resistor*/
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
	/*Select HSI16 clock as input for USART2 clock*/
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_1;
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL_0;
	
	/*Enable USART2 clock*/
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	/*Enable USART2_TX/RX pins for input and output*/
	GPIO_Init(GPIOA, 2, GPIO_ALTERNATIVE);
	GPIO_Init(GPIOA, 3, GPIO_ALTERNATIVE);
	
	/*Enable GPIOA2 and GPIOA3 alternative I/O settings for USART2*/
	GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);
	
	/*Set baud rate*/
	USART2->BRR = 139;
	/*Overrun disabled*/
	USART2->CR3 |= USART_CR3_OVRDIS;
	/*Enable transfer, recieve of USART2 and enable it overall*/
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}


/*
Function Name: USART2_RX
Input Variables: None
Output Variables: Character that is received through the USART2
Desc: Gets a character through USART2 from device to computer
*/
char USART2_RX(void)
{
	/*Loop till input buffer is filled*/
	while (!(USART2->ISR & USART_ISR_RXNE));
	/*Return input character*/
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
	/*Loop till output buffer is empty*/
	while (!(USART2->ISR & USART_ISR_TXE));
	/*Send output through USART2*/
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
	/*Keeps looping till 'string' is empty*/
	/*Sends char at address to USART2 then move address by 1*/
	while (*string != '\0') USART2_TX(*string++);
}


/*
Function Name: Delay
Input Variables: Amount of time to delay in ms
Output Variables: None
Desc: Delays the program from running other code for a certain amount of time.
*/
void Delay(unsigned int duration)
{
  /*Saves current system time to prevMillis*/
	unsigned int prevMillis = sysMillis;
  /*Waits till the difference of current time and previous time is less than or
  equal to the number 'duration' in ms*/
	while(sysMillis - prevMillis <= duration);
}


/*
Function Name: SysTick_Handler
Input Variables: None
Output Variables: None
Desc: Adds 1 to sysMillis variable every 1 ms.
*/
void SysTick_Handler(void)
{
	sysMillis++;
}
