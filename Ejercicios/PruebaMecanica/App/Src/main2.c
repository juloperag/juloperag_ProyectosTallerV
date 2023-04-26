/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Prueba de USART
 ******************************************************************************
 */

//----------------------------------Inicio de definicion de librerias-------------------------------------------
#include <stdint.h>
#include <stm32f411xe.h>
#include <GPIOxDriver.h>
#include <BasicTimer.h>
#include "USARTxDriver.h"
//-----------------------------------Inicio de definicion de librerias------------------------------------------


//---------------------------Inicio de definicion de funciones y variables base----------------------------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
GPIO_Handler_t handler_led2 = {0};

//Definimos un elemento del tipo GPIO_Handler_t (Struct)
BasicTimer_Handler_t handlerUserTimer ={0};

//Definimos la cabecera para la configuracion
void int_Hardware(void);
//---------------------------Fin de definicion de funciones y variables base----------------------------------

//---------Comunicacion Serial--------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para transmitir datos por USB
GPIO_Handler_t handler_GPIO_USB = {0};
//Definicion un elemento del tipo USART_Handler_t para transmitir datos por USB
USART_Handler_t handler_USB = {0};
//Variable para guardar el char
uint8_t  letra = 'A';

//---------Motor Paso a Paso--------
//Definimos la cabecera para la funcion que controla el movimiento del motor
void Control_MPP(void);
//Definimos un elemento del tipo GPIO_Handler_t (Struct) como señal para girar el motor paso a paso
GPIO_Handler_t handler_GPIO_MPP = {0};
//Variable para la pausa del cambio de estado del motor
uint8_t pausa = 1;
//Defino una variable para contar tiempo
uint8_t  contador = 1;

//---------Servomotor--------
//Definimos la cabecera para la funcion que controla el servo motor
void Control_Servo(uint8_t valor);
//Definimos un elemento del tipo GPIO_Handler_t (Struct) como señal para girar al servomotor
GPIO_Handler_t handler_GPIO_SM = {0};
//Definimos un elemento del tipo GPIO_Handler_t (Struct)
BasicTimer_Handler_t handlerSeñalServo ={0};
//Variables para establecer el estado alto o bajo de la señal del servomotor
uint16_t contiempo = 0;
uint8_t estadoSS = 1;
//Variable para definir el multiplo correspondiente a los 18°
uint8_t mulgrado=0;

int main(void)
{
	//Realizamos la configuracuion inicial
	int_Hardware();

	//Definimos par el PIN un 1 logico,
	GPIO_writePin (&handler_led2, SET);

	while(1)
	{
		if (letra == '1')
		{
			Control_MPP();
			letra = 'A';
		}
		else if(letra == '2')
		{
			mulgrado = 0;
			letra = 'A';
		}
		else if(letra == '3')
		{
			mulgrado = (50/18);
			letra = 'A';
		}
		else
		{
			__NOP();
		}
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

	//---------------PIN: PA3----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_USB.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIO_USB.GPIO_PinConfig.GPIO_PinNumber = PIN_3; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_USB, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF7);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_USB);

	//---------------PIN: PA9---------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_MPP.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIO_MPP.GPIO_PinConfig.GPIO_PinNumber = PIN_9; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_MPP, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_MPP);

	//---------------PIN: PC7---------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_SM.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIO_SM.GPIO_PinConfig.GPIO_PinNumber = PIN_7; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_SM, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_SM);

	//-------------------Fin de Configuracion GPIOx-----------------------

	//-------------------Inicio de Configuracion USARTx-----------------------

	//---------------USART2----------------
	//Definimos el periferico USARTx a utilizar
	handler_USB.ptrUSARTx = USART2;
	//Definimos la configuracion del USART seleccionado
	handler_USB.USART_Config.USART_mode = USART_MODE_RX ;           //USART_MODE_x  x-> TX, RX, RXTX, DISABLE
	handler_USB.USART_Config.USART_baudrate = USART_BAUDRATE_9600;  //USART_BAUDRATE_x  x->9600, 19200, 115200
	handler_USB.USART_Config.USART_parity= USART_PARITY_ODD;       //USART_PARITY_x   x->NONE, ODD, EVEN
	handler_USB.USART_Config.USART_stopbits=USART_STOPBIT_1_5;         //USART_STOPBIT_x  x->1, 0_5, 2, 1_5
	//Cargamos la configuracion del USART especifico
	USART_Config(&handler_USB);

	//-------------------Fin de Configuracion USARTx-----------------------

	//-------------------Inicio de Configuracion TIMx-----------------------

	//---------------TIM2----------------
	//Definimos el TIMx a usar
	handlerUserTimer.ptrTIMx = TIM2;
	//Definimos la configuracion del TIMER seleccionado
	handlerUserTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handlerUserTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handlerUserTimer.TIMx_Config.TIMX_period = 250;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handlerUserTimer);

	//---------------TIM3----------------
	//Definimos el TIMx a usar
	handlerSeñalServo.ptrTIMx = TIM3;
	//Definimos la configuracion del TIMER seleccionado
	handlerSeñalServo.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_10us; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handlerSeñalServo.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handlerSeñalServo.TIMx_Config.TIMX_period = 10;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handlerSeñalServo);

	//-------------------Fin de Configuracion TIMx-----------------------
}
//------------------------------Fin Configuracion del microcontrolador------------------------------------------


//----------------------------Inicio de la definicion de las funciones ISR---------------------------------------

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
//INTERMITENCIA DEL LED2
void BasicTimer2_Callback(void)
{
	GPIOxTooglePin(&handler_led2);
	//La interrupcion se activa a los 250ms por tanto el contador = 1 equivale a 250ms
	if(contador == 1)
	{
		pausa = 0;
		contador = 1;
	}
	else
	{
		contador++;
	}
}

void BasicTimer3_Callback(void)
{
	//Nota:La interrupcion se genera cada 100us, con dicho tiempo podemos desplazarnos cada 18°
	if(contiempo == (100+10*mulgrado) && estadoSS == 1)
	{
		Control_Servo(estadoSS);
		contiempo = 0;
		estadoSS = 0;
	}
	else if(contiempo == (2000-(100+10*mulgrado)) && estadoSS == 0)
	{
		Control_Servo(estadoSS);
		contiempo = 0;
		estadoSS = 1;
	}
	else
	{
		contiempo++;
	}

}

void BasicUSART2_Callback(char data)
{
	letra = data;
}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------

//----------------------------Inicio de la definicion de las funciones ---------------------------------------

//Funcion para control de motor paso a paso
void Control_MPP(void)
{
	for(int u=0; u<11; u++)
	{
		pausa = 1;
		GPIOxTooglePin(&handler_GPIO_MPP);
		while(pausa)
		{
			__NOP();
		}
	}
}

//Funcion para control de servomotor
void Control_Servo(uint8_t valor)
{
	switch(valor)
	{
	case 1:
	{
		GPIO_writePin (&handler_GPIO_SM, SET);
		break;
	}
	case 0:
	{
		GPIO_writePin (&handler_GPIO_SM, RESET);
		break;
	}
	default:
	{
		__NOP();
		break;
	}
	}
}
//----------------------------Fin de la definicion de las funciones ----------------------------------------
