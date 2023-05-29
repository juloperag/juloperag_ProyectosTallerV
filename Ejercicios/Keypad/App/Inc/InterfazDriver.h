/*
 * InterfazDriver.h
 *
 *  Created on: 7/05/2023
 *      Author: julian
 */

#ifndef INC_INTERFAZDRIVER_H_
#define INC_INTERFAZDRIVER_H_


#include <USARTxDriver.h>
#include <stdint.h>
#include <GPIOxDriver.h>

//Definimos las cabeceras para el uso de la interfaz de usuario
void executeChar(USART_Handler_t *prthandlerUSART, char data);
void InterfaceStart(USART_Handler_t *prthandlerUSART,uint8_t *amount_containers);
void msgInterface(USART_Handler_t *prthandlerUSART);
void msgContainers(USART_Handler_t *prthandlerUSART);
void obtainNum(uint8_t data);
uint8_t obtainAmount();

#endif /* INC_INTERFAZDRIVER_H_ */