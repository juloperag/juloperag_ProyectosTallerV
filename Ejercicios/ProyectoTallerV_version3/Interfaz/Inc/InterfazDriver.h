/*
 * InterfazDriver.h
 *
 *  Created on: 7/05/2023
 *      Author: julian
 */

#ifndef INC_INTERFAZDRIVER_H_
#define INC_INTERFAZDRIVER_H_



#include <I2CDriver.h>
#include <LCD_I2CDriver.h>
#include <stdint.h>
#include <stdio.h>
#include <GPIOxDriver.h>

//Definimos las cabeceras para el uso de la interfaz de usuario
void executeChar(I2C_Handler_t *prthandlerI2C, char data);

void InterfaceStart(I2C_Handler_t *prthandlerI2C, uint8_t *items_Containers);
void InterfaceConfigContainer(I2C_Handler_t *prthandlerI2C,uint8_t *amount_containers);
void InterfaceOpeCounting(I2C_Handler_t *prthandlerI2C, uint8_t caseOper, uint8_t disco, uint8_t parameter);
uint8_t InterfaceOpeSelectedCon(I2C_Handler_t *prthandlerI2C, uint8_t caseOper,  uint8_t parameter);

void msgInterface(I2C_Handler_t *prthandlerI2C);
void msgContainers(I2C_Handler_t *prthandlerI2C);
void msgNumContainer(I2C_Handler_t *prthandlerI2C, uint8_t num);

void obtainNum(uint8_t data);
uint8_t obtainAmount(void);
uint8_t obtainStage (void);

uint8_t amountContainers(void);
void defineSelectedContainers(uint8_t value);

void executeStopOperaction(I2C_Handler_t *prthandlerI2C, uint8_t disco, uint8_t data);

#endif /* INC_INTERFAZDRIVER_H_ */
