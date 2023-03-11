/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Configuracion de los GPIO
 ******************************************************************************
 */

#include <stdint.h>
#include <Stm32f4xx.h>

int main(void)
{

	//Configuracion inicial del MCU
	RCC->AHB1ENR &= ~(1<<0); 	//Borrar la posicion sea cual sea del AHB1ENR
	RCC->AHB1ENR |= 1<<0; 		//Activando el ciclo de relog en el AHB1

	//COnfiguracion MODER
	GPIOA->MODER &= ~(0b11<<10); //Limpiando la posicion presente en el pin 5
	GPIOA->MODER |= (0b01<<10); //Establecemos el pin 5 como salida

	//Configuracion OTYDER
	GPIOA->OTYPER &= ~(0b1<<5); //configuramos el pin 5 como salida

	//configuracion OPSEDD

	GPIOA->OSPEEDR &= ~(0b11<<10); //Limpiando la posicion presente en el pin 5
	GPIOA->OSPEEDR |= (0b01<<10); //Establecemos la velocidad de escritura del pin 5

	//Escribimos un 1 en la salida
	GPIOA->ODR	&= ~(0b1<<5); //limpiamos la poscion 5 del registro ODR
	GPIOA->ODR	|= (0b1<<5); //Establecemos un 1 en el pin 5

	while(1)
	{

	}

	return 0;
}
