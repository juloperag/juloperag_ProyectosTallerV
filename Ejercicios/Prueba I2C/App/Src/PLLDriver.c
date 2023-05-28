/*
 * PLLDriver.c
 *
 *  Created on: 20/05/2023
 *      Author: julian
 */

#include <PLLDriver.h>

uint8_t auxValue = 0;

void configPLL(uint8_t clockSpeed)
{
	//Guardamos en una variable auxiliar el valor de la velocidad del reloj
	auxValue = clockSpeed;

	//-------------1) Seleccion del HSI como la fuente de reloj para el PLL---------------
	//Registro: PLLCFGR
	RCC->PLLCFGR &= ~(0b1<<RCC_PLLCFGR_PLLSRC_Pos);

	//---------------------------2) Configuracion del PLL----------------------------------
	//Registro: PLLCFGR

	//----a) Modificacion del factor divisor M---------
	     //Señal de entrada al VCO entre 1 MHz y 2 MHz
	RCC->PLLCFGR |= (0x8<<RCC_PLLCFGR_PLLM_Pos);  //De acuerdo al Manual de usuario se especifica un valor de 8 para que la señal de entrada al VCO sea de 2 Mhz

	//----b) Modificacion del factor multiplicador N-----
         //Señal de salida al VCO entre 100 MHz y 438 MHz
		 //El valor de N sera entre de 50 y 200
	RCC->PLLCFGR |= (clockSpeed<<RCC_PLLCFGR_PLLN_Pos); //De acuerdo al Manual de usuario y los calculos realizados el valor de N sera entre 50 y 200, para un valor de P de 4.

	//----c) Modificacion del factor divisor P-----
	     //Señal de salida del PLL entre 25 MHz y 100 MHz
	RCC->PLLCFGR |= (0b01<<RCC_PLLCFGR_PLLP_Pos);   //El valor establecido es 4

	//---------------------------3) Activacion PLL----------------------------------
	//Registro: CR

	RCC->CR |= RCC_CR_PLLON;  //Escribimos un valor alto en el bit PLLON para su habilitacion

	//Espera de la activacion del PLL
	while(!(RCC->CR & RCC_CR_PLLRDY))
	{
		__NOP();
	}

	//---------------------------4) Valor de Latencia----------------------------------
	//Registro: ACR

	//Se define el valor de la latencia de acuerdo a la velocidad de reloj establecida
	if (180<clockSpeed && clockSpeed<=200 )
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
	}
	else if (128<clockSpeed && clockSpeed<=180)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
	}
	else if (60<clockSpeed && clockSpeed<=128)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_1WS;
	}
	else
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_0WS;
	}

	//------------------5) Seleccion del PLL como la fuente de reloj del sistema----------------------------
	//Registro: CFGR

	RCC->CFGR |= (0b10<<RCC_CFGR_SWS_Pos);
}

//Funcion que retorna la velocidad de reloj entregado por el PLL en MHz
uint8_t getConfigPLL(void)
{
	return (auxValue/2);
}
