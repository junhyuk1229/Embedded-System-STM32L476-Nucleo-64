#include "st_basic.h"

const uint8_t inputArr[] = "HELLO";
uint8_t* inputPtr = (uint8_t *)inputArr;
uint8_t inputSize = sizeof(inputArr);
volatile uint8_t inputIndex = 0;

uint8_t outputArr[sizeof(inputArr)];
uint8_t outputSize = sizeof(inputArr);
volatile uint8_t outputIndex = 0;

void Handle_I2C_Master(void);
void Slave_Recv_Callback(void);
void I2C1_EV_IRQHandler(void);

int main(void)
{
	I2C1_Slave_Init();
	I2C3_Master_Init();

	Handle_I2C_Master();

	while(1)
	{

	}
}

void Handle_I2C_Master(void)
{
	//Set slave address
	I2C3->CR2 |= SLAVE_OWN_ADDRESS;

	//Set 7-bit mode
	I2C3->CR2 &= ~(I2C_CR2_ADD10);

	//Set transfer size
	I2C3->CR2 |= inputSize << I2C_CR2_NBYTES_Pos;

	//Set automatic end mode
	I2C3->CR2 |= I2C_CR2_AUTOEND;

	//Generate start condition
	I2C3->CR2 |= I2C_CR2_START;

	//Loop until stop flag is raised
	while(!(I2C3->ISR & I2C_ISR_STOPF))
	{
		//Check for transmit empty flag
		if(I2C3->ISR & I2C_ISR_TXIS)
		{
			//Write data in transmit data register
			I2C3->TXDR = (*inputPtr++);
		}
	}

	I2C3->ICR |= I2C_ICR_STOPCF;
}

void Slave_Recv_Callback(void)
{
	//Read 8 bits
	outputArr[outputIndex++] = (I2C1->RXDR & 0xFF);
}

void I2C1_EV_IRQHandler(void)
{
	//Check ADDR flag
	if(I2C1->ISR & I2C_ISR_ADDR)
	{
		//Check if address match
		//if(I2C1_Get_Address_Matchcode(SLAVE_OWN_ADDRESS) == 1)
		//{
			//Verify transfer direction
			if((I2C1->ISR & I2C_ISR_DIR_Msk) == 0)
			{
				//Clear ADDR flag
				I2C1->ICR |= I2C_ICR_ADDRCF;

				//Enable receive interrupt
				I2C1->CR1 |= I2C_CR1_RXIE;
			}
		//}
	}

	//Check RXNE flag
	else if(I2C1->ISR & I2C_ISR_RXNE)
	{
		Slave_Recv_Callback();
	}

	//Check STOP flag
	else if(I2C1->ISR & I2C_ISR_STOPF)
	{
		//Declare stop flag
		I2C1->ICR |= I2C_ICR_STOPCF;
	}
}
