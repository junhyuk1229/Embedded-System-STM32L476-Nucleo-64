
#ifndef _ST_BASIC_
#define _ST_BASIC_

#ifndef STM32L476xx
#define STM32L476xx
#endif

#include "stm32l476xx.h"

#define SLAVE_OWN_ADDRESS		0x5A
#define I2C_TIMING				0x00000E14

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
void ADC1_Init(void);
void ADC1_start_conversion(void);
int ADC1_read(void);
void SysTickDelayMs(int delay);
void TIM1_Init(void);
void TIM2_Init(void);
void Button_INT_Init(void);
void SysTick_Init(void);
void TIM1_INT_Init(void);
void I2C1_Slave_Init(void);
void I2C1_SetOwnAddress1(int address, int addressSize);
void I2C3_Master_Init(void);
int I2C1_Get_Address_Matchcode(uint8_t compNum);

#endif
