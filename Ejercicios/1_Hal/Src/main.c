/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Configuracion del Hal
 ******************************************************************************
 */

#include <stdint.h>
#include <Hal.h>

GPIOx_RegDef_t registro={0};

int main(void)
{
	registro.MODER = 0;
	registro.OTYPER = 1;

	while(1)
	{

	}

	return 0;
}
