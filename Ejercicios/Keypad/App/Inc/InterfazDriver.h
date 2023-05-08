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

void Interface(USART_Handler_t *prthandlerUSART,uint8_t *amount_containers);
void msgInterface(USART_Handler_t *prthandlerUSART);
void obtainNum(uint8_t data);
uint8_t obtainAmount(USART_Handler_t *prthandlerUSART);

#endif /* INC_INTERFAZDRIVER_H_ */
