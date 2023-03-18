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
#include <GPIOxDriver.h>


int main(void)
{
	//-------------------Inicio de Configuracion GPIOx-----------------------
	//Definimos un elemento del tipo GPIO_Handler_t (Struct)
	GPIO_Handler_t handlerUserPin = {0};

	//---------------PIN: PA5----------------
	//Definimos el periferico GPIOx a usar.
	handlerUserPin.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handlerUserPin, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handlerUserPin);

	//-------------------Fin de Configuracion GPIOx-----------------------


	//Definimos para el PIN un 1 logico,
	GPIO_writePin (&handlerUserPin, SET);
	while(1)
	{

	}

	return 0;
}
