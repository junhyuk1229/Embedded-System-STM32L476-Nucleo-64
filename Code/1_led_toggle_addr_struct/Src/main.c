#include <stdint.h>

#define PERIPH_BASE		(0x40000000UL)
#define AHB1_OFFSET		(0x00020000UL)
#define AHB2_OFFSET		(0x08000000UL)

#define AHB1_PERIPH_BASE		(PERIPH_BASE + AHB1_OFFSET)
#define AHB2_PERIPH_BASE		(PERIPH_BASE + AHB2_OFFSET)

#define RCC_OFFSET		(0x00001000UL)
#define RCC_BASE		(AHB1_PERIPH_BASE + RCC_OFFSET)

#define GPIOA_OFFSET	(0x000UL)
#define GPIOA_BASE		(AHB2_PERIPH_BASE + GPIOA_OFFSET)

#define PIN_5			(1U<<5)
#define LED_PIN			(PIN_5)

#define GPIOAEN			(1U<<0)

typedef struct
{
	volatile uint32_t DUMMY[19];
	volatile uint32_t AHB3ENR;
} RCC_TypeDef;

typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t DUMMY[4];
	volatile uint32_t ODR;
} GPIO_TypeDef;

#define GPIOA			((GPIO_TypeDef *)GPIOA_BASE)
#define RCC				((RCC_TypeDef *)RCC_BASE)

int main(void)
{
	RCC->AHB3ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1)
	{
		GPIOA->ODR ^= LED_PIN;

		for(int i=0;i<100000;i++);
	}
}
