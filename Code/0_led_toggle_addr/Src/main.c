#define PERIPH_BASE		(0x40000000UL)
#define AHB1_OFFSET		(0x00020000UL)
#define AHB2_OFFSET		(0x08000000UL)

#define AHB1_PERIPH_BASE		(PERIPH_BASE + AHB1_OFFSET)
#define AHB2_PERIPH_BASE		(PERIPH_BASE + AHB2_OFFSET)

#define RCC_OFFSET		(0x00001000UL)
#define RCC_BASE		(AHB1_PERIPH_BASE + RCC_OFFSET)

#define GPIOA_OFFSET	(0x000UL)
#define GPIOA_BASE		(AHB2_PERIPH_BASE + GPIOA_OFFSET)
#define GPIOA_MODE_R	(*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))

#define MODE_R_OFFSET	(0x00)

#define AHB2EN_R_OFFSET			(0x4C)
#define RCC_AHB2EN_R			(*(volatile unsigned int *)(RCC_BASE + AHB2EN_R_OFFSET))

#define OD_R_OFFSET		(0x14U)
#define GPIOA_OD_R		(*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))

#define PIN_5			(1U<<5)
#define LED_PIN			(PIN_5)

#define GPIOAEN			(1U<<0)


int main(void)
{
	RCC_AHB2EN_R |= GPIOAEN;
	GPIOA_MODE_R |= (1U<<10);
	GPIOA_MODE_R &= ~(1U<<11);

	while(1)
	{
		GPIOA_OD_R ^= LED_PIN;

		for(int i=0;i<100000;i++);
	}
}
