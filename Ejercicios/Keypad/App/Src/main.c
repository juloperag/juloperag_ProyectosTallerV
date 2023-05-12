/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Control de KeyPad
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
#include <KeyPadDriver.h>

//-----------------------------------Fin de definicion de librerias------------------------------------------


//---------------------------Inicio de definicion de funciones y variables base----------------------------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
GPIO_Handler_t handler_BlinkyPin = {0};

//Definimos un elemento del tipo GPIO_Handler_t (Struct)
BasicTimer_Handler_t handler_BlinkyTimer ={0};

//Definimos la cabecera para la configuracion
void int_Hardware(void);
//---------------------------Fin de definicion de funciones y variables base----------------------------------


//--------------------------USART-------------------------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB
GPIO_Handler_t handler_GPIO_USB = {0};
USART_Handler_t handler_USB = {0};
//Variable que regula el tiempo de impresion
uint8_t conMg = 1;
//Definimos string
char sendMg[] = "Boton presionado \n";
char bufferMsg[64] = {0};

//--------------------------KEYPAD-------------------------------------------
//Definimos un elemento del tipo Keypad4x5_Rows_t (Struct) para el uso del KeyPad de 5x4
Keypad5x4_Rows_t KeyPad_Control = {0};

//-----------------Columnas------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y EXTI_Config_t (Struct) para la columna 1 del KeyPad
GPIO_Handler_t handler_GPIO_Column1 = {0};
EXTI_Config_t handler_EXTI_Column1 ={0};
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y EXTI_Config_t (Struct) para la columna 1 del KeyPad
GPIO_Handler_t handler_GPIO_Column2 = {0};
EXTI_Config_t handler_EXTI_Column2 ={0};
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y EXTI_Config_t (Struct) para la columna 1 del KeyPad
GPIO_Handler_t handler_GPIO_Column3 = {0};
EXTI_Config_t handler_EXTI_Column3 ={0};
//Definimos un elemento del tipo GPIO_Handler_t (Struct) y EXTI_Config_t (Struct) para la columna 1 del KeyPad
GPIO_Handler_t handler_GPIO_Column4 = {0};
EXTI_Config_t handler_EXTI_Column4 ={0};

//-----------------Filas---------------
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 1 del KeyPad
GPIO_Handler_t handler_GPIO_Fila1 = {0};
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 2 del KeyPad
GPIO_Handler_t handler_GPIO_Fila2 = {0};
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 3 del KeyPad
GPIO_Handler_t handler_GPIO_Fila3 = {0};
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 4 del KeyPad
GPIO_Handler_t handler_GPIO_Fila4 = {0};
//Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 5 del KeyPad
GPIO_Handler_t handler_GPIO_Fila5 = {0};

//Definicion de la distribucion de teclas
char keys[5][4] = {
  {'A','0','D','E'},
  {'7','8','9','N'},
  {'4','5','6','S'},
  {'1','2','3','W'},
  {'F','G','#','*'}
};

//Varible que guarda la tecla plecionada
char tecla = '\0';

int main(void)
{
	//Realizamos la configuracuion inicial
	int_Hardware();

	//Definimos para el PIN un 1 logico,
	GPIO_writePin (&handler_BlinkyPin, SET);
	//Definimos un valor alto para los pines de la filas
	GPIO_writePin (&handler_GPIO_Fila1, SET);
	GPIO_writePin (&handler_GPIO_Fila2, SET);
	GPIO_writePin (&handler_GPIO_Fila3, SET);
	GPIO_writePin (&handler_GPIO_Fila4, SET);
	GPIO_writePin (&handler_GPIO_Fila5, SET);

	while(1)
	{
		if(tecla !='\0')
		{
			//Convertimos en un string
			sprintf(bufferMsg,"Tecla precionada: %c \n",tecla);
			//Enviamos por puerto serial dicho string
			writeMsg(&handler_USB, bufferMsg);
			//definimos la variable como el elemento NULL
			tecla = '\0';
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

	//--------------KeyPad----------------

	//---------------PIN: PA8----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Fila1.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIO_Fila1.GPIO_PinConfig.GPIO_PinNumber = PIN_8; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_Fila1, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_Fila1);
	//Cargamos el elemento GPIO_Handler_t en la estructura que define las filas del KeyPad
	KeyPad_Control.pGPIORow1 = &handler_GPIO_Fila1;

	//---------------PIN: PB10----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Fila2.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_Fila2.GPIO_PinConfig.GPIO_PinNumber = PIN_10; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_Fila2, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_Fila2);
	//Cargamos el elemento GPIO_Handler_t en la estructura que define las filas del KeyPad
	KeyPad_Control.pGPIORow2 = &handler_GPIO_Fila2;

	//---------------PIN: PB4----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Fila3.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_Fila3.GPIO_PinConfig.GPIO_PinNumber = PIN_4; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_Fila3, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_Fila3);
	//Cargamos el elemento GPIO_Handler_t en la estructura que define las filas del KeyPad
	KeyPad_Control.pGPIORow3 = &handler_GPIO_Fila3;

	//---------------PIN: PB5----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Fila4.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_Fila4.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_Fila4, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_Fila4);
	//Cargamos el elemento GPIO_Handler_t en la estructura que define las filas del KeyPad
	KeyPad_Control.pGPIORow4 = &handler_GPIO_Fila4;

	//---------------PIN: PB3----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Fila5.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIO_Fila5.GPIO_PinConfig.GPIO_PinNumber = PIN_10; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_Fila5, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_Fila5);
	//Cargamos el elemento GPIO_Handler_t en la estructura que define las filas del KeyPad
	KeyPad_Control.pGPIORow5 = &handler_GPIO_Fila5;


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

	//---------------PIN: PC0----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Column1.pGPIOx = GPIOC;
	//Definimos el pin a utiliza2
	handler_GPIO_Column1.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTI_Column1.pGPIOHandler = &handler_GPIO_Column1;
	//Definimos el tipo de flanco
	handler_EXTI_Column1.edgeType = EXTERNAL_INTERRUPP_RISING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTI_Column1);

	//---------------PIN: PC1----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Column2.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIO_Column2.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTI_Column2.pGPIOHandler = &handler_GPIO_Column2;
	//Definimos el tipo de flanco
	handler_EXTI_Column2.edgeType = EXTERNAL_INTERRUPP_RISING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTI_Column2);

	//---------------PIN: PC2----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Column3.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIO_Column3.GPIO_PinConfig.GPIO_PinNumber = PIN_2;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTI_Column3.pGPIOHandler = &handler_GPIO_Column3;
	//Definimos el tipo de flanco
	handler_EXTI_Column3.edgeType = EXTERNAL_INTERRUPP_RISING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTI_Column3);

	//---------------PIN: PB3----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Column4.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_Column4.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTI_Column4.pGPIOHandler = &handler_GPIO_Column4;
	//Definimos el tipo de flanco
	handler_EXTI_Column4.edgeType = EXTERNAL_INTERRUPP_RISING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTI_Column4);

	//-------------------Fin de Configuracion EXTIx-----------------------

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

//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el EXTI0
void callback_extInt0(void)
{
	//determinar la tecla seleccionada
	tecla = readKeyPad5x4(&KeyPad_Control, &handler_GPIO_Column1, keys, 0);
	//Limpiamos la bandera de nuevo.
	EXTI->PR |= (EXTI_PR_PR0);
}

void callback_extInt1(void)
{
	//determinar la tecla seleccionada
	tecla = readKeyPad5x4(&KeyPad_Control, &handler_GPIO_Column2, keys, 1);
	//Limpiamos la bandera de nuevo.
	EXTI->PR |= (EXTI_PR_PR1);
}

void callback_extInt2(void)
{
	//determinar la tecla seleccionada
	tecla = readKeyPad5x4(&KeyPad_Control, &handler_GPIO_Column3, keys, 2);
	//Limpiamos la bandera de nuevo.
	EXTI->PR |= (EXTI_PR_PR2);
}

void callback_extInt3(void)
{
	//determinar la tecla seleccionada
	tecla = readKeyPad5x4(&KeyPad_Control, &handler_GPIO_Column4, keys, 3);
	//Limpiamos la bandera de nuevo.
	EXTI->PR |= (EXTI_PR_PR3);
}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------
