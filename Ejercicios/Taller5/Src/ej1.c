/*
 * ej1.c
 *
 *  Created on: 9/03/2023
 *      Author: julian
 */

//Incluye librerias
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

//para definir variable indicamos con define
#define CONTANTE 100

bool variableBooleana = true;
uint8_t parametro1 = 100;
uint16_t parametro2 = 173;

/*Se puede definir encabezados de tal forma que luego en el codigo definamos que hace dicha funcion*/
void clearGlobal(void);
uint8_t getMaxChar(void);
uint16_t getMaxValue(uint16_t x, uint16_t y, uint16_t z);

//para la funcion main siempre se defini un int ya que devuelve un int
int main(void)
{
	clearGlobal();
	parametro1 = getMaxChar(); //Aqui guardamos la variable que se retorna de la funcion
	uint16_t parametro3 = getMaxValue(100,200,50);

	return 0;
}

/*Esta es la definicion de la funcion, donde no recibe nada por el (void)
 * y no entrega nada por el void inicial
 */
void clearGlobal(void)
{
	variableBooleana = true;
	parametro1 = 0;
	parametro2 = 0;
}

uint8_t getMaxChar(void)
{
	uint8_t maxChar = pow(2,8)-1; //aqui utilizamos la libreria math ---pow(variable,potencia)---
	return maxChar;
}

uint16_t getMaxValue(uint16_t x, uint16_t y, uint16_t z)
{
	if ((x>=y) && (x>=z))
	{
		return x;
	}
	else if((y>=x) && (x>=z))
	{
		return y;
	}
	else
	{
		return z;
	}
}

