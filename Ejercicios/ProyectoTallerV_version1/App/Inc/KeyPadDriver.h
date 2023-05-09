/*
 * KeyPadDriver.h
 *
 *  Created on: 6/05/2023
 *      Author: julian
 */

#ifndef INC_KEYPADDRIVER_H_
#define INC_KEYPADDRIVER_H_

//Estructura que contiene los pines definidos para las filas del KeyPad
typedef struct
{
	GPIO_Handler_t *pGPIORow1;
	GPIO_Handler_t *pGPIORow2;
	GPIO_Handler_t *pGPIORow3;
	GPIO_Handler_t *pGPIORow4;
	GPIO_Handler_t *pGPIORow5;

}Keypad5x4_Rows_t;

//Definimos la cabecera para la funcion
char readKeyPad5x4(Keypad5x4_Rows_t *ptrKeypadRows, GPIO_Handler_t *pGPIOColum, char postecla[][4], uint8_t poscolum);

#endif /* INC_KEYPADDRIVER_H_ */
