/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Ejercicios taller 4
 ******************************************************************************
 */

#include <stdint.h>

//Defino variablaes



int main(void)
{
	//Defino variablaes
	uint16_t resultado = 0;
	uint16_t testShift  = 0b0001100001010011;
	uint16_t testMask  = 0b0000000000000100;

	while(1)
	{
		//Mascara para cambiar la posicion 2 del numero binario de 16 bits por un 1
		resultado = testShift | testMask;
		//Mascara para cambiar la posicion 2 del numero binario de 16 bits por un 0
		resultado = testShift &(~testMask);
	}

	return 0;
}


