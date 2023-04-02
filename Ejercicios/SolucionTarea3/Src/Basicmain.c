/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Solucion Tarea 3
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


//Definimos elementos del tipo GPIO_Handler_t para los leds del display 7 segmentos.
GPIO_Handler_t handler_ledA = {0};
GPIO_Handler_t handler_ledB = {0};
GPIO_Handler_t handler_ledC = {0};
GPIO_Handler_t handler_ledD = {0};
GPIO_Handler_t handler_ledE = {0};
GPIO_Handler_t handler_ledF = {0};
GPIO_Handler_t handler_ledG = {0};
//Definimos elementos del tipo GPIO_Handler_t para la respectiva conexion de los catodos comunes
GPIO_Handler_t handler_commonD = {0};
GPIO_Handler_t handler_commonU = {0};

//Definimos elementos del tipo BasicTimer_Handler_t para el uso del TIMER 2 y 3
BasicTimer_Handler_t handlerUserTimer ={0};
BasicTimer_Handler_t handlermultiplexacion = {0};

//Arreglo que define los pines encendidos para la representacion de un numero en un display de 7 segmentos
uint8_t num[10] = {0b01111110, 0b00110000, 0b01101101, 0b01111001, 0b00110011,
		           0b01011011, 0b01011111, 0b01110000, 0b01111111, 0b01111011};

//Arreglo que define los pines encendidos para el modo culebrita
uint8_t cul[12] = {0b01000000, 0b11000000, 0b10000010, 0b10000100, 0b10001000, 0b00000100,
				   0b00000010, 0b10100000, 0b10010000, 0b00001000, 0b00010000, 0b00100000};

//Variables que representan la cantidad de decenas y unidades
uint8_t unidad = 0;
uint8_t decena = 0;

//Variable que representan la posicion de la secuencia en el modo culebrita
uint8_t secuencia = 0;

//Variable para el respectivo encendido de un display correspondiente a las decenas o unidades
uint8_t estadoCommon = 0;

//Variable que selecciona entre el modo numerico y culebrita a ejecutarse
uint8_t modo=0;

/*Definicion un elemento del tipo EXTI_Config_t y PIO_Handler_t para la interrupcion presente en PA9, la cual
 * esta conectada a la salida A del Encoder.
*/
GPIO_Handler_t handler_GPIOEncoA =  {0};
EXTI_Config_t handler_EXTIEncoA  = {0};

//Definimos elementos del tipo GPIO_Handler_t leer el estado de la salida B del Encoder
GPIO_Handler_t handler_EncoB = {0};

/*Definicion un elemento del tipo EXTI_Config_t y PIO_Handler_t para la interrupcion presente en PA0, la cual
 * esta conectada a la salida SW del Encoder.
*/
GPIO_Handler_t handler_GPIOEncoSW =  {0};
EXTI_Config_t handler_EXTIEncoSW  = {0};

//Definicion de las cabeceras de los dos tipos de modo a ejecutarse
void modo_numerico(void);
void modo_culebrita(void);
void mculebrita_EncendidoLed(void);

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

	//Definicion del pin para el LED LD2

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

	//Definicion de los pines para los leds del display de 7 segmentos

	//---------------PIN: PB5----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledA.pGPIOx = GPIOB;
		//Definimos el pin a utilizar
		handler_ledA.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledA, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledA);

		//---------------PIN: PB10----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledB.pGPIOx = GPIOB;
		//Definimos el pin a utilizar
		handler_ledB.GPIO_PinConfig.GPIO_PinNumber = PIN_10; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledB, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledB);

		//---------------PIN: PC0----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledC.pGPIOx = GPIOC;
		//Definimos el pin a utilizar
		handler_ledC.GPIO_PinConfig.GPIO_PinNumber = PIN_0; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledC, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledC);

		//---------------PIN: PC1----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledD.pGPIOx = GPIOC;
		//Definimos el pin a utilizar
		handler_ledD.GPIO_PinConfig.GPIO_PinNumber = PIN_1; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledD, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledD);

		//---------------PIN: PC2----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledE.pGPIOx = GPIOC;
		//Definimos el pin a utilizar
		handler_ledE.GPIO_PinConfig.GPIO_PinNumber = PIN_2; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledE, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledE);

		//---------------PIN: PB4----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledF.pGPIOx = GPIOB;
		//Definimos el pin a utilizar
		handler_ledF.GPIO_PinConfig.GPIO_PinNumber = PIN_4; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledF, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledF);

		//---------------PIN: PB0----------------
		//Definimos el periferico GPIOx a usar.
		handler_ledG.pGPIOx = GPIOB;
		//Definimos el pin a utilizar
		handler_ledG.GPIO_PinConfig.GPIO_PinNumber = PIN_0; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_ledG, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_ledG);

		//Definicion de los pines para la respectiva conexion de los catodos comunes

		//---------------PIN: PA4----------------
		//Definimos el periferico GPIOx a usar.
		handler_commonD.pGPIOx = GPIOA;
		//Definimos el pin a utilizar
		handler_commonD.GPIO_PinConfig.GPIO_PinNumber = PIN_4; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_commonD, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_commonD);

		//---------------PIN: PA1----------------
		//Definimos el periferico GPIOx a usar.
		handler_commonU.pGPIOx = GPIOA;
		//Definimos el pin a utilizar
		handler_commonU.GPIO_PinConfig.GPIO_PinNumber = PIN_1; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_commonU, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_commonU);

		//Definicion del Pin para la lectura del estado de la Salida B del Encoder

		//---------------PIN: PA8----------------

		//Definimos el periferico GPIOx a usar.
		handler_EncoB.pGPIOx = GPIOA;
		//Definimos el pin a utilizar
		handler_EncoB.GPIO_PinConfig.GPIO_PinNumber = PIN_8; 						//PIN_x, 0-15
		//Definimos la configuracion de los registro para el pin seleccionado
		// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
		GPIO_PIN_Config(&handler_EncoB, GPIO_MODE_IN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
		/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
		 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
		//Cargamos la configuracion del PIN especifico
		GPIO_Config(&handler_EncoB);

	//-------------------Fin de Configuracion GPIOx-----------------------


	//-------------------Inicio de Configuracion TIMx-----------------------

	//---------------TIM2------------------

	//Definimos el timer a usar
	handlerUserTimer.ptrTIMx = TIM2;
	//Definimos la configuracion del TIMER seleccionado
	handlerUserTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handlerUserTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handlerUserTimer.TIMx_Config.TIMX_period = 250;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handlerUserTimer);

	//---------------TIM3------------------

	//Definimos el timer a usar
	handlermultiplexacion.ptrTIMx = TIM3;
	//Definimos la configuracion del TIMER seleccionado
	handlermultiplexacion.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handlermultiplexacion.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handlermultiplexacion.TIMx_Config.TIMX_period = 10;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handlermultiplexacion);

	//-------------------Fin de Configuracion TIMx-----------------------


	//-------------------Inicio de Configuracion EXTIx -----------------------

	//---------------PIN: PC4----------------

	//Definimos el periferico GPIOx a usar.
	handler_GPIOEncoSW.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIOEncoSW.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTIEncoSW.pGPIOHandler = &handler_GPIOEncoSW;
	//Definimos el tipo de flanco
	handler_EXTIEncoSW.edgeType = EXTERNAL_INTERRUPP_FALLING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTIEncoSW);

	//---------------PIN: PA9----------------

	//Definimos el periferico GPIOx a usar.
	handler_GPIOEncoA.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIOEncoA.GPIO_PinConfig.GPIO_PinNumber = PIN_9;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTIEncoA.pGPIOHandler = &handler_GPIOEncoA;
	//Definimos el tipo de flanco
	handler_EXTIEncoA.edgeType = EXTERNAL_INTERRUPP_FALLING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTIEncoA);

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

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM3

void BasicTimer3_Callback(void)
{
	/*De acuerdo a la variable estadoCommon se prendera un display o el otro para mostrar
	 * el numero correspondiente a las unidades o decenas respectivamente*/

	if(estadoCommon == 0)
	{

		//Encendemos el display correspondiente a las unidades
		GPIO_writePin (&handler_commonD, 0);
		GPIO_writePin (&handler_commonU, 1);
		//Mostramos el numero correspondiente por el display
		GPIO_writePin (&handler_ledA, (num[unidad]>>6)&(0b1));
		GPIO_writePin (&handler_ledB, (num[unidad]>>5)&(0b1));
		GPIO_writePin (&handler_ledC, (num[unidad]>>4)&(0b1));
		GPIO_writePin (&handler_ledD, (num[unidad]>>3)&(0b1));
		GPIO_writePin (&handler_ledE, (num[unidad]>>2)&(0b1));
		GPIO_writePin (&handler_ledF, (num[unidad]>>1)&(0b1));
		GPIO_writePin (&handler_ledG, (num[unidad]>>0)&(0b1));
		estadoCommon = 1;
	}
	else if(estadoCommon == 1)
	{
		//Encendemos el display correspondiente a las decenas
		GPIO_writePin (&handler_commonU, 0);
		GPIO_writePin (&handler_commonD, 1);
		//Mostramos el numero correspondiente por el display
		GPIO_writePin (&handler_ledA, (num[decena]>>6)&(0b1));
		GPIO_writePin (&handler_ledB, (num[decena]>>5)&(0b1));
		GPIO_writePin (&handler_ledC, (num[decena]>>4)&(0b1));
		GPIO_writePin (&handler_ledD, (num[decena]>>3)&(0b1));
		GPIO_writePin (&handler_ledE, (num[decena]>>2)&(0b1));
		GPIO_writePin (&handler_ledF, (num[decena]>>1)&(0b1));
		GPIO_writePin (&handler_ledG, (num[decena]>>0)&(0b1));
		estadoCommon = 0;
	}
	else
	{
		__NOP();
	}
}

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el EXTI4
void callback_extInt4(void)
{

	if(modo==0)
	{
		//Desactivamos las interrupciones presentadas por el TIMER 3
		__disable_irq();
		handlermultiplexacion.ptrTIMx->DIER &= ~(TIM_DIER_UIE);
		__enable_irq();
		mculebrita_EncendidoLed();
		//Invertimos el valor de la variable modo
		modo = 1;
	}
	else if(modo==1)
	{
		//Activamos las interrupciones presentadas por el TIMER 3
		__disable_irq();
		handlermultiplexacion.ptrTIMx->DIER |= TIM_DIER_UIE;
		__enable_irq();
		//Invertimos el valor de la variable modo
		modo = 0;
	}
	else
	{
		__NOP();
	}
}

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el EXTI12

void callback_extInt9(void)
{
	//Deacuerdo al modo establecido se ejecutada la funcion correspondiente a dicho modo
	if(modo==0)
	{
		modo_numerico();
	}
	else if(modo==1)
	{
		modo_culebrita();
	}
	else
	{
		__NOP();
	}

}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------


//----------------------------Inicio de la definicion de las funciones auxiliares-------------------------------------

//Definimos la funcion que se ejecutada en el modo numerico
void modo_numerico(void)
{
	if(GPIO_RedPin (&handler_EncoB) == 1)
	{
		//Incremento numerico
		if(unidad<9)
		{
			unidad++;
		}
		else if(unidad == 9 && decena == 9)
		{
			__NOP();
		}
		else
		{
			unidad=0;
			decena++;
		}
	}
	else if (GPIO_RedPin (&handler_EncoB) == 0)
	{
		//Decremento numerico
		if(unidad>0)
		{
			unidad--;
		}
		else if(unidad == 0 && decena == 0)
		{
			__NOP();
		}
		else
		{
			unidad=9;
			decena--;
		}
	}
	else
	{
		__NOP();
	}
}

//Definimos la funcion que se ejecutada en el modo culebrita
void modo_culebrita(void)
{
	if(GPIO_RedPin (&handler_EncoB) == 1)
	{
		//Incremento de la secuencia
		if(secuencia<11)
		{
			secuencia++;
		}
		else
		{
			secuencia = 0;
		}
	}
	else if (GPIO_RedPin (&handler_EncoB) == 0)
	{
		//Decremento de la secuencia
		if(secuencia>0)
		{
			secuencia--;
		}
		else
		{
			secuencia = 11;
		}
	}
	else
	{
		__NOP();
	}
	mculebrita_EncendidoLed();
}

//Definimos la funcion de encendido de los leds para el modo culebrita
void mculebrita_EncendidoLed(void)
{

	//Encendemos el display correspondiente a la posicion de la secuencia dada
	GPIO_writePin (&handler_commonD, (cul[secuencia]>>7)&(0b1));
	GPIO_writePin (&handler_commonU, ~(cul[secuencia]>>7)&(0b1));
	//Encendemos un unico led deacuerdo a la posicion de la secuencia dada
	GPIO_writePin (&handler_ledA, (cul[secuencia]>>6)&(0b1));
	GPIO_writePin (&handler_ledB, (cul[secuencia]>>5)&(0b1));
	GPIO_writePin (&handler_ledC, (cul[secuencia]>>4)&(0b1));
	GPIO_writePin (&handler_ledD, (cul[secuencia]>>3)&(0b1));
	GPIO_writePin (&handler_ledE, (cul[secuencia]>>2)&(0b1));
	GPIO_writePin (&handler_ledF, (cul[secuencia]>>1)&(0b1));
	GPIO_writePin (&handler_ledG, (cul[secuencia]>>0)&(0b1));

}

//----------------------------Fin de la definicion de las funciones auxiliares-------------------------------------

