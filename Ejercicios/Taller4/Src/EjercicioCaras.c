/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Ejercicios taller 4
 ******************************************************************************
 */

#include <stdint.h>

//Defino variablaes
uint16_t  facesset1 = 0b0000011011001110;
unsigned short facesset2 = 0b1101100001000111;

int main(void)
{
	//Defino variablaes
	uint16_t PCMasculinas1 = 0;
	uint16_t PCMasculinas2 = 0;
	uint16_t PCFlentes1 = 0;
	uint16_t PCFlentes2 = 0;
	uint16_t PCMbigotes1 = 0;
	uint16_t PCMbigotes2 = 0;
	uint16_t PCIF1 = 0;
	uint16_t PCIF2 = 0;
	uint32_t grupocompleto = 0;

	while(1)
	{
		//Establecemos un 1 en las posiciones que corresponden a caras masculinas
		PCMasculinas1 = ~facesset1;
		PCMasculinas2 = ~facesset2;
		/*Establecemos un 1 en las posiciones que corresponden a caras
		 * femeninas que usen lentes
		 */
		PCFlentes1 = facesset1 & (0b0000010000000000);
		PCFlentes2 = facesset2 & (0b0001000000000000);
		/*Establecemos un 1 en las posiciones que corresponden a caras
		 * masculinas que tienen bigote
		 */
		PCMbigotes1 = (~facesset1) & (0b0001000100010001);
		PCMbigotes2 = (~facesset2) & (0b0000010000001000);
		/*Establecemos un 1 en las posiciones que corresponden a las
		 * personas que puedan ser ingenieros fisicos
		 */
		PCIF1 = (facesset1 | (0b1111111111111111)) & (0b0100101000000101);
		PCIF2 = (facesset2 | (0b1111111111111111)) & (0b1001011000000000);
		/*juntamos los dos numero binario en uno solo*/
		grupocompleto = (facesset1<<16) | facesset2;
	}
	return 0;
}
