#include "st_basic.h"

int sensor_value;

int main(void)
{
	//Initialize USART2
	USART2_Init();

	//Initialize ADC1
	ADC1_Init();

	//Delay
	for(int i = 0; i < 1000; i++);

	//Start ADC1 conversion
	ADC1_start_conversion();

	while(1)
	{
		//Save ADC1 conversion value to sensor_value variable
		sensor_value = ADC1_read();

		//Print sensor_value
		USART2_TX_String("ADC Output:\t");

		//Checks if sensor_value is connected to pins and prints accordingly
		if(sensor_value == 0)
		{
			USART2_TX_String("Grounded");
		}
		else if(sensor_value > 1000)
		{
			USART2_TX_String("Connected to 3V3");
		}
		else
		{
			USART2_TX_String("Not connected");
		}
		USART2_TX('\n');

		//Delay
		for(int i = 0; i < 1000000; i++);
	}
}
