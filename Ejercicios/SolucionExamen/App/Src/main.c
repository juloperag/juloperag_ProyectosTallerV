/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Solucion del Examen
 ******************************************************************************
 */

//----------------------------------Inicio de definicion de librerias-------------------------------------------
#include <stdint.h>
#include <stm32f411xe.h>
#include <stdio.h>
#include <GPIOxDriver.h>
#include <BasicTimer.h>
#include <USARTxDriver.h>
#include <SysTickDriver.h>
#include <PwmDriver.h>
#include <I2CDriver.h>
#include <PLLDriver.h>
#include <RTCDriver.h>
#include <AdcDriver.h>
//#include <dsp/basic_math_functions.h>
#include <arm_math.h>

//-----------------------------------Fin de definicion de librerias------------------------------------------
GPIO_Handler_t handler_GPIO_Prueba = {0};       //Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB

//---------------------------Inicio de definicion de funciones y variables base----------------------------------

GPIO_Handler_t handler_BlinkyPin = {0};         //Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
BasicTimer_Handler_t handler_BlinkyTimer ={0};  //Definimos un elemento del tipo GPIO_Handler_t (Struct)
void int_Hardware(void);                        //Definimos la cabecera para la configuracion

//---------------------------Fin de definicion de funciones y variables base----------------------------------

//-------------------------PIN MCO1--------------------------------
GPIO_Handler_t handler_GPIO_MCO1 = {0};       //Definimos un elemento del tipo GPIO_Handler_t (Struct) para utilizar el pin MCO1 con el fin de muestrear las frecuencias de los osciladores

//--------------------------USART USB-------------------------------
GPIO_Handler_t handler_GPIO_USB_TX = {0};       //Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB
GPIO_Handler_t handler_GPIO_USB_RX = {0};
USART_Handler_t handler_USART_USB = {0};
char charRead = '\0';                          //Variable que almacena el caracter leido

//--------------------------PWM-------------------------------
PWM_Handler_t handler_PWM_1 = {0};       //Definimos un elemento del tipo PWM_Handler_t para la generan de la señal PWM

//-------------------------RTC-------------------------------
RTC_Handler_t handler_RTC = {0};                                                  //Definimos un elemento del tipo RTC_Handler_t (Struct) para el uso del RTC
uint8_t numberDayMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};    //Arreglo que contiene la cantidad de los dias de cada mes del año

//-------------------------Acelerometro----------------------------
GPIO_Handler_t handler_GPIO_SCL_Acelerometro = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) y I2C_Handler_t para la comunicacion I2C
GPIO_Handler_t handler_GPIO_SDA_Acelerometro = {0};
I2C_Handler_t handler_I2C_Acelerometro = {0};

//-------Direcciones--------
#define ACCEL_ADDRESSS_ACCEL  0b1101000;             //Definicion de la direccion del Sclave
#define ACCEL_XOUT_H  0x3B                           //Definicion de la direccion de los registros del Sclave a usar
#define ACCEL_XOUT_L  0x3C
#define ACCEL_YOUT_H  0x3D
#define ACCEL_YOUT_L  0x3E
#define ACCEL_ZOUT_H  0x3F
#define ACCEL_ZOUT_L  0x40

#define AFS_SEL     0x1C                           //Registros de la configuracion del acelerometro
#define PWR_MGMT_l  107                            //Hay un bit en dicho registro para aplicar reset
#define WHO_AM_I    117                            //Registro para verificar la identidad del equipo

void acelerometro_I2C(void);                       //Definimos la cabecera de la funcion encargada de leer los registros accel

//-------------------------------Comandos----------------------------------
void recepcionCommand(void);                      //Funcion que recibe los caracteres del comando recibido
void runCommand(char *command);                   //Funcion que ejecuta el comando ingresando
char bufferRecepcion[64] = {0};                   //Arreglo que almacena el comando ingresado
uint8_t counterRecepcion = 0;                     //Variable para la posicion del arrelgo
uint8_t commandComplete = 0;                      //Bandera que indica si el comando esta completo

//-------------------------------ADC---------------------------------
ADC_Config_t handler_ADC_canales = {0};          //Definimos un elemento del tipo ADC_Config_t para el uso de los canales del ADC
uint16_t adcDataChannel_0[256] = {0};            //Arreglo que almacena los datos recolectados del cannal 0 del ADC
uint16_t adcDataChannel_1[256] = {0};            //Arreglo que almacena los datos recolectados del cannal 1 del ADC
uint16_t adcPosArreglo = 0;                      //Elemento que guarda la posicion de los arreglos
uint8_t adcArregloChannel = 0;                  //Selecciona el arreglo correspondiente al canal respectivo

//------------------Definiciones generales----------------------------------
BasicTimer_Handler_t handler_TIMER_signal_200Hz = {0};     //Definimos un elemento del tipo GPIO_Handler_t (Struct) para realizar una señal de 200Hz

void conditionTxADC(void);                //Funcion que almacena los datos capturados por el ADC y luego los envia por comunicacion serial.
void conditionFFT(void);                  //Funcion ejecutamos la FFT o enviamos los datos de la FFT
void executionFFT(void);                  //Funcion que ejecuta la FFT
float convAccel(int16_t accelx);          //Funcion que convierte los valores guardados entregados por el acelerometro en valores en unidades m/s2
void int_MCO1(void);                      //Funcion para la configuracion inicail del MCO1
uint8_t numberDayWeed(char *msg);         //Funcion que retorna el dia de la semana en numero
void txdayWeed(uint8_t number);            //Funcion que envia por usart el dia de la semana

uint8_t stastusTimer_200Hz = 0;           //Bandera que se usa como indicativo de la señal de 200Hz

uint8_t stastusTxADC = 0;                 //variable que activa la transmision de datos arquiridos por ADC
uint16_t countingTxADC  = 0;             //variable que cuenta las veces que se a mandado lod datos del arreglo
uint8_t  timerTxADC = 0;                 //variable para controlar el tiempo con que se envia los mensajes por USART del ADC

#define size_Sampling  1024             //Definimos la cantidad de datos a tomar con el acelerometro
uint8_t stateSampling_Accel = 0;          //Variable que controla el estado del muestreo de eje z
uint16_t countingSampling = 0;            //variable que cuenta las veces que se a mandado el a enviado el muestreo
float32_t accelSampling_Z[size_Sampling] = {0};//Arreglo para guardar los datos leidos por el Accel

uint8_t stateFFT = 0;                     //Bandera para ejecutar la FFT para los datos del accel
uint8_t timerTxFFT = 0;                   //variable para controlar el tiempo con que se envia los mensajes por USART de la FFT
uint32_t countingTxFFT = 0;                //variable que cuenta las veces que se a mandado los datos del arreglo
float32_t dateFFTSampling[size_Sampling] = {0};         //Arreglo que almacena los datos de la FFT luego de aplicales valor Absoluto
uint32_t dateDominantFreq = 0;                //variable que guarda el dato de la frecuencia dominate de los datos muestreados por el acelerometro
float32_t valueMaxFFT = 0;                //Variable que guarda el maximo valor de la FFT y por tanto la amplitud del armonico
uint32_t fftSize = 1024;                                        //Valor que indica la duracion del proceso RFFT/CIFFT.

int main(void)
{
	//Incrementamos la velocidad de reloj del sistema
	uint8_t clock = CLOCK_SPEED_100MHZ;    //Velocidad de reloj entre 25 o 100 MHz
	configPLL(clock);
	//Realizamos la configuracuion inicial
	int_Hardware();
	//Activamos el punto flotante por medio del registro especifico
	SCB->CPACR |= 0xF <<20;
	//Definimos la configuracion inicail del MCO1
	 int_MCO1();

	while(1)
	{
		//Condicion para el muestreo de datos del acelerometro
		if(stateSampling_Accel == 1 && stastusTimer_200Hz == 1)
		{
			GPIO_writePin (&handler_GPIO_Prueba, SET);
			//Realizamos el muestreo del eje Z ejes de accel
			acelerometro_I2C();
			GPIO_writePin (&handler_GPIO_Prueba, RESET);
			//Reiniciamos variable
			stastusTimer_200Hz = 0;
		}
		else
		{
		 __NOP();
		}

		///Verificamos para ejecutar la FFT e imprimirla
		if((stateFFT==1 || stateFFT==2) && stastusTimer_200Hz == 1)
		{
			//Ejecutamos la FFT o enviamos los datos de la FFT
			conditionFFT();
			//Reniciamos la variable
			stastusTimer_200Hz = 0;
		}
		else
		{
			__NOP();
		}

		///Verificamos para ejecuta el comando ingresando
		if(commandComplete==1)
		{
			runCommand(bufferRecepcion);
			//Reniciamos la variable
			commandComplete=0;
		}
		else
		{
			__NOP();
		}

		///Verificamos para imprimir los datos
		if(stastusTxADC == 1 && stastusTimer_200Hz == 1)
		{
			//Enviamos datos por comunicacion serial
			conditionTxADC();
			//Reiniciamos variable
			 stastusTimer_200Hz = 0;
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

	//---------------PIN: PC0----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_Prueba.pGPIOx = GPIOC;
	//Definimos el pin a utilizar
	handler_GPIO_Prueba.GPIO_PinConfig.GPIO_PinNumber = PIN_0; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_Prueba, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_Prueba);

	//---------------------------BlinkyLed--------------------------------
	//---------------PIN: PH1----------------
	//Definimos el periferico GPIOx a usar.
	handler_BlinkyPin.pGPIOx = GPIOH;
	//Definimos el pin a utilizar
	handler_BlinkyPin.GPIO_PinConfig.GPIO_PinNumber = PIN_1; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_BlinkyPin, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_BlinkyPin);

	//-------------------------PIN_MCO1--------------------------------
	//---------------PIN: PA8----------------
	//------------AF0: MCO_1----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_MCO1.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIO_MCO1.GPIO_PinConfig.GPIO_PinNumber = PIN_8; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_MCO1, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_MCO1);

	//---------------------------USB--------------------------------
	//---------------PIN: PA9----------------
	//------------AF7: USART1_TX-------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_USB_TX.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIO_USB_TX.GPIO_PinConfig.GPIO_PinNumber = PIN_9; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_USB_TX, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF7);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_USB_TX);

	//---------------PIN: PA10----------------
	//------------AF7: USART1_RX----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_USB_RX.pGPIOx = GPIOA;
	//Definimos el pin a utilizar
	handler_GPIO_USB_RX.GPIO_PinConfig.GPIO_PinNumber = PIN_10; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_USB_RX, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF7);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_USB_RX);

	//---------------------------Acelerometro--------------------------------
	//---------------PIN: PB8----------------
	//------------AF4: I2C1_SCL----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_SCL_Acelerometro.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_SCL_Acelerometro.GPIO_PinConfig.GPIO_PinNumber = PIN_8; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_SCL_Acelerometro, GPIO_MODE_ALTFN, GPIO_OTYPER_OPENDRAIN, GPIO_OSPEEDR_FAST, GPIO_PUPDR_NOTHING, AF4);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_SCL_Acelerometro);

	//---------------PIN: PB9----------------
	//------------AF4: I2C1_SDA----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_SDA_Acelerometro.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_SDA_Acelerometro.GPIO_PinConfig.GPIO_PinNumber = PIN_9; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_SDA_Acelerometro, GPIO_MODE_ALTFN, GPIO_OTYPER_OPENDRAIN, GPIO_OSPEEDR_FAST, GPIO_PUPDR_NOTHING, AF4);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_SDA_Acelerometro);

	//-------------------Fin de Configuracion GPIOx-----------------------

	//-------------------Inicio de Configuracion USARTx-----------------------

	//---------------USART1----------------
	//Definimos el periferico USARTx a utilizar
	handler_USART_USB.ptrUSARTx = USART1;
	//Definimos la configuracion del USART seleccionado
	handler_USART_USB.USART_Config.USART_mode = USART_MODE_RXTX ;           //USART_MODE_x  x-> TX, RX, RXTX, DISABLE
	handler_USART_USB.USART_Config.USART_baudrate = USART_BAUDRATE_115200;  //USART_BAUDRATE_x  x->9600, 19200, 115200
	handler_USART_USB.USART_Config.USART_parity= USART_PARITY_NONE;         //USART_PARITY_x   x->NONE, ODD, EVEN
	handler_USART_USB.USART_Config.USART_stopbits=USART_STOPBIT_1;          //USART_STOPBIT_x  x->1, 0_5, 2, 1_5
	handler_USART_USB.USART_Config.USART_enableIntRX = USART_RX_INTERRUP_ENABLE;   //USART_RX_INTERRUP_x  x-> DISABLE, ENABLE
	handler_USART_USB.USART_Config.USART_enableIntTX = USART_TX_INTERRUP_ENABLE;   //USART_TX_INTERRUP_x  x-> DISABLE, ENABLE
	//Cargamos la configuracion del USART especifico
	USART_Config(&handler_USART_USB);

	//-------------------Fin de Configuracion USARTx-----------------------

	//-------------------Inicio de Configuracion TIMx-----------------------

	//---------------TIM2----------------
	//Definimos el TIMx a usar
	handler_TIMER_signal_200Hz.ptrTIMx = TIM2;
	//Definimos la configuracion del TIMER seleccionado
	handler_TIMER_signal_200Hz.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handler_TIMER_signal_200Hz.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handler_TIMER_signal_200Hz.TIMx_Config.TIMX_period = 3;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handler_TIMER_signal_200Hz);

	//---------------TIM4----------------
	//Definimos el TIMx a usar
	handler_BlinkyTimer.ptrTIMx = TIM4;
	//Definimos la configuracion del TIMER seleccionado
	handler_BlinkyTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handler_BlinkyTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handler_BlinkyTimer.TIMx_Config.TIMX_period = 248;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handler_BlinkyTimer);

	//------------------------Fin de Configuracion TIMx-----------------------

	//-------------------Inicio de Configuracion PWM_Channelx----------------------

	//---------------TIM3_Channel_1----------------
	//Definimos el TIMx a usar
	handler_PWM_1.ptrTIMx = TIM3;
	//Definimos la configuracion para el PWM
	handler_PWM_1.config.periodcnt = BTIMER_PCNT_1us; //BTIMER_PCNT_xus x->1,10,100/ BTIMER_PCNT_1ms
	handler_PWM_1.config.periodo = 100;             //Al definir 1us, 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	handler_PWM_1.config.channel = PWM_CHANNEL_1;     //PWM_CHANNEL_x x->1,2,3,4
	handler_PWM_1.config.duttyCicle = 8;          //Valor señal en alto
	//Cargamos la configuracion
	pwm_Config(&handler_PWM_1);

	//---------------------Fin de Configuracion PWM_Channelx-----------------------


	//-----------------------Inicio de Chhonfiguracion ADC----------------------

	//Configuracion Multicanal
	handler_ADC_canales.conversion.channelSequence_0 = ADC_CHANNEL_0;                           //ADC_CHANNEL_x                 x-> 0-16
	handler_ADC_canales.sampling.samplingPeriodChannel_0 = ADC_SAMPLING_PERIOD_112_CYCLES;      //ADC_SAMPLING_PERIOD_x_CYCLES  x-> 3, 15, 28, 56, 84, 112, 144, 480
	handler_ADC_canales.conversion.channelSequence_1 = ADC_CHANNEL_1;                           //ADC_CHANNEL_x                 x-> 0-16
	handler_ADC_canales.sampling.samplingPeriodChannel_1 = ADC_SAMPLING_PERIOD_112_CYCLES;      //ADC_SAMPLING_PERIOD_x_CYCLES  x-> 3, 15, 28, 56, 84, 112, 144, 480
	//Configuracion general
	 handler_ADC_canales.resolution = ADC_RESOLUTION_12_BIT;                                    //ADC_RESOLUTION_x_BIT          x-> 8,6,10,12
	 handler_ADC_canales.dataAlignment =  ADC_ALIGNMENT_RIGHT;                                  //ADC_ALIGNMENT_x               x-> RIGHT, LEFT
	 handler_ADC_canales.externalEvent = ADC_EXTERNAL_EVENT_RISING;                             //ADC_EXTERNAL_EVENT_x          x-> DISABLE, RISING, FALLING, BOTH
	 handler_ADC_canales.typeEXTEvent =  ADC_EVENT_TIMER_3_CC1;                                 //ADC_EVENT_TIMER_x_y           x-> 1, 2, 3, 4, 5 | y-> Depende
	ADC_ConfigMultichannel(&handler_ADC_canales, 2);

	//--------------------------Fin de Configuracion ADC----------------------


	//-----------------------Inicio de Configuracion I2Cx----------------------

	//---------------I2C1----------------
	//Definimos el I2Cx a usar
	handler_I2C_Acelerometro.prtI2Cx = I2C1;
	//Definimos la configuracion para el I2C
	handler_I2C_Acelerometro.modeI2C = I2C_MODE_FM;               //I2C_MODE_x  x->SM,FM
	handler_I2C_Acelerometro.slaveAddress = ACCEL_ADDRESSS_ACCEL;       //Direccion del Sclave
	//Cargamos la configuracion
	i2c_Config(&handler_I2C_Acelerometro);

	//------------------------Fin de Configuracion I2Cx--------------------------


	//-----------------------Inicio de Configuracion del RTC----------------------

	//Confiracion inicial del calendario
	handler_RTC.seg = 0;
	handler_RTC.min = 20;
	handler_RTC.hour = 20;
	handler_RTC.dayWeek = LUNES;
	handler_RTC.date = 12;
	handler_RTC.month = JUNIO;
	handler_RTC.year = 23;
	//Cargamos la configuracion y inicializamos el RTC
	configRTC(&handler_RTC);
	//------------------------Fin de Configuracion del RTC-------------------------
}
//------------------------------Fin Configuracion del microcontrolador------------------------------------------



//-----------------------------Inicio configuracion MCO1------------------------------------------

//Funcion para la configuracion inicail del MCO1
void int_MCO1(void)
{
	//Configuracion inicial del MCO1
	configMCO1(0);
	configMCO1PRE(1);
	//Imprimimos la señal de reloj escogida
	writeMsgForTXE(&handler_USART_USB, "Senal de reloj escogida: HSI \n");
	//Imprimimos el valor del preescaler seleccionado
	writeMsgForTXE(&handler_USART_USB, "Valor del preescaler del MCO1: 1 \n");
}
//------------------------------Fin configuracion MCO1------------------------------------------


//----------------------------Inicio de la definicion de las funciones de los comandos----------------------------------------

//Funcion que recibe los caracteres del comando recibido
void recepcionCommand(void)
{
	if (stastusTxADC == 0 && stateSampling_Accel==0 && stateFFT==0)
	{
		if(charRead == '&')
		{
			//Almacenamos el elemento nulo
			bufferRecepcion[counterRecepcion] = '\0';
			//Establecemos la bandera como alta
			commandComplete = 1;
			//Reiniciamos la variable
			counterRecepcion = 0;
			//Reiniciamos la variable
		}
		else
		{
			//Almacenamos los caracteres del comando en un arrelgo
			bufferRecepcion[counterRecepcion] = charRead;
			//Aumentamos en uno la posicion del arreglo
			counterRecepcion++;
			//Reiniciamos la variable
		}
	}
	else
	{
		__NOP();
	}

}

//Funcion que ejecuta el comando ingresando
void runCommand(char *prtcommand)
{
	//Variables para almacenar los elmentos que entrega el comando luego de ser divididos por la funcion sscanf
	char cmd[64]= {0};
	char userMsg[64]= {0};
	int firtsParameter = 0;
	int secondParameter = 0;
	int thirdParameter = 0;
	char bufferMsg[64]= {0};

	//Funcion que lee la cadena de caracteres y la divide en los elementos definidos
	sscanf(prtcommand, "%s %u %u %u %s", cmd, &firtsParameter, &secondParameter,&thirdParameter, userMsg);

	//Imprime lista que muestra los comandos que tiene el dispositivo
	if(strcmp(cmd, "help")==0)
	{
		writeMsgForTXE(&handler_USART_USB, "Help Menu: \n");
		writeMsgForTXE(&handler_USART_USB, "1) help  ---Imprime lista de comandos \n");
		writeMsgForTXE(&handler_USART_USB, "2) MCO1 # ---Selecciona la señal de reloj que saldra del pin MCO1: #->0-3 (HSI,LSE,HSE,PLL) \n");
		writeMsgForTXE(&handler_USART_USB, "3) MCO1PRE #  ---Selecciona el preescaler de la señal saliente del pin MCO1: #->1-5 \n");
		writeMsgForTXE(&handler_USART_USB, "4) configTime # # #  ---Establece la hora, minutos, y segundos del RTC \n");
		writeMsgForTXE(&handler_USART_USB, "5) configDate # # # msg ---Establece el año, mes, fecha y dia de la semana del RTC \n");
		writeMsgForTXE(&handler_USART_USB, "6) showTime  ---Muestra el tiempo establecida en el RTC \n");
		writeMsgForTXE(&handler_USART_USB, "7) showDate  ---Muestra la fecha establecida en el RTC \n");
		writeMsgForTXE(&handler_USART_USB, "8) speedADC # ---Configura la velocidad de muestreo del ADC: #->(30000-105000 Hz) \n");
		writeMsgForTXE(&handler_USART_USB, "9) startADC ---Inicia la toma de datos por parte del ADC, para los dos canales \n");
		writeMsgForTXE(&handler_USART_USB, "10) startAccel ---Inicia la toma de datos por parte del Acelerometro a 200Hz \n");
		writeMsgForTXE(&handler_USART_USB, "11) startFFT --Ejecucion de la transformada rapida de Fourier sobre los datos muestreados por Accel \n");
	}


	//Definimos la señal de reloj saliente del pin MCO1
	else if (strcmp(cmd, "MCO1") == 0)
	{
		if (firtsParameter>=0 && firtsParameter<=3)
		{
			//Definimos la señal de reloj
			configMCO1(firtsParameter);

			switch (firtsParameter) {
				case 0:
				{
					//Imprimimos la señal de reloj escogida
					writeMsgForTXE(&handler_USART_USB, "Señal de reloj escogida: HSI \n");
					break;
				}
				case 1:
				{
					//Imprimimos la señal de reloj escogida
					writeMsgForTXE(&handler_USART_USB, "Señal de reloj escogida: LSE \n");

					break;
				}
				case 2:
				{
					//Imprimimos la señal de reloj escogida
					writeMsgForTXE(&handler_USART_USB, "Señal de reloj escogida: HSE \n");
					break;
				}
				case 3:
				{
					//Imprimimos la señal de reloj escogida
					writeMsgForTXE(&handler_USART_USB, "Señal de reloj escogida: PLL \n");
					break;
				}
				default:
				{
						__NOP();
						break;
				}
			}
		}
		else
		{
			//se imprime la invalidacion de algun parametro del comando
			writeMsgForTXE(&handler_USART_USB, "Valor no valido \n");
		}
	}


	//Definimos el preescaler de la señal de reloj saliente del pin MCO1
	else if (strcmp(cmd, "MCO1PRE") == 0)
	{
		if (firtsParameter>=1 && firtsParameter<=5)
		{
			//Cambiamos el preescaler
			configMCO1PRE(firtsParameter);
			//Imprimimos el valor del preescaler seleccionado
			sprintf(bufferMsg,"Valor del preescaler del MCO1: %u \n", firtsParameter);
			writeMsgForTXE(&handler_USART_USB, bufferMsg);
		}
		else
		{
			//se imprime la invalidacion de algun parametro del comando
			writeMsgForTXE(&handler_USART_USB, "Valor no valido \n");
		}
	}

	//Establece la hora, minutos, y segundos del RTC
	else if (strcmp(cmd, "configTime") == 0)
	{
		if (firtsParameter>=0 && firtsParameter<=23 && secondParameter>=0 && secondParameter<=59 && thirdParameter>=0 && thirdParameter<=59)
		{
			//Actualizamos el tiempo
			 updatetimeConfigRTC(&handler_RTC, firtsParameter, secondParameter, thirdParameter);
			//Se imprime la validacion de la accion
			writeMsgForTXE(&handler_USART_USB, "Nuevo tiempo establecido \n");

		}
		else
		{
			//se imprime la invalidacion de algun parametro del comando
			writeMsgForTXE(&handler_USART_USB, "Alguno o todos los valores no son validos \n");
		}
	}


	//Establece el año, mes, fecha y dia de la semana del RTC
	else if (strcmp(cmd, "configDate") == 0)
	{
		if (firtsParameter>=0 && firtsParameter<=99 && secondParameter>=1 && secondParameter<=12 && thirdParameter>=1 && thirdParameter<=numberDayMonth[secondParameter-1])
		{
			//convertimos el dia de la semana en numero
			uint8_t numberdayweek = numberDayWeed(userMsg);
			//verificamos la validacion del numero
			if(numberdayweek>0 && numberdayweek<=7)
			{
				//Actualizamos de la fecha
				updateDateConfigRTC(&handler_RTC, firtsParameter, secondParameter, thirdParameter, numberdayweek);
				//Se imprime la validacion de la accion
				writeMsgForTXE(&handler_USART_USB, "Nuevo fecha establecido \n");
			}
			else
			{
				//se imprime la invalidacion de algun parametro del comando
				writeMsgForTXE(&handler_USART_USB, "Dia de la semana no especificado correctamente \n");
			}
		}
		else
		{
			//se imprime la invalidacion de algun parametro del comando
			writeMsgForTXE(&handler_USART_USB, "Alguno o todos los valores no son validos \n");
		}
	}

	//Muestra el tiempo establecida en el RTC
	else if (strcmp(cmd, "showTime") == 0)
	{
		//Leemos el registro del tiempo
		initTimeShowRTC();
		//De dicho registro obtenemos los valores equivalentes a las horas, minutos y segundos
		uint8_t segShow = segShowRTC();
		uint8_t minShow = minShowRTC();
		uint8_t hourShow = hourShowRTC();
		//Mostrar el tiempo establecido en el RTC
		sprintf(bufferMsg,"Reloj: %u : %u : %u  \n", hourShow, minShow, segShow);
		writeMsgForTXE(&handler_USART_USB, bufferMsg);
	}

	//Muestra la fecha establecida en el RTC
	else if (strcmp(cmd, "showDate") == 0)
	{
		//Leemos el registro de la fecha
		initDateShowRTC();
		//De dicho registro obtenemos los valores equivalentes a los años, meses, dia del mes y dia de la semana
		uint8_t yearShow = yearShowRTC();
		uint8_t monthShow = monthShowRTC();
		uint8_t dateShow = dateShowRTC();
		uint8_t dayweekShow = dayweekShowRTC();
		//Mostrar el tiempo establecido en el RTC
		sprintf(bufferMsg,"Fecha: %u / %u / %u ", yearShow , monthShow, dateShow);
		writeMsgForTXE(&handler_USART_USB, bufferMsg);
		//Por ultimo mostramos el dia de la semana
		txdayWeed(dayweekShow);
	}

	else if (strcmp(cmd, "speedADC") == 0)
	{
		if(firtsParameter>=3000 && firtsParameter<=105000)
		{
			//Obtenemos el periodo
			uint8_t peridPwm = (1000000/firtsParameter);
			//Cargamos el valor del periodo y atualizamos el valor del DuttyCycle
			updateFrequency(&handler_PWM_1,  peridPwm);
//			updateDuttyCyclePercentage(&handler_PWM_1, 30);
			//Enviamos mensaje de validacion
			writeMsgForTXE(&handler_USART_USB, "Se actualizo la velocidad de muestreo del ADC \n");
		}
		else
		{
			//se imprime la invalidacion del parametro
			writeMsgForTXE(&handler_USART_USB, "Valor de frecuencia fuera de rango \n");
		}
	}


	else if (strcmp(cmd, "startADC") == 0)
	{
		//se el inicio del muestreo
		writeMsgForTXE(&handler_USART_USB, "Muestreo del ADC iniciado \n");
		//Activar el TIMER y con ello el PWM
		startPwmSignal(&handler_PWM_1);
	}

	else if (strcmp(cmd, "startAccel") == 0)
	{
		//se imprime el inicio del muestreo
		writeMsgForTXE(&handler_USART_USB, "Muestreo del acelerometro iniciado \n");
		writeMsgForTXE(&handler_USART_USB, "Tomando datos... \n");
		//Activamos la bandera para empezar el muestreo
		stateSampling_Accel = 1;
	}

	else if (strcmp(cmd, "startFFT") == 0)
	{
		//se imprime  la ejecucion del FFT
		writeMsgForTXE(&handler_USART_USB, "Ejecucion de la FFT sobre los datos muestreados: \n");
		//Activamos la bandera para ejecutar la FFT
		stateFFT = 1;
	}

	else
	{
		writeMsgForTXE(&handler_USART_USB, "Comando no correcto \n");
		//Se imprime que el comando no fue valido
	}


}


//----------------------------Fin de la definicion de las funciones de los comandos----------------------------------------





//----------------------------Inicio de la definicion de las funciones ISR---------------------------------------

//-------------------------Acelerometro_Muestreo--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM4
void BasicTimer2_Callback(void)
{
	 stastusTimer_200Hz = 1;
}

//-------------------------BlinkyLed--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
void BasicTimer4_Callback(void)
{
	GPIOxTooglePin(&handler_BlinkyPin);
}

//-------------------------USARTRX--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el USART2
void BasicUSART1_Callback(void)
{
	//Guardamos el caracter recibido
	charRead = getRxData();
	//Funcion que almacena los caracteres del comando recibido
	recepcionCommand();
}

//-------------------------ADC--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el ADC
void adcComplete_Callback(void)
{
	//Verificamos que se toman los 256
	if(adcPosArreglo<256)
	{
		//Verificamos en cual arreglo se depositaran los datos
		if(adcArregloChannel==0)
		{
			adcDataChannel_0[adcPosArreglo] = getADC();
			adcArregloChannel++;          //Sumamos a la variable
		}
		else
		{
			adcDataChannel_1[adcPosArreglo] = getADC();
			adcArregloChannel=0;     //Reiniciamos la variable
			adcPosArreglo++;		 //Sumamos a la variable
		}
	}
	else
	{
		stastusTxADC = 1;               //Establecemos la bandera
		adcPosArreglo = 0;              //Reiniciamos la variable
		stopPwmSignal(&handler_PWM_1);  //Desactivamos el ADC por medio del PWM
	}
}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------



//----------------------------Inicio de la definicion de las funciones-----------------------------------------

void acelerometro_I2C(void)
{
	//Reseteamos el registro PWR_MGHT_L con el fin de asegurar que no se presente en el acelerometro en modo SLEEP o CYCLE
	i2c_WriteSingleRegister(&handler_I2C_Acelerometro, PWR_MGMT_l, 0x00);
	//Aceleracion del eje z
	uint8_t AccelZ_high = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, ACCEL_ZOUT_H);
	uint8_t AccelZ_low = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, ACCEL_ZOUT_L);
	uint16_t AccelZ = AccelZ_high<<8 | AccelZ_low;
	//Verificamos el valor del contador
	if(countingSampling<size_Sampling)
	{
		//Almaceno los datos de los eje z en el arreglo especifico para realizar la captura de datos
		accelSampling_Z[countingSampling] =  convAccel(AccelZ);
		//Aunmento el valor del contador
		countingSampling++;
	}
	else
	{
		countingSampling = 0;            //Reiniciamos el contador
		stateSampling_Accel = 0;          //Definimos el estado dos del muestreo
		//Imprimir mensaje de fin de muestreo
		writeMsgForTXE(&handler_USART_USB, "Toma de datos finalizada \n");
	}
}

//Funcion que almacena los datos capturados por el ADC y luego los envia por comunicacion serial.
void conditionTxADC(void)
{
	char bufferMsg[64] = {0};

	//Por medio de una variable controlamos el tiempo para enviar exitosamente los datos por el ADC
	if(timerTxADC>10)
	{
		//Verificamos el valor del contador
		if(countingTxADC<256)
		{
			//Enviamos mensaje
			sprintf(bufferMsg,"Channel_1: %#.2f, Channel_2: %#.2f \n", ((adcDataChannel_0[countingTxADC]*3.3)/4095), (adcDataChannel_1[countingTxADC]*3.3)/4095);
			writeMsgForTXE(&handler_USART_USB, bufferMsg);
			//Aunmento el valor del contador
			countingTxADC++;
			//reiniciamos la variable
			timerTxADC = 0;
		}
		else
		{
			//reiniciamos las variables
			timerTxADC= 0;
			countingTxADC = 0;
			stastusTxADC = 0;
		}
	}
	else
	{
		timerTxADC++;
	}
}

//Funcion ejecutamos la FFT o enviamos los datos de la FFT
void conditionFFT(void)
{
	//Definimos un string
	char bufferMsg[64] = {0};

	//Verificamos el estado de la bandera
	if(stateFFT==1)
	{
		//Ejecutamos la FFT
		executionFFT();
	}
	else if(stateFFT==2)
	{
		//Por medio de una variable controlamos el tiempo para enviar exitosamente los datos de la arreglo de la FFT
		if(timerTxFFT>0)
		{
			//Verificamos el valor del contador
			if(countingTxFFT<(size_Sampling/2))
			{
				//Enviamos mensaje
				sprintf(bufferMsg,"Valor de la FFT: %#.2f \n", dateFFTSampling[countingTxFFT]);
				writeMsgForTXE(&handler_USART_USB, bufferMsg);
				//Aunmento el valor del contador
				countingTxFFT++;
				//reiniciamos la variable
				timerTxFFT = 0;
			}
			else
			{
				//Mostramos la frecuencia dominante con la conversion
				sprintf(bufferMsg,"Frecuencia Dominante: %#.4f Hz \n",  (float32_t) (dateDominantFreq*200)/(fftSize));
				writeMsgForTXE(&handler_USART_USB, bufferMsg);
				//reiniciamos las variables;
				timerTxFFT= 0;
				countingTxFFT = 0;
				stateFFT = 0;
			}
		}
		else
		{
			timerTxFFT++;
		}
	}
	else
	{
		__NOP();
	}
}

//Funcion que ejecuta la FFT
void executionFFT(void)
{
	float32_t complexsetFFT[size_Sampling] = {0};                   //Arreglo que guarda los elementos complejos de la FFT
	float32_t abscomplexsetFFT[size_Sampling] = {0};                //Arreglo que guarda los elementos complejos de la FFT despues de aplicarles valor absoluto
	arm_status statusInitFFT = ARM_MATH_ARGUMENT_ERROR;             //variable para indicar la correcta modificacio de la strut
	arm_rfft_fast_instance_f32  config_Rfft_fast_f32 = {0};         //Struct de tipo arm_rfft_fast_instance_f32 que contiene la configuracion para la FFT
	uint32_t posDateFFT = 0;                                        //Variable que recorre las posiciones del arreglo DateFFTSampling

	//Cargamos la configuracion indicada a la struct de tipo arm_rfft_fast_instance_f32
	statusInitFFT = arm_rfft_fast_init_f32(&config_Rfft_fast_f32, fftSize);

	//Verificacion de la correcta modificacion de la struct
	if (statusInitFFT == ARM_MATH_SUCCESS)
	{
		//Realizamos la FFT, recibe un arreglo de tipo float32_t de elementos reales y entrega un arreglo de elementos complejos
		//Elementos:  X = { real[0], imag[0], real[1], imag[1], real[2], imag[2] ... real[(N/2)-1], imag[(N/2)-1}
		arm_rfft_fast_f32(&config_Rfft_fast_f32, accelSampling_Z, complexsetFFT, 0);
		//Funcion que devuelve el valor asolucto de un arreglo
		arm_abs_f32(complexsetFFT, abscomplexsetFFT, fftSize);
		//Tomamos los valores reales
		for(uint32_t i=0; i<size_Sampling; i++)
		{
			if (i % 2)
			{
				//Guardamos los valores reales en un arr2048eglo global ademas de la amplitud del armonico
				dateFFTSampling[posDateFFT] = 2*abscomplexsetFFT[i];
				//Sumamos a la variable
				posDateFFT++;
			}
		}
		//Definimos la frecuencia dominate sin conversion
		arm_max_f32(dateFFTSampling, posDateFFT,  &valueMaxFFT, &dateDominantFreq);
		//Cambiamos el estado de la bandera
		stateFFT=2;
	}
	else
	{
		//Reiniciamos la bandera
		stateFFT = 0;
		//Enviamos mensaje
		writeMsgForTXE(&handler_USART_USB, "Error en la configuracion de la FFT \n");
	}
}

//Funcion que convierte los valores guardados entregados por el acelerometro en valores en unidades m/s2
float32_t convAccel(int16_t accelx)
{
	float32_t value = (accelx*9.8/16384);

	return value;
}


//Funcion que retorna el dia de la semana en numero
uint8_t numberDayWeed(char *msg)
{
	uint8_t number = 0;
	if (strcmp(msg, "lunes")==0)
	{
		number = LUNES;
	}
	else if (strcmp(msg, "martes")==0)
	{
		number = MARTES;
	}
	else if (strcmp(msg, "miercoles")==0)
	{
		number = MIERCOLES;
	}
	else if (strcmp(msg, "jueves")==0)
	{
		number = JUEVES;
	}
	else if (strcmp(msg, "viernes")==0)
	{
		number = VIERNES;
	}
	else if (strcmp(msg, "sabado")==0)
	{
		number = SABADO;
	}
	else if (strcmp(msg, "domingo")==0)
	{
		number = DOMINGO;
	}
	else
	{
		number = 0;
	}
	return number;
}

//Funcion que envia por usart el dia de la semana
void txdayWeed(uint8_t number)
{
	switch (number)
	{
	case 1:
	{
		writeMsgForTXE(&handler_USART_USB, "dia: lunes \n");
		break;
	}
	case 2:
	{
		writeMsgForTXE(&handler_USART_USB, "dia: martes \n");
		break;
	}
	case 3:
	{
		writeMsgForTXE(&handler_USART_USB, "dia: miercoles \n");
		break;
	}
	case 4:
	{
		writeMsgForTXE(&handler_USART_USB, "dia: jueves \n");
		break;
	}
	case 5:
	{
		writeMsgForTXE(&handler_USART_USB, "dia: viernes \n");
		break;
	}
	case 6:
	{
		writeMsgForTXE(&handler_USART_USB, "dia: sabado \n");
		break;
	}
	case 7:
	{
		writeMsgForTXE(&handler_USART_USB, "dia: domingo \n");
		break;
	}
	default:
	{
			__NOP();
		break;
	}
	}
}

//--------------------------  --Fin de la definicion de las funciones------------------------------------------

