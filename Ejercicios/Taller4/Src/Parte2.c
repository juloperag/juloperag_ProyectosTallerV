/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Ejercicios taller 4
 ******************************************************************************
 */

#include <stdint.h>

char var1 = 0;
uint8_t var2 =0;
uint16_t var3 =0;
uint64_t var4 =0;

int main(void)
{
	uint16_t testShift  = 0b0001100001010011;
	uint16_t testMask  = 0b0000000000000100;

	while(1)
	{
		uint16_t resultado = testShift | testMask;
		resultado = testShift &(~testMask);
	}

	return 0;
}


