/**
 ******************************************************************************
 * @file           : punto3Tarea.c
 * @author         : juloperag
 * @brief          : Punto 3. Tarea 2
 ******************************************************************************
 */

//Incluye librerias
#include <stdint.h>
#include <Stm32f4xx.h>
#include <GPIOxDriver.h>

/*Definicion de arreglos que contiene la configuracion de los pines de acuerdo a su posicion
 * para representar numeros binarios: PC9, PC6, PB8, PA6, PC7, PC8, PA7
 */
uint8_t gpio[7] = {3, 3, 2, 1, 3, 3, 1};
uint8_t pin_led[7] = {PIN_9, PIN_6, PIN_8, PIN_6, PIN_7, PIN_8, PIN_7};

//Definicion de variables
uint32_t tiempo = 16000000;
uint8_t contador = 2;

//variables para la conversion de decimal a binario
uint8_t residuo[8] = {0,0,0,0,0,0,0,0};
uint8_t dec_bin[8] = {0,0,0,0,0,0,0,0};
uint8_t con = 0;

//variable que guarda el del Valor Boton
uint8_t valor = 0;

int main(void)
{
	//-----------------------------------Inicio de Configuracion GPIOx-----------------------------------------------

	GPIO_Handler_t handlerUserPin = {0};

	//------------------Configuracion PIN PC9-------------------------------
	//Definimos el GPIO a utilizar
	handlerUserPin.pGIOx = GPIOC;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_9; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_OUT; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinOTPype 		= GPIO_OTYPER_PUSHPULL;	//GPIO_OTYPER_x---> PUSHPULL, OPENDRAIN
	handlerUserPin.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_MEDIUM;	//GPIO_OSPEEDR_x---> LOW, MEDIUM, FAST, HIGH
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//------------------Configuracion PIN PC6-------------------------------
	//Definimos el GPIO a utilizar
	handlerUserPin.pGIOx = GPIOC;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_6; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_OUT; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinOTPype 		= GPIO_OTYPER_PUSHPULL;	//GPIO_OTYPER_x---> PUSHPULL, OPENDRAIN
	handlerUserPin.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_MEDIUM;	//GPIO_OSPEEDR_x---> LOW, MEDIUM, FAST, HIGH
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//------------------Configuracion PIN PB8-------------------------------
	//Definimos el GPIO a utilizar
	handlerUserPin.pGIOx = GPIOB;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_8; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_OUT; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinOTPype 		= GPIO_OTYPER_PUSHPULL;	//GPIO_OTYPER_x---> PUSHPULL, OPENDRAIN
	handlerUserPin.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_MEDIUM;	//GPIO_OSPEEDR_x---> LOW, MEDIUM, FAST, HIGH
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//------------------Configuracion PIN PA6-------------------------------
	//Definimos el GPIO a utilizar
	handlerUserPin.pGIOx = GPIOA;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_6; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_OUT; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinOTPype 		= GPIO_OTYPER_PUSHPULL;	//GPIO_OTYPER_x---> PUSHPULL, OPENDRAIN
	handlerUserPin.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_MEDIUM;	//GPIO_OSPEEDR_x---> LOW, MEDIUM, FAST, HIGH
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//------------------Configuracion PIN PC7-------------------------------
	//Definimos el GPIO a utilizar
	handlerUserPin.pGIOx = GPIOC;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_7; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_OUT; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinOTPype 		= GPIO_OTYPER_PUSHPULL;	//GPIO_OTYPER_x---> PUSHPULL, OPENDRAIN
	handlerUserPin.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_MEDIUM;	//GPIO_OSPEEDR_x---> LOW, MEDIUM, FAST, HIGH
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//------------------Configuracion PIN PC8-------------------------------
	//Definimos el GPIO a utilizar
	handlerUserPin.pGIOx = GPIOC;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_8; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_OUT; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinOTPype 		= GPIO_OTYPER_PUSHPULL;	//GPIO_OTYPER_x---> PUSHPULL, OPENDRAIN
	handlerUserPin.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_MEDIUM;	//GPIO_OSPEEDR_x---> LOW, MEDIUM, FAST, HIGH
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//------------------Configuracion PIN PA7-------------------------------
	//Definimos el GPIO a utilizar
	handlerUserPin.pGIOx = GPIOA;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_7; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_OUT; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinOTPype 		= GPIO_OTYPER_PUSHPULL;	//GPIO_OTYPER_x---> PUSHPULL, OPENDRAIN
	handlerUserPin.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_MEDIUM;	//GPIO_OSPEEDR_x---> LOW, MEDIUM, FAST, HIGH
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//------------------Configuracion PIN PC13-------------------------------
	//Definimos el GPIO a utilizar
	handlerUserPin.pGIOx = GPIOC;
	//Definimos el pin  a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_13; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_IN; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15

	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//--------------------------------------Fin de Configuracion GPIOx------------------------------------------------


	while(1)
	{
		//---------------------------Conversion decimal a binario---------------------
		int8_t n = 0;
		uint8_t i = 0;
		con = contador;
		if(con == 0)
		{
			for(uint8_t e=0;e<8;e++)
			{
				dec_bin[e] = 0;
			}
		}
		//Realizamos un procedimiento para obtener el residuo de la variable contador
		while (con != 0)
		{

			residuo[n] = con%2;
			con /= 2;
			n++;
		}
		//invertimos los elementos del arreglo "residuo" para obtener la forma binario de la variable contador
		n=7;
		while(n>=0)
		{
			dec_bin[i] = residuo[n];
			n--;
			i++;
		}

		//-----------------------Estado de salida de los Pines establecidos------------------------

		//Definimos un 1 o 0 en los pines configurados de acuerdo al arreglo "dec_bin"
		for(uint8_t e=0;e<7;e++)
		{
			if(gpio[e]==1)
			{
				handlerUserPin.pGIOx = GPIOA;
			}
			else if (gpio[e]==2)
			{
				handlerUserPin.pGIOx = GPIOB;
			}
			else if(gpio[e]==3)
			{
				handlerUserPin.pGIOx = GPIOC;
			}

			handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = pin_led[e];

			if (dec_bin[(e+1)]==1)
			{
				GPIO_writePin (&handlerUserPin, 1);
			}
			else
			{
				GPIO_writePin (&handlerUserPin, 0);
			}

		}


		//Ciclo for para generar un delay

		for(uint32_t e;e<tiempo;e++)
		{
		}

		//------------------Incremento o decremento de la variable contador---------------------

		//Leemos el valor del PC13 al cual esta conectado al Boton
		handlerUserPin.pGIOx = GPIOC;
		handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_13;
		valor = GPIO_RedPin (&handlerUserPin);

		//Incrementador. En caso de llega a 60 se reinicia en 1
		if (valor == 1)
		{
			if(contador<60)
			{
				contador++;
			}
			else
			{
				contador = 1;
			}
		}
		//Decrementador. En caso de llega a 1 se reinicia en 60
		else
		{
			if(contador>1)
			{
				contador--;
			}
			else
			{
				contador = 60;
			}
		}
	}

	return 0;
}
