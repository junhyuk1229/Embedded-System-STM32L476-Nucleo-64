#include <stm32l4xx.h>

typedef enum GPIO_Mode
{
	GPIO_INPUT, GPIO_OUTPUT, GPIO_ALTERNATIVE, GPIO_ANALOG,
	GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN = 0x8
} GPIO_Mode;

void ClockInit(void);
void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode);

int main(void)
{
    /*Setup initial clock configuration*/
	ClockInit();
	
	/*Enable GPIOA5 as output*/
	GPIO_Init(GPIOA, 5, GPIO_OUTPUT);
	/*Enable GPIOC13 as input with pulldown*/
	GPIO_Init(GPIOC, 13, GPIO_INPUT_PULLDOWN);
	
	while (1)
	{
		/*If button is not pressed*/
		if(GPIOC->IDR & GPIO_IDR_ID13)
		{
			/*Turns LD2 off*/
			GPIOA->BSRR = GPIO_BSRR_BR5;
		/*If button is pressed*/
		}
		else
		{
			/*Turns LD2 on*/
			GPIOA->BSRR = GPIO_BSRR_BS5;
		}
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
