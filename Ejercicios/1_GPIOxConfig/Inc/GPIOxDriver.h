/*
 * Hal.h
 *
 *  Created on: Mar 2, 2023
 *      Author: juloperag
 */

/*Describe una estructura que nos permite configurar el PINx, ademas tambien la creacion de una estructura
 *Hlader que contiene la configuracion del usuario y el apuntador al periferico.
 */

#ifndef INC_GIOXDRIVER_H_
#define INC_GIOXDRIVER_H_

#include <Stm32f4xx.h>

//Definimos una estructura de datos que contienen las variables para la configuracion del PIN
typedef struct
{
	uint8_t GPIO_PinNumber;			// Pin con el que se desea trabajar
	uint8_t GPIO_PinModer;			// Modo de la  configuracion
	uint8_t	GPIO_PinOTPype;			// Selecionados PPD o OpenDrain
	uint8_t GPIO_PinSpeed;  		// Velocidad de  puesta del pin(solo salida)
	uint8_t GPIO_PinPUPdControl;	// Seleccionar si se desea una salida Pull-up, Pull-down o libre
	uint8_t	GPIO_PinAltFunMode;		// Selecionados cual es la funcion alternativa que desea seleccionar

}GPIO_PinConfig_t;


typedef struct
{
	/*Renombramos la estructura para usarla con el fin de configurar el puerto
	 */
	GPIO_PinConfig_t GPIO_PinConfig;

	/*Renombramos la estructura para usarla con el fin de definir la direccion al puerto que el pin corresponde
	 */
	GPIOx_RegDef_t *pGIOx;

}GPIO_Handler_t;

/* Definicion de las cabeceras de las funciones del GPIOxDriver */

void GPIO_Config (GPIO_Handler_t *pGPIOHandler);
void GPIO_writePin (GPIO_Handler_t *pPinHandler, uint8_t newState);
uint32_t GPIO_RedPin (GPIO_Handler_t *pPinHandler);
void GPIOxTooglePin(GPIO_Handler_t *pPinHandler);

#endif



