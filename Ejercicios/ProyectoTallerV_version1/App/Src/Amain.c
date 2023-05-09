/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Separador de tuercas version 1
 ******************************************************************************
 */

//----------------------------------Inicio de definicion de librerias-------------------------------------------
#include <stdint.h>
#include <stm32f411xe.h>
#include <GPIOxDriver.h>
#include <BasicTimer.h>
#include <ExtiDriver.h>
#include <USARTxDriver.h>
#include <SysTickDriver.h>
#include <PwmDriver.h>
#include <KeyPadDriver.h>
#include <InterfazDriver.h>

//-----------------------------------Fin de definicion de librerias------------------------------------------

//---------------------------Inicio de definicion de funciones y variables base----------------------------------
void int_Hardware(void);                       //Definimos la cabecera para la configuracion
GPIO_Handler_t handler_BlinkyPin = {0};        //Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
BasicTimer_Handler_t handler_BlinkyTimer ={0}; //Definimos un elemento del tipo GPIO_Handler_t (Struct)

//---------------------------Fin de definicion de funciones y variables base----------------------------------

//-----------------------Buzzer Activo--------------------------
GPIO_Handler_t handler_GPIO_Buzzer = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) para el uso del buzzer Activo

//--------------------Comunicacion Serial-----------------------
GPIO_Handler_t handler_GPIO_USB = {0};    //Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB
USART_Handler_t handler_USB = {0};

//----------------- Cantidades por recipiente------------------
uint8_t canRecipiente[6] = {0,0,0,0,0,0};    //Arreglo que contiene la cantidad elementos por recipiente
uint8_t pos_canRec = 0;                     //Variable que indica la posicion en el Arreglo

//-------------------------Sensor---------------------------------
GPIO_Handler_t handler_GPIO_Sensor = {0};  //Definicion un elemento del tipo EXTI_Config_t y GPIO_Handler_t para la recepcion de la señal del Sensor Inductivo
EXTI_Config_t handler_EXTI_Sensor ={0};
uint8_t contador = 0;                      //Variable que guarda la cantidad de elementos que pasa delante del sensor

//---------------------Motor Paso a Paso----------------------------
void control_MPP(void);                   //Definimos la cabecera para la funcion que controla el movimiento del motor
GPIO_Handler_t handler_GPIO_MPP = {0};    //Definimos un elemento del tipo GPIO_Handler_t (Struct) como señal para girar el motor paso a paso
GPIO_Handler_t handler_GPIO_MPP_DIR = {0};//Definimos un elemento del tipo GPIO_Handler_t (Struct) para definir la direccion de giro del MPP
uint8_t movState = 1;                     //Variable que habilita el movimiento del servo motor

//------------------------Servomotor--------------------------------
void control_Servo(uint8_t valor);          //Definimos la cabecera para la funcion que controla el servo motor
GPIO_Handler_t handler_GPIO_Servo = {0};    //Definimos un elemento del tipo GPIO_Handler_t (Struct) y PWM_Handler_t para la señal PWM para el servomotor
PWM_Handler_t handler_PWM_Servo = {0};

//--------------------------Keypad----------------------------------
Keypad5x4_Rows_t KeyPad_Control = {0};     //Definimos un elemento del tipo Keypad4x5_Rows_t (Struct) para el uso del KeyPad de 5x4

//------------Columnas------------
GPIO_Handler_t handler_GPIO_Column1 = {0};  //Definimos un elemento del tipo GPIO_Handler_t (Struct) y EXTI_Config_t (Struct) para la columna 1 del KeyPad
EXTI_Config_t handler_EXTI_Column1 ={0};
GPIO_Handler_t handler_GPIO_Column2 = {0};  //Definimos un elemento del tipo GPIO_Handler_t (Struct) y EXTI_Config_t (Struct) para la columna 1 del KeyPad
EXTI_Config_t handler_EXTI_Column2 ={0};
GPIO_Handler_t handler_GPIO_Column3 = {0};  //Definimos un elemento del tipo GPIO_Handler_t (Struct) y EXTI_Config_t (Struct) para la columna 1 del KeyPad
EXTI_Config_t handler_EXTI_Column3 ={0};
GPIO_Handler_t handler_GPIO_Column4 = {0};  //Definimos un elemento del tipo GPIO_Handler_t (Struct) y EXTI_Config_t (Struct) para la columna 1 del KeyPad
EXTI_Config_t handler_EXTI_Column4 ={0};

//-------------Filas---------------
GPIO_Handler_t handler_GPIO_Fila1 = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 1 del KeyPad
GPIO_Handler_t handler_GPIO_Fila2 = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 1 del KeyPad
GPIO_Handler_t handler_GPIO_Fila3 = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 1 del KeyPad
GPIO_Handler_t handler_GPIO_Fila4 = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 1 del KeyPad
GPIO_Handler_t handler_GPIO_Fila5 = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) para la fila 1 del KeyPad

char keys[5][4] = {                         //Definicion de la distribucion de teclas
  {'A','0','D','E'},
  {'7','8','9','N'},
  {'4','5','6','S'},
  {'1','2','3','W'},
  {'F','G','#','*'}
};
char tecla = 'A';                           //Variable para guardar el char

//------------------Definiciones generales----------------------------------
uint8_t boolOperacion = 1;                   //Variable para el control del ciclo while de la operacion
uint8_t boolInterfaceEnd = 1;                //Variable para el control del ciclo while de  interface end

int main(void)
{
	//Realizamos la configuracuion inicial
	int_Hardware();
	//Activamos el SysTick
	config_SysTick_ms(0);
	//Definimos par el PIN un 1 logico,
	GPIO_writePin (&handler_BlinkyPin, SET);
	//Definimos un valor alto para los pines de la filas
	GPIO_writePin (&handler_GPIO_Fila1, SET);
	GPIO_writePin (&handler_GPIO_Fila2, SET);
	GPIO_writePin (&handler_GPIO_Fila3, SET);
	GPIO_writePin (&handler_GPIO_Fila4, SET);
	GPIO_writePin (&handler_GPIO_Fila5, SET);

	while(1)
	{
		//-----------------------Interfaz Inicial-----------------------------------
		//funcion que se encarga de ejecutarla la interfaz inicial
		InterfaceStart(&handler_USB, canRecipiente);

		//-----------------------Pausa inicial-----------------------------------
		//Realizamos una pausa acompañada de la activacion del buzzer activo para indicar el inicio de la operacion
		GPIO_writePin (&handler_GPIO_Buzzer, SET);
		delay_ms(2000);
		GPIO_writePin (&handler_GPIO_Buzzer, RESET);
		delay_ms(1000);

		//---------------------Operacion de separacion-----------------------------------
		GPIO_writePin (&handler_GPIO_MPP_DIR, RESET); //Definimos el sentido de giro
		boolOperacion = 1; //se define un valor alto
		pos_canRec = 0; //Se define la primera pocision

		while(boolOperacion)
		{
			/*El brazo del servo se establece x grados, con lo cual el sistema de transmision se engancha
			 * con el engranaje del Disco Superior.
			*/
			control_Servo(1);
			delay_ms(2000);

			movState = 1; //se define un valor alto
			//El MPP se mueve cada 10 pasos por cada ciclo While. Se detiene cuando se cumpla la cantidad deseada de elementos.
			while(movState)
			{
				control_MPP();
			}
			movState = 1; //se define un valor alto

			if(pos_canRec<(amountContainers()-1))
			{
				/*El brazo del servo se establece 0 grados, con lo cual el sistema de transmision se engancha
				* con el engranaje del Disco Inferior.
				 */
				control_Servo(2);
				delay_ms(2000);

				//El MPP se mueve 50 pasos lo que equivale a que el disco inferior se mueva 60°
				for(uint8_t u=0; u<6; u++)
				{
					control_MPP();
				}
				//Cambiamos la poscion de la cantidad a la siguiente
				pos_canRec++;
			}
			else
			{
				boolOperacion=0;
			}

		}

		//-----------------------Pausa final-----------------------------------
		//Realizamos una pausa acompañada de la activacion del buzzer activo para indicar el fin de la operacion
		GPIO_writePin (&handler_GPIO_Buzzer, SET);
		delay_ms(2000);
		GPIO_writePin (&handler_GPIO_Buzzer, RESET);
		delay_ms(1000);

		//-------------------Interfaz Inicial + Movimiento Disco Inferior-----------------------------------
		//Cargamso la posicion final de los recipiente al number_Containers
		definenumberContainers(pos_canRec);
		/*El brazo del servo se establece 0 grados, con lo cual el sistema de transmision se engancha
		* con el engranaje del Disco Inferior.
		 */
		control_Servo(2);
		delay_ms(2000);
		//Definimos una variable para guardad la posicion de los recipientes anteriores
		uint8_t posant_Recipiente = pos_canRec;

		//Ciclo while de la interfaz final
		while(boolInterfaceEnd)
		{
			uint8_t pos_Recipiente = 0;          //Definimos una variable para guarda la posicion de los recipientes

			//Ejecutamos la funcion de la interfazfinal y ademas guardamos la posicion de los recipientes definida por KeyPad
			pos_Recipiente = InterfaceEnd(&handler_USB);

			//Verificamos si la tecla presionada fue Ent, con ello salimos del ciclo while
			if(tecla=='E')
			{
				boolInterfaceEnd = 0;
			}
			else
			{
				//veficamos si el valor definido de la posicion actual de los recipientes disminuyo
				if(pos_Recipiente<posant_Recipiente)
				{
					GPIO_writePin (&handler_GPIO_MPP_DIR, SET); //Definimos el sentido de giro
					//El MPP se mueve 50 pasos lo que equivale a que el disco inferior se mueva 60°
					for(uint8_t u=0; u<6; u++)
					{
						control_MPP();
					}

					posant_Recipiente=pos_Recipiente;
				}
				if(pos_Recipiente>posant_Recipiente)
				{
					//veficamos si el valor definido de la posicion actual de los recipientes aunmento
					GPIO_writePin (&handler_GPIO_MPP_DIR, RESET); //Definimos el sentido de giro
					//El MPP se mueve 50 pasos lo que equivale a que el disco inferior se mueva 60°
					for(uint8_t u=0; u<6; u++)
					{
						control_MPP();
					}
					posant_Recipiente=pos_Recipiente;
				}
				else
				{
					__NOP();
				}
			}
		}
		//Reiniciamos el valor de la variable
		boolInterfaceEnd = 1;
	}
	return 0;
}



//------------------------------Inicio Configuracion del microcontrolador------------------------------------------
void int_Hardware(void)
{
	//-------------------Inicio de Configuracion GPIOx-----------------------

	//---------------------------BlinkyLed--------------------------------
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

	//-------------------------Buzzer Activo-------------------------------
	//---------------PIN: PB8----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Buzzer.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_Buzzer.GPIO_PinConfig.GPIO_PinNumber = PIN_8; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_Buzzer, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_Buzzer);

	//---------------------------USART--------------------------------
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

	//---------------------------MPP--------------------------------
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
	handler_GPIO_MPP_DIR.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIO_MPP_DIR.GPIO_PinConfig.GPIO_PinNumber = PIN_7; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_MPP_DIR, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_MPP_DIR);


	//---------------------------Servo--------------------------------
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

	//---------------------------KeyPad--------------------------------
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
	handler_GPIO_Fila5.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_Fila5.GPIO_PinConfig.GPIO_PinNumber = PIN_3; 						//PIN_x, 0-15
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
	handler_USB.USART_Config.USART_stopbits = USART_STOPBIT_1;         //USART_STOPBIT_x  x->1, 0_5, 2, 1_5
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

	//---------------------------KeyPad--------------------------------
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

	//---------------PIN: PC3----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Column4.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIO_Column4.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTI_Column4.pGPIOHandler = &handler_GPIO_Column4;
	//Definimos el tipo de flanco
	handler_EXTI_Column4.edgeType = EXTERNAL_INTERRUPP_RISING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTI_Column4);

	//---------------------------Sensor--------------------------------
	//---------------PIN: PC12----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Sensor.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIO_Sensor.GPIO_PinConfig.GPIO_PinNumber = PIN_12;
	//Definimos la posicion del elemento pGIOHandler.
	handler_EXTI_Sensor.pGPIOHandler = &handler_GPIO_Sensor;
	//Definimos el tipo de flanco
	handler_EXTI_Sensor.edgeType = EXTERNAL_INTERRUPP_RISING_EDGE;
	//Cargamos la configuracion del EXTIx
	extInt_Config(&handler_EXTI_Sensor);

	//-------------------Fin de Configuracion EXTIx-----------------------

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

//-------------------------BlinkyLed--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
void BasicTimer2_Callback(void)
{
	GPIOxTooglePin(&handler_BlinkyPin);
}

//---------------------------KeyPad--------------------------------
//Definimos las funciones que se desean ejecutar cuando se genera la interrupcion por el EXTI0-1-2-3
void callback_extInt0(void)
{
	//determinar la tecla seleccionada
	tecla = readKeyPad5x4(&KeyPad_Control, &handler_GPIO_Column1, keys, 0);
	//Limpiamos la bandera de nuevo.
	EXTI->PR |= (EXTI_PR_PR0);
	//funcion que ejecuta una operacion de la interface deacuerdo a la tecla presionada
	executeChar(&handler_USB, tecla);
}
void callback_extInt1(void)
{
	//determinar la tecla seleccionada
	tecla = readKeyPad5x4(&KeyPad_Control, &handler_GPIO_Column2, keys, 1);
	//Limpiamos la bandera de nuevo.
	EXTI->PR |= (EXTI_PR_PR1);
	//funcion que ejecuta una operacion de la interface deacuerdo a la tecla presionada
	executeChar(&handler_USB, tecla);
}
void callback_extInt2(void)
{
	//determinar la tecla seleccionada
	tecla = readKeyPad5x4(&KeyPad_Control, &handler_GPIO_Column3, keys, 2);
	//Limpiamos la bandera de nuevo.
	EXTI->PR |= (EXTI_PR_PR2);
	//funcion que ejecuta una operacion de la interface deacuerdo a la tecla presionada
	executeChar(&handler_USB, tecla);
}
void callback_extInt3(void)
{
	//determinar la tecla seleccionada
	tecla = readKeyPad5x4(&KeyPad_Control, &handler_GPIO_Column4, keys, 3);
	//Limpiamos la bandera de nuevo.
	EXTI->PR |= (EXTI_PR_PR3);
	//funcion que ejecuta una operacion de la interface deacuerdo a la tecla presionada
	executeChar(&handler_USB, tecla);
}

//---------------------------Sensor--------------------------------
//Definimos la funcion que se desean ejecutar cuando se genera la interrupcion por el EXTI12
void callback_extInt12(void)
{
	contador++;

	if(contador<canRecipiente[pos_canRec])
	{
		contador = 0;
		movState= 0;
	}
	else
	{
		__NOP();
	}
}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------

//----------------------------Inicio de la definicion de las funciones ---------------------------------------

//Funcion para generar un paso del motor paso a paso
void control_MPP(void)
{
	for(int u=0; u<21; u++)
	{
		GPIOxTooglePin(&handler_GPIO_MPP);
		delay_ms(10);
	}
}

//Funcion para control de servomotor
void control_Servo(uint8_t pos)
{
	if(pos==1)
	{
		updateDuttyCycle(&handler_PWM_Servo, 7);
	}
	else if(pos==2)
	{
		updateDuttyCycle(&handler_PWM_Servo, 5);
	}
	else
	{
		__NOP();
	}

}
//----------------------------Fin de la definicion de las funciones ----------------------------------------
