/**
 ******************************************************************************
 * @file           : punto2Tarea.c
 * @author         : juloperag
 * @brief          : Punto 2. Tarea 2
 ******************************************************************************
 */

//Incluye librerias
#include <stdint.h>
#include <Stm32f4xx.h>
#include <GPIOxDriver.h>


int main(void)
{
	//-------------------Inicio de Configuracion GPIOx-----------------------

	GPIO_Handler_t handlerUserPin = {0};
	/*Utilizamos la estrutura GPIOx_RegDef_t (nombrada como pGIOx en la estrutura GPIO_Handler_t)
	 * con la posicion de memoria de GPIOx selecionado.
	 * Vease Stm32f4xx.h :
	 * #define GPIOx ((GPIOx_RegDef_t *) GPIOx_BASE_ADDR) / Ej #define GPIOA_BASE_ADDR (AHB1_BASE_ADDR + 0x0000U)
	 */
	handlerUserPin.pGIOx = GPIOA;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_OUT; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinOTPype 		= GPIO_OTYPER_PUSHPULL;	//GPIO_OTYPER_x---> PUSHPULL, OPENDRAIN
	handlerUserPin.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_MEDIUM;	//GPIO_OSPEEDR_x---> LOW, MEDIUM, FAST, HIGH
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);
	//-------------------Fin de Configuracion GPIOx-----------------------

	//Definimos un estado 1 en la salida del PIN_5.
	GPIO_writePin (&handlerUserPin, 1);

	while(1)
	{
		//Cambiamos el estado del pin por su valor opuesto
		GPIOxTooglePin(&handlerUserPin);

	}

	return 0;
}
