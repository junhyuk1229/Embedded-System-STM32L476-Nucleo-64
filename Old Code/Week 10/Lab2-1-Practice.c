/*
Name: Lab2-1-Practice.c
Desc: Set up clock to set specific wanted delay
			LED LD2 blinks on and off every 0.1 secs
*/

#include <stm32l4xx.h>

void ClockInit(void);

int main(void)
{
	/*Used in for loops*/
	int i = 0;
	
  /*Setup initial clock configuration*/
	ClockInit();
	
	/*Enable Clock for GPIOA*/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	/*Change GPIOA5's state to output([0, 0] = input, [0, 1] = output)*/
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	
	while (1)
	{
		/*Turns LD2 on*/
		GPIOA->BSRR = GPIO_BSRR_BS5;
		
		/*Wait for 0.1 second*/
		for (i = 0; i < 500000; i++);
		
		/*Turns LD2 off*/
		GPIOA->BSRR = GPIO_BSRR_BR5;
		
		/*Wait for 0.1 second*/
		for (i = 0; i < 500000; i++);
	}
}


/*
Function Name: ClockInit
Input Variables: None
Output Variables: None
Desc: Sets up initial settings for clock for use.
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
}
