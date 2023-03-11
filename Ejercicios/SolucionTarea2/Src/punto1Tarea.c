/**
 ******************************************************************************
 * @file           : punto1Tarea.c
 * @author         : juloperag
 * @brief          : Punto 1. Tarea 2
 ******************************************************************************
 */

//--------------------------Explicacion--------------------------
/*
 *a) ¿Cúal es este error?
 *  Cuando nosotros definimos la variabale pinValue como
 *  (pPinHandler->pGIOx->IDR >> pPinHandler->GPIO_PinConfig.GPIO_PinNumber)
 *	estamos leyendo el registro IDR del GPIO seleccionado y ademas el numero binario leido lo estamos
 *	moviendo una cantidad de espacios a la derecha equivalentes al numero del pin donde deseamos leer su valor.
 *	Por ejemplo el numero del pin que deseas leer es el 5 y sabemos que dicho valor logico sera el 1, ahora en
 *	el registro IDR se almacenada el 0b100000, pero solo nos interesa conocer el valor del PIN_5 por lo cual
 *	desplazamos a la derecha 5 veces dicho numero binario, estableciendo  en la variable pinValue 0b1.
 *	Pero que pasaria si a parte del PIN_5 tambien se tiene como entrada el PIN_7 y por el se establece un 1
 *	logico tambien, pues el valor almacenado en el registro IDR sera 0b10100000, por lo cual al realizar el
 *	desplazamiento bit a bit el valor registrado en la variable pinValue sera 0b101.Por lo tanto no podriamos
 *	establecer el valor real del Pin que deseamos leer.
 *
 *b) ¿Cómo lo puede solucionar?
 *	Este error en la lectura de un PIN en especifico se puede solucionar aplicando una mascara a la variable
 *	pinValue de la siguiente forma:
 *	pinValue = (pPinHandler->pGIOx->IDR >> pPinHandler->GPIO_PinConfig.GPIO_PinNumber)&(0b1);
 *	Con dicha mascara por ejemplo al tener el valor 0b101 luego de leer el registro IDR y aplicarle el
 *	desplazamiento bit a bit, se definida un valor para pinValue de 0b1, ya que al definir una operacion bitwise
 *	AND los valores presentes en las posiciones mayores de 0 se volveran nulos, quedandonos solo con la posicion 0 del
 *	numero binario, correspondiente al valor del Pin que se desea leer.
 */
//---------------------------------------------------------------

//------------------Ejercicio------------------------------------
/*
 * Para comprobar la modificacion realizada vamos a establecer como entradas el PIN_15 y PIN_13 del periferico
 * GPIO_C. Para el PIN_15 vamos a activar el PULL_UP resistor con fin de establecer un 1 logico, mientras que el PIN_13
 * nos servira de tester ya que este esta conectado al Push-button B1, con el cual modificaremos el valor de dicho Pin.
 */
//-----------------------------------------------------------------

//Incluye librerias
#include <stdint.h>
#include <Stm32f4xx.h>
#include <GPIOxDriver.h>

uint32_t valorRead = 0;
uint32_t valor = 0;

int main(void)
{

	//-------------------Inicio de Configuracion GPIOx-----------------------

	GPIO_Handler_t handlerUserPin = {0};
	/*Utilizamos la estrutura GPIOx_RegDef_t (nombrada como pGIOx en la estrutura GPIO_Handler_t)
	 * con la posicion de memoria del GPIOx selecionado.
	 * Vease Stm32f4xx.h :
	 * #define GPIOx ((GPIOx_RegDef_t *) GPIOx_BASE_ADDR) / Ej #define GPIOA_BASE_ADDR (AHB1_BASE_ADDR + 0x0000U)
	 */
	handlerUserPin.pGIOx = GPIOC;
	//Definimos el pin  a utilizar

	//----------------------------PIN_15
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_15; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_IN; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_PULLUP; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15
	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);

	//----------------------------PIN_13
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_13; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para un pin selecionado
	handlerUserPin.GPIO_PinConfig.GPIO_PinModer 		= GPIO_MODE_IN; 		//GPIO_MODE_x--->IN, OUT, ALTFN, ANALOG
	handlerUserPin.GPIO_PinConfig.GPIO_PinPUPdControl 	= GPIO_PUPDR_NOTHING; 	//GPIO_PUPDR_x ---> NOTHING, PULLUP, PULLDOWN, RESERVED
	handlerUserPin.GPIO_PinConfig.GPIO_PinAltFunMode 	= AF0;					//AFx, 0-15
	/*Cargamos la configuracion del PIN especifico
	 * Vease: void GPIO_Config (GPIO_Handler_t *pGPIOHandler)
	 */
	GPIO_Config (&handlerUserPin);
	//-------------------Fin de Configuracion GPIOx-----------------------

	//Ciclo
	while(1)
	{
		valorRead = GPIO_RedPin(&handlerUserPin);
		valor = valorRead;
	}

	return 0;
}
