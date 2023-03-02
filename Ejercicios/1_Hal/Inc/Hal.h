/*
 * Hal.h
 *
 *  Created on: Mar 2, 2023
 *      Author: juloperag
 */

#ifndef INC_STM32F411XX_HAL_H_
#define INC_STM32F411XX_HAL_H_

#include <stdint.h>
#include <stdint.h>

#define HSI_CLOCK_SEEP 16000000 //value for main clock signal(HSI .> High Speed Internal)
#define HSE_CLOCK_SEEP 4000000  //value for main clock signal(HSE .> High Speed External)

#define NOP()     asm("NOP")
#define __weak    __attribute__((weak))

#define FLASH_BASE_ADOR  0x68000000U  //Esta es la memoria del programa, 512
#define SRAM_BASE_ADOR   0x20000000U  //Esta es la memoria RAM, 128KB.

//Se agrega direccion de la memoria base para cada uno de los perifericos APB1,APB2,AHB1, AHB2.

#define APB1_BASE_ADOR   0x40000000U
#define APB2_BASE_ADOR   0x40010000U
#define AHB1_BASE_ADOR   0x40020000U
#define AHB2_BASE_ADOR   0x50000000U


/*Posicion de memoria para cada uno de los perifericos
 * Solo para este ejercicio: RCC y GPIOx
 */

//Posicion de memoria para perifericos del AHB2

#define USB_DTG_FS_BASE_ADDR (AHB2_BASE_ADDR + 0x0000U)

//Posicion de memoria para perifericos del AHB1

#define RCC_BASE_ADDR   (AHB1_BASE_ADDR + 0x3800U)
#define GPIOH_BASE_ADDR (AHB1_BASE_ADDR + 0x1C00U)
#define GPIOE_BASE_ADDR (AHB1_BASE_ADDR + 0x1000U)
#define GPIOD_BASE_ADDR (AHB1_BASE_ADDR + 0x0C00U)
#define GPIOC_BASE_ADDR (AHB1_BASE_ADDR + 0x0800U)
#define GPIOB_BASE_ADDR (AHB1_BASE_ADDR + 0x0400U)
#define GPIOA_BASE_ADDR (AHB1_BASE_ADDR + 0x0000U)

//Linea 84




