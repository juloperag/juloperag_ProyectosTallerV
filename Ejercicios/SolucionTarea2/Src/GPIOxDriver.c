/**
 ******************************************************************************
 * @file           : GIOPxDriver.c
 * @author         : juloperag
 * @brief          : Driver GPIOx
 ******************************************************************************
 */

/* Se desarrolla el programa para que sea lo mas generico posible
 * para identificar el puerto GPIO y el pin seleccionado
 */

#include <GPIOxDriver.h>

//Activacion de la señal de reloj de un elemento en especifico

void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
{
	//variables para hacer todo paso a paso
	uint32_t auxConfig = 0;
	uint32_t auxPosition = 0;

	//----------------------1) Activamos el periferico-----------------------------

	//Verificamos para GIOPA
	if (pGPIOHandler->pGIOx == GPIOA)
	{
		//Escribimos 1 (SET) en la posicion correspondiente al GPIOA
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOA_EN);
		/*Modificamos el registro AHB1ENR(32 bit)presente en periferico RCC, vease el 6.3.9 RCC_AHB1ENR---AHB1
		peripheral clock enable register*/
	}

	//Verificamos para GIOPB
	else if (pGPIOHandler->pGIOx == GPIOB)
	{
		//Escribimos 1 (SET) en la posicion correspondiente al GPIOB
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOB_EN);
		/*Modificamos el registro AHB1ENR(32 bit)presente en periferico RCC, vease el 6.3.9 RCC_AHB1ENR---AHB1
		peripheral clock enable register*/
	}

	//Verificamos para GIOPC
	else if (pGPIOHandler->pGIOx == GPIOC)
	{
		//Escribimos 1 (SET) en la posicion correspondiente al GPIOC
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOC_EN);
		/*Modificamos el registro AHB1ENR(32 bit)presente en periferico RCC, vease el 6.3.9 RCC_AHB1ENR---AHB1
		peripheral clock enable register*/
	}

	//Verificamos para GIOPD
	else if (pGPIOHandler->pGIOx == GPIOD)
	{
		//Escribimos 1 (SET) en la posicion correspondiente al GPIOD
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOD_EN);
		/*Modificamos el registro AHB1ENR(32 bit)presente en periferico RCC, vease el 6.3.9 RCC_AHB1ENR---AHB1
		peripheral clock enable register*/
	}

	//Verificamos para GIOPE
	else if (pGPIOHandler->pGIOx == GPIOE)
	{
		//Escribimos 1 (SET) en la posicion correspondiente al GPIOE
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOE_EN);
		/*Modificamos el registro AHB1ENR(32 bit)presente en periferico RCC, vease el 6.3.9 RCC_AHB1ENR---AHB1
		peripheral clock enable register*/
	}

	//Verificamos para GIOPH
	else if (pGPIOHandler->pGIOx == GPIOH)
	{
		//Escribimos 1 (SET) en la posicion correspondiente al GPIOH
		RCC->AHB1ENR |= (SET << RCC_AHB1ENR_GPIOH_EN);
		/*Modificamos el registro AHB1ENR(32 bit)presente en periferico RCC, vease el 6.3.9 RCC_AHB1ENR---AHB1
		peripheral clock enable register*/
	}

	//----------------2) Configurando el registro GPIOx_MODER----------------------
	//SE ENTIENDE

	//Leemos el PinNumber para mover una cantidad de veces  el pinModer
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinModer << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Cargamos auxConfig en el registro MODER
	pGPIOHandler->pGIOx->MODER &= ~(0b11 << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandler->pGIOx->MODER |= auxConfig;

	//----------------3) Configurando el registro GPIOx_OTYPER----------------------
	//SE ENTIENDE

	//Leemos el PinNumber para mover una cantidad de veces el pinOTPype
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinOTPype << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Cargamos  auxConfig en el registro OTYPER
	pGPIOHandler->pGIOx->OTYPER &= ~(SET << pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandler->pGIOx->OTYPER |= auxConfig;

	//----------------4) Configurando el registro GPIOx_OSPEEDR----------------------
	//SE ENTIENDE

	//Leemos el PinNumber para mover una cantidad de veces el pinSpeed
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Cargamos  auxConfig en el registro OSPEEDR
	pGPIOHandler->pGIOx->OSPEEDR &= ~(0b11 << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandler->pGIOx->OSPEEDR |= auxConfig;

	//----------------5) Configurando el registro GPIOx_PUPDR----------------------
	//SE ENTIENDE

	//Leemos el PinNumber para mover una cantidad de veces el pinSpeed
	auxConfig = (pGPIOHandler->GPIO_PinConfig.GPIO_PinPUPdControl << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	//Cargamos  auxConfig en el registro PUPDR
	pGPIOHandler->pGIOx->PUPDR &= ~(0b11 << 2*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandler->pGIOx->PUPDR|= auxConfig;

	//---------------- MODER: Configurando funciones alternativas----------------------

	//si el Modo de configuracion escogido es GPIO_MODE_ALTFN, entonces activamos el AF
	if(pGPIOHandler->GPIO_PinConfig.GPIO_PinModer == GPIO_MODE_ALTFN)
	{
		//Para los pines 0 a 8 escogemos el AFRL
		if (pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber < 8)
		{
			auxPosition = 4*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber;

			//Cargamos auxPosition en el registro AFRL
			pGPIOHandler->pGIOx->AFRL = ~(0b1111 >> auxPosition);
			pGPIOHandler->pGIOx->AFRL = (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode >> auxPosition);
		}

		//Para los pines 8 a 15 escogemos el AFRH
		else
		{
			auxPosition = 4*pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber;

			//Cargamos auxPosition en el registro AFRL
			pGPIOHandler->pGIOx->AFRH = ~(0b1111 >> auxPosition);
			pGPIOHandler->pGIOx->AFRH = (pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode >> auxPosition);
		}
	}
}

//---------------- MODER: Configurando Output: Registro BSRR----------------------

void GPIO_writePin (GPIO_Handler_t *pPinHandler, uint8_t newState)
{
	//Limpiamos la posicion que deseamos
	//pPinHandler->pGIOx->ODR &= ~(SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
	if (newState == SET)
	{
		//Trabajamos con la parte baja del registro
		pPinHandler->pGIOx->BSRR |= (SET<<pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
	}
	else
	{
		//Trabajamos con la parte alta del registro
		pPinHandler->pGIOx->BSRR |= (SET<<(pPinHandler->GPIO_PinConfig.GPIO_PinNumber+16));
	}
}

//---------------- MODER: Lectura input----------------------

uint32_t GPIO_RedPin (GPIO_Handler_t *pPinHandler)
{
	//Creamos una variable auxiliar
	uint32_t pinValue = 0;

	/*Cargamos el valor del registro IDR y lo desplazamos a la posicion
	 * correspondiente
	 */
	pinValue = (pPinHandler->pGIOx->IDR >> pPinHandler->GPIO_PinConfig.GPIO_PinNumber)&(0b1);

	return pinValue;
}


//---------------- Toogle ---------------------
void GPIOxTooglePin(GPIO_Handler_t *pPinHandler)
{
	pPinHandler->pGIOx->ODR  ^= (SET << pPinHandler->GPIO_PinConfig.GPIO_PinNumber);
}

