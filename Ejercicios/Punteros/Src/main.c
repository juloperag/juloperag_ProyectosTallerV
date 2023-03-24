/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Configuracion basica proyecto
 ******************************************************************************
 */

//Definicion de librerias
#include <stdint.h>
#include <stm32f411xe.h>
/*#include <GPIOxDriver.h>
#include <BasicTimer.h>
*/

//Definimos una variable de control
uint32_t counter = 0;
uint32_t auxCounter = 0;
uint32_t *ptr_Counter;

uint8_t	byteVariable;
uint8_t	*ptr_ByteVariable;

int main(void)
{
	counter = 12345;
	auxCounter = counter;
	//Apuntada a la poscicion donde se guarda counter
	ptr_Counter = &counter;
	//Definimos un valor para esa posicion de memoria
	*ptr_Counter = 43658706;
	//aunmentamos 4 posciones en la mememoria;
	ptr_Counter++;
	//Se escribe el valor en la variable auxCounter
	*ptr_Counter = 43658706;

	//Indicamos que apunte hacia la variable
	byteVariable = 32;
	ptr_ByteVariable = &byteVariable;
	*ptr_ByteVariable = 123;

	//Intentando mezclar punteros
	//Le asigamos el valor de la posicion de memoria de ptr_Counter, osea el valor de la poscicion 0x20000001
	auxCounter = (uint32_t)&ptr_Counter; //Haciendo un cast para obtener el valor
	//Definimos que ptr_ByteVariable a punta a la posicion de memoria 0x20000001
	ptr_ByteVariable = (uint8_t *)auxCounter; //Es lo mismo como RCC ((RCC_RegDef_t *)RCC_BASE_ADDR)
	*ptr_ByteVariable=1;

	return 0;
}


