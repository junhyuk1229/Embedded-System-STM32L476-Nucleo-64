/*
Name: Lab2-2-Practice.c
Desc: Set up clocks and systick to get wanted delay
			Make a basic function to get delay(ms)
			Blinks LED LD2 every 1 secs
*/

#include <stm32l4xx.h>

/*Used to store the time passed from the start of the system in ms */
static unsigned int sysMillis = 0;

void ClockInit(void);
void Delay(unsigned int duration);
void SysTick_Handler(void);

int main(void)
{
  /*Setup initial clock configuration*/
	ClockInit();
	
	/*Enable clock for GPIOA*/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	/*Change GPIOA5's state to output([0, 0] = input, [0, 1] = output)*/
	GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	
	while (1)
	{
		/*Turns LD2 on*/
		GPIOA->BSRR = GPIO_BSRR_BS5;
		
		/*Wait for 1 second*/
		Delay(1000);
		
		/*Turns LD2 off*/
		GPIOA->BSRR = GPIO_BSRR_BR5;
		
		/*Wait for 1 second*/
		Delay(1000);
	}
}


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
