/*
 * Ej1.c
 *
 *  Created on: 23/03/2023
 *      Author: julian
 */

#include <stdint.h>
#include <stm32f411xe.h>

int main(void)
{
	uint8_t dato = 10;

	uint8_t *pDato = &dato;

	//Dentro de la memoria
	uint8_t auxdato = *pDato;

	//Direccion de memoria
	uint32_t addrDato = (uint32_t) pDato;

	pDato ++;

	pDato ++;

	*pDato = 1;

	//-------------------------Arreglos

	//al crear un arreglo estamos creando un puntero
	uint32_t miPrimerArreglo[5] = {12,15,0XABC,0b1011110111,21};

	uint32_t variable = *miPrimerArreglo; //almacena el primer arreglo elemento del arreglo

	for(uint8_t i=0; i<5; i++)
	{
		uint32_t auxvariable = miPrimerArreglo[i];
		//tambien una forma de acceder seria

		uint32_t auxvariable2 = *(miPrimerArreglo+i);

	}

	//-------------------------Estructuras
	typedef struct
	{
		uint8_t edad;
		uint32_t estatura;
		uint16_t notas[10];
	}Estudiante_t;

	Estudiante_t estudiante1 = {0};
								//NOTA: se señala con "." si no es un puntero, de lo contrario seria con "->"
	estudiante1.edad = 22;
	estudiante1.estatura = 173;
	estudiante1.notas[0] = 45;

	Estudiante_t *pEstudiante1 = (Estudiante_t *) 0x20010000;

	pEstudiante1->edad = 22;
	pEstudiante1->estatura = 173;
	pEstudiante1->notas[0] = 45;

	//-------------------------String

	char miString[] = "Linux es mejor";	//Guarda cada letra como un elemento de la un arreglo
	char miString2[3] = {'L', 'i', 'n'}; //comillas simples para elementos

	return 0;
}
