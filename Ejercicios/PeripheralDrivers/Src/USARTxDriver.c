/*
 * USARTxDriver.c
 *
 *  Created on: 21/04/2023
 *      Author: julian
 */

#include <stm32f4xx.h>
#include "USARTxDriver.h"
//Se cuenta con 3 USART: USART1, USART2, USART6

//Variable que guarda la referencia del periferico a utilizar
USART_TypeDef *ptrUSART1Used;
USART_TypeDef *ptrUSART2Used;
USART_TypeDef *ptrUSART6Used;

//Funcion para cargar la configuracion del periferico USART
void USART_Config(USART_Handler_t *ptrUsartHandler)
{
	//---------------------------------1) Activamos el periferico------------------------------------------
	//Registro: APB1ENR
	//Registro: APB2ENR

	if(ptrUsartHandler->ptrUSARTx == USART1)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB2ENR = RCC_APB2ENR_USART1EN;
		//Guardamos una referencia al periferico que estamos utilizando
		ptrUSART1Used = ptrUsartHandler->ptrUSARTx;
	}

	if(ptrUsartHandler->ptrUSARTx == USART2)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR = RCC_APB1ENR_USART2EN;
		//Guardamos una referencia al periferico que estamos utilizando
		ptrUSART2Used = ptrUsartHandler->ptrUSARTx;
	}

	else if(ptrUsartHandler->ptrUSARTx == USART6)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB2ENR = RCC_APB2ENR_USART6EN;
		//Guardamos una referencia al periferico que estamos utilizando
		ptrUSART6Used = ptrUsartHandler->ptrUSARTx;
	}

	//-------------------------------2) Limpieza de registros ------------------------------------------
	//Registro: CR1
	//Registro: CR2

	ptrUsartHandler->ptrUSARTx->CR1 = 0;
	ptrUsartHandler->ptrUSARTx->CR2 = 0;

	//-------------------------3) Configuracion del parity y tamaño de dato ------------------------------
	//Registro: CR1

	//Verificamos si la paridad esta activada
	if(ptrUsartHandler->USART_Config.USART_parity != USART_PARITY_NONE)
	{
		//Activamos Parity control Enable PCE
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_PCE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_PCE;

		//Definimos un tamaño de datos de 9 bits M = 1
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_M;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_M;

		//Verificamos si se selecciona ODD o EVEN
		if(ptrUsartHandler->USART_Config.USART_parity == USART_PARITY_ODD)
		{
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_PS;
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_PS;
		}
		else
		{
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_PS;
		}

	}
	else
	{
		//Desactivamos la Parity control Enable PCE
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_PCE;
		//Definimos un tamaño de datos de 8 bits M = 0
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_M;
	}

	//--------------------------------4) Configuracion del Stop bits--------------------------------------
	//Registro: CR2

	//Se verifica que stop bits se selecciono
	switch(ptrUsartHandler->USART_Config.USART_stopbits)
	{
	//Para el 1 Stop bit se escribe 00
	case USART_STOPBIT_1:
	{
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11<<USART_CR2_STOP_Pos);
		ptrUsartHandler->ptrUSARTx->CR2 |= (0b00<<USART_CR2_STOP_Pos);
		break;
	}
	//Para el 0.5 Stop bit se escribe 01
	case USART_STOPBIT_0_5:
	{
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11<<USART_CR2_STOP_Pos);
		ptrUsartHandler->ptrUSARTx->CR2 |= (0b01<<USART_CR2_STOP_Pos);
		break;
	}
	//Para el 2 Stop bit se escribe 10
	case USART_STOPBIT_2:
	{
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11<<USART_CR2_STOP_Pos);
		ptrUsartHandler->ptrUSARTx->CR2 |= (0b10<<USART_CR2_STOP_Pos);
		break;
	}
	//Para el 1.5 Stop bit se escribe 11
	case USART_STOPBIT_1_5:
	{
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11<<USART_CR2_STOP_Pos);
		ptrUsartHandler->ptrUSARTx->CR2 |= (0b11<<USART_CR2_STOP_Pos);
		break;
	}
	//En caso por defecto selsccionamos un bit de paraba
	default:
	{
		ptrUsartHandler->ptrUSARTx->CR2 &= ~(0b11<<USART_CR2_STOP_Pos);
		ptrUsartHandler->ptrUSARTx->CR2 |= (0b00<<USART_CR2_STOP_Pos);
		break;
	}
	}

	//-------------------5) Configuracion del Baudrate(Velocidad de Trans o Rec)-------------------------
	//Registro: BRR       //Numero de 32 bits

	if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600)
	{
		//Velocidad de 9600bps
		/*El valor a cargar es 104.1875
		 * Mantiza = 104 = 0x68
		 * fraction = 16*0.1875 = 3
		 * Valor a cargar 0x0683
		 */
		//Se carga el valor de la velocidad en el registro
		ptrUsartHandler->ptrUSARTx->BRR = 0x0683;
	}
	else if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_19200)
	{
		//Velocidad de 19200bps
		/*El valor a cargar es 52.0625
		 * Mantiza = 52 = 0x34
		 * fraction = 16*0.0625 = 1
		 * Valor a cargar 0x0341
		 */
		//Se carga el valor de la velocidad en el registro
		ptrUsartHandler->ptrUSARTx->BRR = 0x0341;
	}
	else if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_115200)
		{
		//Velocidad de 115200bps
		/*El valor a cargar es 8.6875
		 * Mantiza = 8 = 0x08
		 * fraction = 16*0.6875 = 11
		 * Valor a cargar 0x008B
		 */
		//Se carga el valor de la velocidad en el registro
		ptrUsartHandler->ptrUSARTx->BRR = 0x008B;
	}
	else
	{
		__NOP();
	}
	//-------------------6) Configuramos el MODO-------------------------
	//Registro: CR1

	//MODOS: TX only, RX only, RXTX, disable

	switch(ptrUsartHandler->USART_Config.USART_mode)
	{
	//Activamos la parte del sistema encargada de enviar
	case USART_MODE_TX :
	{
		//Activamos el Baud rate clock para la transmision
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TE;

		break;
	}
	//Activamos la parte del sistema encargada de recibir
	case USART_MODE_RX:
	{
		//Activamos el Baud rate clock para la recepcion
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RE;

		//------------Configuramos interrupcion---------------
		//Activamos el bit RXNEIE
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RXNEIE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RXNEIE;

		__disable_irq();
		if(ptrUsartHandler->ptrUSARTx == USART1)
		{
			NVIC_EnableIRQ(USART1_IRQn);
		}
		else if(ptrUsartHandler->ptrUSARTx == USART2)
		{
			NVIC_EnableIRQ(USART2_IRQn);
		}
		else if(ptrUsartHandler->ptrUSARTx == USART6)
		{
			NVIC_EnableIRQ(USART6_IRQn);
		}
		__enable_irq();

		break;
	}
	//Activamos ambas  parte del sistema encargadas de enviar y recibir
	case USART_MODE_RXTX:
	{
		//Activamos el Baud rate clock para la transmision
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TE;
		//Activamos el Baud rate clock para la recepcion
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RE;


		break;
	}
	//Desativamos ambos canales
	case USART_MODE_DISABLE:
	{
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
		break;
	}
	default:
	{
		//Actuando por defecto, desactivamos ambos canales
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
		break;
	}
	}

	//-------------------6) Activacion del modulo Serial------------------------
	//Registro: CR1

	if(ptrUsartHandler->USART_Config.USART_mode != USART_MODE_DISABLE)
	{
		//Activamos el modulo Serial
		ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
		ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_UE;
	}
}

//Funcion para escribir un solo char

void writeChar(USART_Handler_t *ptrUsartHandler, uint8_t datatoSend)
{

	//Almacenamos un elemento char en el registro USART_DR
	ptrUsartHandler->ptrUSARTx->DR = datatoSend;

	//Se genera una pausa del codigo, mientras se completa la transferencia de datos
	while(!(ptrUsartHandler->ptrUSARTx->SR & USART_SR_TC))
	{
		__NOP();
	}
	ptrUsartHandler->ptrUSARTx->SR &= ~USART_SR_TC;
}

//Funcion para escribir un string
void writeString(USART_Handler_t *ptrUsartHandlerString, char *string)
{
	uint8_t i = 0;

	while(string[i] != '\0')
	{
		writeChar(ptrUsartHandlerString, string[i]);
		i++;
	}
}

//Definimos las funciones para cuando se genera una interrupcion del USART1-2 y 6
__attribute__((weak)) void BasicUSART1_Callback(char data)
{
	__NOP();
}


__attribute__((weak)) void BasicUSART2_Callback(char data)
{
	__NOP();
}


__attribute__((weak)) void BasicUSART6_Callback(char data)
{
	__NOP();
}


/* Cuando se produce una interrupcion en el NVIC debido a uno de los USART apuntara a una de
 * estas funciones en el vector de interrupciones respectivamente.
 * Con ello Guardamos el elemento char recibido
 */

void USART1_IRQHandler(char data)
{
	//Confirmamos que el registro RXNE esta activo
	while(!(ptrUSART1Used->SR & USART_SR_RXNE))
	{
		__NOP();
	}

	char datatoReceiveUSART1;
	//Leemos el registro DR del respectivo USART
	datatoReceiveUSART1 = ptrUSART1Used->DR;
	//Ademas el bit RXNE es limpiado con la lectura del registro DR
	BasicUSART2_Callback(datatoReceiveUSART1);
}

void USART2_IRQHandler(char data)
{
	//Confirmamos que el registro RXNE esta activo
	while(!(ptrUSART2Used->SR & USART_SR_RXNE))
	{
		__NOP();
	}

	char datatoReceiveUSART2;
	//Leemos el registro DR del respectivo USART
	datatoReceiveUSART2 = ptrUSART2Used->DR;
	//Ademas el bit RXNE es limpiado con la lectura del registro DR
	BasicUSART2_Callback(datatoReceiveUSART2);
}

void USART6_IRQHandler(void)
{
	//Confirmamos que el registro RXNE esta activo
	while(!(ptrUSART6Used->SR & USART_SR_RXNE))
	{
		__NOP();
	}

	char datatoReceiveUSART6;
	//Leemos el registro DR del respectivo USART
	datatoReceiveUSART6 = ptrUSART6Used->DR;
	//Ademas el bit RXNE es limpiado con la lectura del registro DR
	BasicUSART2_Callback(datatoReceiveUSART6);
}
