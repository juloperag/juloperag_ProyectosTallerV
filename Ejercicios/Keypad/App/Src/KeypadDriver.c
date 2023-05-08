/*
 * KeypadDriver.c
 *
 *  Created on: 6/05/2023
 *      Author: julian
 */

#include <GPIOxDriver.h>
#include <KeyPadDriver.h>

//Funcion para determinar la tecla seleccionada
char readKeyPad5x4(Keypad5x4_Rows_t *ptrKeypadRows, GPIO_Handler_t *pGPIOColum, char postecla[][4], uint8_t poscolum)
{
	//Caracter a retornar
	char chartecla = '\0';
	//Posicion de la Fila
	uint8_t posRow = 0;
	//Booleano
	uint8_t booleano=1;
	//Elemento de tipo GPIO_Handler_t para almacenar el pin correspondiente a la fila del KeyPad
	GPIO_Handler_t *ptrGPIORow;

	//Ciclo para determinar la tecla
	while(booleano)
	{
		//Elegimos el pin correspondiente a la fila actual
		switch(posRow)
		{
		case 0:
		{
			ptrGPIORow = ptrKeypadRows->pGPIORow1;
			break;
		}
		case 1:
		{
			ptrGPIORow = ptrKeypadRows->pGPIORow2;
			break;
		}
		case 2:
		{
			ptrGPIORow = ptrKeypadRows->pGPIORow3;
			break;
		}
		case 3:
		{
			ptrGPIORow = ptrKeypadRows->pGPIORow4;
			break;
		}
		case 4:
		{
			ptrGPIORow = ptrKeypadRows->pGPIORow5;
			break;
		}
		default:
		{
			__NOP();
			break;
		}
		}

		//Definimos un valor bajo por el pin seleccionado
		GPIO_writePin(ptrGPIORow, 0);

		//leemos el el pin correspondiente a la columna que salto la interrupcion
		if(GPIO_RedPin(pGPIOColum)==0)
		{
			booleano = 0;
			//Tomamos el caracter del arreglo creado(Correspondiente al teclado)
			chartecla = postecla[posRow][poscolum];
		}
		else
		{
			__NOP();
		}

		//Definimos un valor alto por el pin seleccionado
		GPIO_writePin(ptrGPIORow, 1);
		//Sumamos una posicion a la fila
		posRow++;
	}

	//Retornamos el valor establecido
	return chartecla;
}



