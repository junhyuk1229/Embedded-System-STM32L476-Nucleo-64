/*
Name: st_basic.c
Desc: Saves all functions used for class material.
	  Doesn't contain main function.
*/

#include "st_basic.h"

/*
Function Name: ClockInit
Input Variables: None
Output Variables: None
Desc: Sets up initial settings for clock for use.
*/

void ClockInit(void)
{
	//Increase the delay by 4 wait states(5 clock cycles) to read the flash
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
	
	//Enable PLLR that can be used as the system clock
	//Divide the 16MHz input clock by 2(to 8MHz), multiply by 20(to 160MHz),
	//divide by 2(to 80MHz)
	//Set PLL input source to HSI
	RCC->PLLCFGR = RCC_PLLCFGR_PLLREN | (20 << RCC_PLLCFGR_PLLN_Pos)
							 | RCC_PLLCFGR_PLLM_0 | RCC_PLLCFGR_PLLSRC_HSI;
	
	//Turn on HSI oscillator and PLL circuit.
	RCC->CR |= RCC_CR_PLLON | RCC_CR_HSION;
	
	//Be sure that the wait state of the flash changed,
	//PLL circuit is locked, and HSI is stabilized
	while (!((FLASH->ACR & FLASH_ACR_LATENCY_4WS)
				&& (RCC->CR & RCC_CR_PLLRDY)
				&& (RCC->CR & RCC_CR_HSIRDY)));
	
	//Set the system clock source from PLL
	RCC->CFGR = RCC_CFGR_SW_PLL;
	
	//Turn off MSI to reduce power consumption
	RCC->CR &= ~RCC_CR_MSION;
}

/*
Function Name: GPIO_Init
Input Variables: None
Output Variables: None
Desc: Sets up initial settings for GPIO.
*/
void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode)
{
	unsigned int modeIn32Bit = ((unsigned int)(mode & 3) << (2 * pin));
	unsigned int pullUpDown = ((unsigned int)(mode >> 2) << (2 * pin));
	
	RCC->AHB2ENR |= (1 << (((unsigned int)port - GPIOA_BASE) >> 10));
	
	port->MODER |= modeIn32Bit;
	port->MODER &= (modeIn32Bit | ~((unsigned int)3 << (2 * pin)));
	
	port->PUPDR |= pullUpDown;
	port->PUPDR &= (pullUpDown | ~((unsigned int)3 << (2 * pin)));
}
