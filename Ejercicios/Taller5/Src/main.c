/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Taller 5
 ******************************************************************************
 */

#include <stdint.h>

//la funcion indicada con int--> devuelve un entero, mientras que con void devuelve nada
int8_t funtion1(short par1)
{
	return 0;
}

/* En esta funcion devuelve resultado  y vemos que debe especificase la variable de la misma
 * forma que especificamos en  la funcion*/
int8_t funtion2(uint8_t par1, uint16_t par2)
{
	int8_t resultado = par1+ par2;
	return resultado;
}

int main(void)
{
	while(1)
	{
	}
	return 0;
}
