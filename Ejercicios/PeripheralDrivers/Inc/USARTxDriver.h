/*
 * USARTxDriver.h
 *
 *  Created on: 21/04/2023
 *      Author: julian
 */

#include <stdio.h>
#include <stm32f4xx.h>

#ifndef USARTXDRIVER_H_
#define USARTXDRIVER_H_

//Definicion de macros

#define USART_MODE_TX 				0
#define USART_MODE_RX 				1
#define USART_MODE_RXTX 			2
#define USART_MODE_DISABLE 			3

#define USART_BAUDRATE_9600			0
#define USART_BAUDRATE_19200		1
#define USART_BAUDRATE_115200		2
/*
#define USART_DATASIZE_8BIT    	 	0
#define USART_DATASIZE_9BIT    		1
*/

/*EN PS
 * 0: Even parity
 * 1: Odd parity
 */
#define USART_PARITY_NONE			0
#define USART_PARITY_ODD			1
#define USART_PARITY_EVEN			2

#define USART_STOPBIT_1				0
#define USART_STOPBIT_0_5			1
#define USART_STOPBIT_2				2
#define USART_STOPBIT_1_5			3

/*Estructura que contiene la configuracion minima para el uso del protocolo de comunicacion
 * Velocidad(baudrate)
 * Tamaño de los datos
 * Control de errores
 * Bit de parada
 */

typedef struct
{
	uint8_t USART_mode;
	uint8_t USART_baudrate;
	//uint8_t USART_datasize;
	uint8_t USART_parity;
	uint8_t USART_stopbits;
}USART_Config_t;

//Handler para el uso de USART

typedef struct
{
	USART_Config_t USART_Config;
	USART_TypeDef *ptrUSARTx;
	/*
	//Buffer de recepcion de datos
	uint8_t receptionBuffer[64];
	//Elemento que indica cuantos datos se reciben
	uint8_t dataInputSize;
	//Buffer de transmision de datos
	uint8_t transmisionBuffer[64];
	//Elemento que indica cuantos datos se deben transmitir
	uint8_t dataOutputSize;
	*/
}USART_Handler_t;

//Definicion de las cabeceras de las funciones para el uso del USART
void USART_Config(USART_Handler_t *ptrUsartHandler);
void writeChar(USART_Handler_t *ptrUsartHandlerWrite, uint8_t datatoSend);
void writeString(USART_Handler_t *ptrUsartHandlerString, char *string);
void BasicUSART1_Callback(char data);
void BasicUSART2_Callback(char data);
void BasicUSART6_Callback(char data);

#endif /* USARTXDRIVER_H_ */
