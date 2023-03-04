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
//Pagina 53 Datasheet

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

//Posicion de memoria para perifericos del APB2
//Para este ejercicio no es necesario, solo se escribe un ejemplo

/*#define TIM11_BASE_ADDR (APB2_BASE_ADOR + 0x4800U)
* #define TIM10_BASE_ADDR (APB2_BASE_ADOR + 0x4400U)
*/

//Posicion de memoria para perifericos del APB1
//Para este ejercicio no es necesario, solo se escribe un ejemplo

/*#define I2C3_BASE_ADDR  (APB1_BASE_ADOR  + 0x5C00U)
 *#define I2C2_BASE_ADDR  (APB1_BASE_ADOR  + 0x5800U)
 */

//Macros Genericos

#define ENABLE 			1
#define DISABLE 		0
#define SET 			ENABLE
#define CLEAR			DISABLE
#define RESET			DISABLE
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET
#define	FLAG_SET		SET
#define	FLAG_RESET		RESET
#define	I2C_WRITE		0
#define	I2C_READ		1

/* ++============INICIO de la descripcion de los elementos de los perifericos ==========++*/

/* Definicion de la estructura de datos que presenta a cada uno de los registros que componen el
 * periferico RCC.
 */

/*Por el curso solo se define los bits de los registros: 6.3.1(RCC_CR) hasta el 6.3.12(RCC_APB2ENR),
 * 6.3.17 (RCC_BDCR) y 6.3.18 (RCC_CSR)
 */

/*NOTA: El offset debe encajar perfectamente en la posicion de memria del equio, indicada en la
 * de datos, observace que tambien esta indicado los elementos "reservedx".
 */







