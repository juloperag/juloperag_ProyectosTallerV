/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Ejemplo de struct
 ******************************************************************************
 */

#include <stdint.h>
#include <StructureExample.h>

BasicExample muestraA = {0};

int main(void)
{
	muestraA.ID = 4;
	muestraA.ID2 = 40;
	muestraA.DRR = 3;

	while(1)
	{
		for(char e=0; e<muestraA.DRR;e++)
		{

		}
	}

	return 0;
}
