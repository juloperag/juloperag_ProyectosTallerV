/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Configuracion basica proyecto
 ******************************************************************************
 */

//Definicion de librerias
#include <stdint.h>
#include <stm32f411xe.h>
#include <GPIOxDriver.h>
#include <BasicTimer.h>

//Definimos una variable de control
uint8_t estado=0;

int main(void)
{
	//-------------------Inicio de Configuracion GPIOx-----------------------
	//Definimos un elemento del tipo GPIO_Handler_t (Struct)
	GPIO_Handler_t handlerUserPin = {0};

	//---------------PIN: PA5----------------
	//Definimos el periferico GPIOx a usar.
	handlerUserPin.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handlerUserPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handlerUserPin, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handlerUserPin);

	//-------------------Fin de Configuracion GPIOx-----------------------

	//-------------------Inicio de Configuracion TIMx-----------------------
	//Definimos un elemento del tipo GPIO_Handler_t (Struct)
	BasicTimer_Handler_t handlerUserTimer ={0};

	//Definimos el timer a usar
	handlerUserTimer.ptrTIMx = TIM2;
	//Definimos la configuracion del TIMER seleccionado
	handlerUserTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handlerUserTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handlerUserTimer.TIMx_Config.TIMX_period = 2000;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handlerUserTimer);

	//-------------------Fin de Configuracion TIMx-----------------------


	//Definimos para el PIN un 1 logico,
	GPIO_writePin (&handlerUserPin, SET);
	while(1)
	{
		if(estado==1)
		{
			GPIOxTooglePin(&handlerUserPin);
			estado = 0;
		}
		else
		{
			__NOP();
		}
	}

	return 0;
}

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
void BasicTimer_Callback(void)
{
	estado=1;
}
