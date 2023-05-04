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
#include <stdio.h>
#include <GPIOxDriver.h>
#include <BasicTimer.h>
#include <ExtiDriver.h>
#include <USARTxDriver.h>
#include <SysTickDriver.h>
#include <PwmDriver.h>

//-----------------------------------Fin de definicion de librerias------------------------------------------


//---------------------------Inicio de definicion de funciones y variables base----------------------------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
GPIO_Handler_t handler_BlinkyPin = {0};

//Definimos un elemento del tipo GPIO_Handler_t (Struct)
BasicTimer_Handler_t handler_BlinkyTimer ={0};

//Definimos la cabecera para la configuracion
void int_Hardware(void);
//---------------------------Fin de definicion de funciones y variables base----------------------------------


//--------------------------EXTI-------------------------------
//Definicion un elemento del tipo EXTI_Config_t y PIO_Handler_t para el user boton
GPIO_Handler_t handler_GPIOButton = {0};
EXTI_Config_t handler_EXTIButton ={0};

//--------------------------USART-------------------------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB
GPIO_Handler_t handler_GPIO_USB = {0};
USART_Handler_t handler_USB = {0};
//Variable que regula el tiempo de impresion
uint8_t conMg = 1;
//Definimos string
char sendMg[] = "Boton presionado \n";
char bufferMsg[64] = {0};

//--------------------------PWM-------------------------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y PWM_Handler_t para el uso del PWM
GPIO_Handler_t handler_GPIO_PWM = {0};
PWM_Handler_t handler_PWM = {0};
//Variables para cambiar el duttycicle
uint8_t duttyporc = 10;
uint8_t estado = 0;

int main(void)
{
	//Realizamos la configuracuion inicial
	int_Hardware();

	//Activamos el SysTick
	config_SysTick_ms(0);

	//Definimos para el PIN un 1 logico,
	GPIO_writePin (&handler_BlinkyPin, SET);

	while(1)
	{
		if(conMg>4)
		{
			//Definimos el tiempo que a pasado desde que el programa empezo a ejecutarse
			uint32_t tiempo = (uint32_t) getTicksMs();
			//ese tiempo lo volvemos un string
			sprintf(bufferMsg,"Tiempo Transcurrido: %lu ms \n",tiempo);
			//Enviamos por puerto serial dicho string
			writeMsg(&handler_USB, bufferMsg);
			//reniciamos
			conMg = 1;
		}
		else if(estado==1)
		{
			updateDuttyCycle(&handler_PWM, duttyporc);
			estado = 0;
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

	//---------------PIN: PA2----------------
	//------------AF7: USART2_TX----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_USB.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIO_USB.GPIO_PinConfig.GPIO_PinNumber = PIN_2; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_USB, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF7);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_USB);

	//---------------PIN: PB6----------------
	//------------AF2: TIM4_CH1----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_PWM.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_PWM.GPIO_PinConfig.GPIO_PinNumber = PIN_6; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_PWM, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF2);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_PWM);

	//-------------------Fin de Configuracion GPIOx-----------------------

	//-------------------Inicio de Configuracion USARTx-----------------------

	//---------------USART2----------------
	//Definimos el periferico USARTx a utilizar
	handler_USB.ptrUSARTx = USART2;
	//Definimos la configuracion del USART seleccionado
	handler_USB.USART_Config.USART_mode = USART_MODE_TX ;           //USART_MODE_x  x-> TX, RX, RXTX, DISABLE
	handler_USB.USART_Config.USART_baudrate = USART_BAUDRATE_9600;  //USART_BAUDRATE_x  x->9600, 19200, 115200
	handler_USB.USART_Config.USART_parity= USART_PARITY_NONE;       //USART_PARITY_x   x->NONE, ODD, EVEN
	handler_USB.USART_Config.USART_stopbits=USART_STOPBIT_1;         //USART_STOPBIT_x  x->1, 0_5, 2, 1_5
	handler_USB.USART_Config.USART_enableIntRX = USART_RX_INTERRUP_DISABLE;   //USART_RX_INTERRUP_x  x-> DISABLE, ENABLE
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


	//-------------------Inicio de Configuracion EXTIx -----------------------

	//---------------PIN: PC13----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIOButton.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIOButton.GPIO_PinConfig.GPIO_PinNumber = PIN_13;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTIButton.pGPIOHandler = &handler_GPIOButton;
	//Definimos el tipo de flanco
	handler_EXTIButton.edgeType = EXTERNAL_INTERRUPP_FALLING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTIButton);

	//-------------------Fin de Configuracion EXTIx-----------------------

	//-------------------Inicio de Configuracion PWM_Channelx----------------------

	//---------------TIM4_Channel_1----------------
	//Definimos el TIMx a usar
	handler_PWM.ptrTIMx = TIM4;
	//Definimos la configuracion para el PWM
	handler_PWM.config.periodcnt = BTIMER_PCNT_1us; //BTIMER_PCNT_xus x->1,10,100/ BTIMER_PCNT_1ms
	handler_PWM.config.periodo = 20000;             //Al definir 1us, 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	handler_PWM.config.channel = PWM_CHANNEL_1;     //PWM_CHANNEL_x x->1,2,3,4
	handler_PWM.config.duttyCicle = 10;             //Valor entre 0-100 [%]
	//Cargamos la configuracion
	pwm_Config(&handler_PWM);
	//Activar el TIMER y con ello el PWM
	startPwmSignal(&handler_PWM);

	//---------------------Fin de Configuracion PWM_Channelx-----------------------
}
//------------------------------Fin Configuracion del microcontrolador------------------------------------------


//----------------------------Inicio de la definicion de las funciones ISR---------------------------------------

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
//INTERMITENCIA DEL LED2
void BasicTimer2_Callback(void)
{
	GPIOxTooglePin(&handler_BlinkyPin);
	conMg++;
}

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el EXTI13

void callback_extInt13(void)
{
	writeMsg(&handler_USB, sendMg);
	if(duttyporc<100)
	{
		duttyporc++;
	}
	else
	{
		__NOP();
	}
	estado = 1;
}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------
