#include "st_basic.h"

int sensor_value;

int main(void)
{
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
	}
}
