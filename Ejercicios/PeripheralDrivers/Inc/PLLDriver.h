/*
 * PLLDriver.h
 *
 *  Created on: 20/05/2023
 *      Author: julian
 */

#ifndef PLLDRIVER_H_
#define PLLDRIVER_H_

#include <stm32f4xx.h>

//Definicion de macros
#define CLOCK_SPEED_25MHZ  50;
#define CLOCK_SPEED_80MHZ  160;
#define CLOCK_SPEED_100MHZ  200;

//Definicion de cabeceras
void configPLL(uint8_t clockSpeed);
uint8_t getConfigPLL(void);

#endif /* PLLDRIVER_H_ */
