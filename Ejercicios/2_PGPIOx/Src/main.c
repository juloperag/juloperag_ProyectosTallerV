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
	RCC->AHB1ENR &= ~(1<<0); 	//Borrar la posicion sea cual sea del AHB1ENR
	RCC->AHB1ENR |= 1<<0; 		//Activando el ciclo de relog en el AHB1

	//COnfiguracion MODER
	GPIOC->MODER &= ~(0b11<<10); //Limpiando la posicion presente en el pin 5
	GPIOC->MODER |= (0b00<<10); //Establecemos el pin 5 como salida

	//Configuracion OTYDER
	GPIOC->OTYPER &= ~(0b1<<5); //configuramos el pin 5 como salida

	//configuracion OPSEDD

	GPIOC->OSPEEDR &= ~(0b11<<10); //Limpiando la posicion presente en el pin 5
	GPIOC->OSPEEDR |= (0b01<<10); //Establecemos la velocidad de escritura del pin 5

	GPIOC->ODR &= ~(0b1<<5); //Limpiamos
	GPIOC->ODR |= (0b1<<5); 	//Encendemos el led

	while(1)
	{

	}

	return 0;
}
