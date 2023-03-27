/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Configuracion Basica Proyecto
 ******************************************************************************
 */

//----------------------------------Inicio de definicion de librerias-------------------------------------------
#include <stdint.h>
#include <stm32f411xe.h>
#include <GPIOxDriver.h>
#include <BasicTimer.h>
#include <ExtiDriver.h>
//-----------------------------------Inicio de definicion de librerias------------------------------------------


//---------------------------Inicio de definicion de funciones y variables base----------------------------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
GPIO_Handler_t handler_led2 = {0};

//Definimos la cabecera para la configuracion
void int_Hardware(void);
//---------------------------Fin de definicion de funciones y variables base----------------------------------

//Definicion un elemento del tipo EXTI_Config_t para el user boton
EXTI_Config_t handler_Button ={0};

//Definimos una variable para contar
uint32_t counterExti13=0;

int main(void)
{
	//Realizamos la configuracuion inicial
	int_Hardware();

	//Definimos para el PIN un 1 logico,
	GPIO_writePin (&handler_led2, SET);
	while(1)
	{
		__NOP();
	}

	return 0;
}



//------------------------------Inicio Configuracion del microcontrolador------------------------------------------
void int_Hardware(void)
{

	//-------------------Inicio de Configuracion GPIOx-----------------------

	//---------------PIN: PA5----------------
	//Definimos el periferico GPIOx a usar.
	handler_led2.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_led2.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_led2, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_led2);

	//---------------PIN: PC13----------------
	handler_Button.pGPIOHandler = 0;
	handler_Button.pGPIOHandler->pGPIOx = 0;
	//Definimos el periferico GPIOx a usar.
	handler_Button.pGPIOHandler->pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_Button.pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber = PIN_13; 				//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(handler_Button.pGPIOHandler, GPIO_MODE_IN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(handler_Button.pGPIOHandler);

	//-------------------Fin de Configuracion GPIOx-----------------------


	//-------------------Inicio de Configuracion TIMx-----------------------
	//Definimos un elemento del tipo GPIO_Handler_t (Struct)
	BasicTimer_Handler_t handlerUserTimer ={0};
	//Definimos el timer a usar
	handlerUserTimer.ptrTIMx = TIM2;
	//Definimos la configuracion del TIMER seleccionado
	handlerUserTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handlerUserTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handlerUserTimer.TIMx_Config.TIMX_period = 250;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handlerUserTimer);

	//-------------------Fin de Configuracion TIMx-----------------------


	//-------------------Inicio de Configuracion EXTIx -----------------------
	//Definimos el tipo de flanco
	handler_Button.edgeType = EXTERNAL_INTERRUPP_FALLING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_Button);

	//-------------------Fin de Configuracion EXTIx-----------------------

}
//------------------------------Fin Configuracion del microcontrolador------------------------------------------



//----------------------------Inicio de la definicion de las funciones ISR---------------------------------------

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
//INTERMITENCIA DEL LED2
void BasicTimer2_Callback(void)
{
	GPIOxTooglePin(&handler_led2);
}

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el EXTI13

void callback_extInt13(void)
{
	counterExti13++;
}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------
