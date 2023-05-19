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
#include <USARTxDriver.h>
#include <SysTickDriver.h>
#include <PwmDriver.h>

//-----------------------------------Inicio de definicion de librerias------------------------------------------


//---------------------------Inicio de definicion de funciones y variables base----------------------------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
GPIO_Handler_t handler_BlinkyPin = {0};

//Definimos un elemento del tipo GPIO_Handler_t (Struct)
BasicTimer_Handler_t handler_BlinkyTimer ={0};

//Definimos la cabecera para la configuracion
void int_Hardware(void);
//---------------------------Fin de definicion de funciones y variables base----------------------------------

//---------Comunicacion Serial--------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB
GPIO_Handler_t handler_GPIO_USB = {0};
USART_Handler_t handler_USB = {0};
//Variable para guardar el char
uint8_t  letra = 'A';

//---------Motor Paso a Paso--------
//Definimos la cabecera para la funcion que controla el movimiento del motor
void Control_MPP(void);
//Definimos un elemento del tipo GPIO_Handler_t (Struct) como señal para girar el motor paso a paso
GPIO_Handler_t handler_GPIO_MPP = {0};

//---------Servomotor--------
//Definimos la cabecera para la funcion que controla el servo motor
void Control_Servo(uint8_t valor);
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y PWM_Handler_t para la señal PWM para el servomotor
GPIO_Handler_t handler_GPIO_Servo = {0};
PWM_Handler_t handler_PWM_Servo = {0};


int main(void)
{
	//Realizamos la configuracuion inicial
	int_Hardware();

	//Activamos el SysTick
	config_SysTick_ms(0);

	//Definimos par el PIN un 1 logico,
	GPIO_writePin (&handler_BlinkyPin, SET);

	while(1)
	{
		if (letra == '1')
		{
			Control_MPP();
			letra = 'A';
		}
		else if(letra == '2' || letra == '3')
		{
			Control_Servo(letra);
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
	handler_BlinkyPin.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_BlinkyPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_BlinkyPin, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_BlinkyPin);

	//---------------PIN: PA3----------------
	//------------AF7: USART2_RX----------------
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

	//---------------PIN: PB6----------------
	//------------AF2: TIM4_CH1----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Servo.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_Servo.GPIO_PinConfig.GPIO_PinNumber = PIN_6; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_Servo, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF2);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_Servo);

	//-------------------Fin de Configuracion GPIOx-----------------------

	//-------------------Inicio de Configuracion USARTx-----------------------

	//---------------USART2----------------
	//Definimos el periferico USARTx a utilizar
	handler_USB.ptrUSARTx = USART2;
	//Definimos la configuracion del USART seleccionado
	handler_USB.USART_Config.USART_mode = USART_MODE_RX ;           //USART_MODE_x  x-> TX, RX, RXTX, DISABLE
	handler_USB.USART_Config.USART_baudrate = USART_BAUDRATE_9600;  //USART_BAUDRATE_x  x->9600, 19200, 115200
	handler_USB.USART_Config.USART_parity= USART_PARITY_NONE;       //USART_PARITY_x   x->NONE, ODD, EVEN
	handler_USB.USART_Config.USART_stopbits=USART_STOPBIT_1;         //USART_STOPBIT_x  x->1, 0_5, 2, 1_5
	handler_USB.USART_Config.USART_enableIntRX = USART_RX_INTERRUP_ENABLE;   //USART_RX_INTERRUP_x  x-> DISABLE, ENABLE
	handler_USB.USART_Config.USART_enableIntTX = USART_TX_INTERRUP_DISABLE;   //USART_TX_INTERRUP_x  x-> DISABLE, ENABLE
	//Cargamos la configuracion del USART especifico
	USART_Config(&handler_USB);

	//-------------------Fin de Configuracion USARTx-----------------------

	//-------------------Inicio de Configuracion TIMx-----------------------

	//---------------TIM2----------------
	//Definimos el TIMx a usar
	handler_BlinkyTimer.ptrTIMx = TIM2;
	//Definimos la configuracion del TIMER seleccionado
	handler_BlinkyTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handler_BlinkyTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handler_BlinkyTimer.TIMx_Config.TIMX_period = 250;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handler_BlinkyTimer);


	//-------------------Fin de Configuracion TIMx-----------------------

	//-------------------Inicio de Configuracion PWM_Channelx----------------------

	//---------------TIM4_Channel_1----------------
	//Definimos el TIMx a usar
	handler_PWM_Servo.ptrTIMx = TIM4;
	//Definimos la configuracion para el PWM
	handler_PWM_Servo.config.periodcnt = BTIMER_PCNT_1us; //BTIMER_PCNT_xus x->1,10,100/ BTIMER_PCNT_1ms
	handler_PWM_Servo.config.periodo = 20000;             //Al definir 1us, 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	handler_PWM_Servo.config.channel = PWM_CHANNEL_1;     //PWM_CHANNEL_x x->1,2,3,4
	handler_PWM_Servo.config.duttyCicle = 5;             //Valor entre 0-100 [%]
	//Cargamos la configuracion
	pwm_Config(&handler_PWM_Servo);
	//Activar el TIMER y con ello el PWM
	startPwmSignal(&handler_PWM_Servo);

	//---------------------Fin de Configuracion PWM_Channelx-----------------------

}
//------------------------------Fin Configuracion del microcontrolador------------------------------------------


//----------------------------Inicio de la definicion de las funciones ISR---------------------------------------

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
//INTERMITENCIA DEL LED2
void BasicTimer2_Callback(void)
{
	GPIOxTooglePin(&handler_BlinkyPin);

}


void BasicUSART2_Callback(void)
{
	letra = getRxData();
}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------

//----------------------------Inicio de la definicion de las funciones ---------------------------------------

//Funcion para generar un paso del motor paso a paso
void Control_MPP(void)
{
	for(int u=1; u<21; u++)
	{
		GPIOxTooglePin(&handler_GPIO_MPP);
		delay_ms(20);
	}
}

//Funcion para control de servomotor
void Control_Servo(uint8_t dato)
{
	if(dato=='2')
	{
		updateDuttyCycle(&handler_PWM_Servo, 8);
	}
	else if(dato=='3')
	{
		updateDuttyCycle(&handler_PWM_Servo, 6);
	}
	else
	{
		__NOP();
	}

}
//----------------------------Fin de la definicion de las funciones ----------------------------------------
