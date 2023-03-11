/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Configuracion de los GPIO
 ******************************************************************************
 */

#include <stdint.h>
#include <Stm32f4xx.h>

uint32_t valorRead = 0;

int main(void)
{

	//Configuracion PC13
	//Configuracion inicial del MCU
	RCC->AHB1ENR &= ~(1<<2); 	//Borrar la posicion sea cual sea del AHB1ENR
	RCC->AHB1ENR |= 1<<2; 		//Activando el ciclo de relog en el AHB1

	//COnfiguracion MODER
	GPIOC->MODER &= ~(0b11<<26); //Limpiando la posicion presente en el pin 5
	GPIOC->MODER |= (0b00<<26); //Establecemos el pin 5 como salida

	//Configuracion OTYDER
	GPIOC->OTYPER &= ~(0b1<<13); //configuramos el pin 5 como salida

	//configuracion OPSEDD

	GPIOC->OSPEEDR &= ~(0b11<<26); //Limpiando la posicion presente en el pin 5
	GPIOC->OSPEEDR |= (0b01<<26); //Establecemos la velocidad de escritura del pin 5

	uint32_t valor = 0;

	while(1)
	{

		//Leemos el registro IDR
		//pinValue = (pPinHandler->pGIOx->IDR >> pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
		valorRead = (GPIOC->IDR>>13);
		valor = valorRead;
	}

	return 0;
}
