/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Solucion Tarea Especial
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
#include <dsp/basic_math_functions.h>
#include <LCD_I2CDriver.h>

//-----------------------------------Fin de definicion de librerias------------------------------------------

GPIO_Handler_t handler_GPIO_Prueba = {0};       //Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB

//---------------------------Inicio de definicion de funciones y variables base----------------------------------

GPIO_Handler_t handler_BlinkyPin = {0};         //Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
BasicTimer_Handler_t handler_BlinkyTimer ={0};  //Definimos un elemento del tipo GPIO_Handler_t (Struct)
void int_Hardware(void);                        //Definimos la cabecera para la configuracion

//---------------------------Fin de definicion de funciones y variables base----------------------------------

//--------------------------USART USB-------------------------------
GPIO_Handler_t handler_GPIO_USB_TX = {0};       //Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB
GPIO_Handler_t handler_GPIO_USB_RX = {0};
USART_Handler_t handler_USART_USB = {0};
char charRead = '\0';                          //Variable que almacena el caracter leido

//--------------------------PWM-------------------------------
GPIO_Handler_t handler_GPIO_PWM_1 = {0};       //Definimos un elemento del tipo GPIO_Handler_t (Struct) y PWM_Handler_t para la generan de la señal PWM
PWM_Handler_t handler_PWM_1 = {0};
GPIO_Handler_t handler_GPIO_PWM_2 = {0};
PWM_Handler_t handler_PWM_2 = {0};
GPIO_Handler_t handler_GPIO_PWM_3 = {0};
PWM_Handler_t handler_PWM_3 = {0};
uint8_t duttyporc = 0;                         //Variables para cambiar el duttycicle
uint8_t estado = 0;

//-------------------------Acelerometro----------------------------
GPIO_Handler_t handler_GPIO_SCL_Acelerometro = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) y I2C_Handler_t para la comunicacion I2C
GPIO_Handler_t handler_GPIO_SDA_Acelerometro = {0};
I2C_Handler_t handler_I2C_Acelerometro = {0};
BasicTimer_Handler_t handler_TIMER_Muestreo = {0};     //Definimos un elemento del tipo GPIO_Handler_t (Struct) para realizar el muestreo a 1Khz
uint8_t stateSampling = 0;                             //Variable que controla el estado del muestreo de los ejes
int16_t accel[3] = {0,0,0};                            //Arreglo que guarda la aceleracion de cada eje
int16_t  configAccel[2] = {0,0};                      //Arreglo que almacena la configuracion del acelerometro
int16_t accelTxSampling[2000][3] = {0};                 //Variable que controla el estado del muestreo de los ejes
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

//------------------Definiciones generales----------------------------------
void duttyAccel(PWM_Handler_t *prtPwmHandler, int16_t valueAccel); //Definimos la cabecera de la funcion encargada de relacionar el valor del accelx con el dutty del PWMx
void conditionTxSampling(void);           //Funcion que almacena los datos capturados por 2s y luego los envia por comunicacion serial.
void transmitAccel(char dataRead);       //Funcion que transmite por el puerto serial la informacion contenida en el arreglo del accel
float convAccel(int16_t accelx);          //Funcion que convierte los valores guardados entregados por el acelerometro en valores en unidades m/s2
void valueconfigacel(void);               //Funcion que obtenie los valores de la configuracion
uint8_t statusTxSampling = 0;             //variable que activa la transmision del muestreo de datos
uint16_t countingTXSampling = 0;          //variable que cuenta las veces que se a mandado el a enviado el muestreo
uint8_t  timerTXSampling = 0;              //Variable para controlar el tiempo con que se envia los mensajes por USART
uint16_t timerLCDSampling = 0;             //Varibale que controla el tiempo con que se muestra los datos en la pantalla LCD

int main(void)
{
	//Incrementamos la velocidad de reloj del sistema
	uint8_t clock = CLOCK_SPEED_80MHZ;    //Velocidad de reloj entre 25 o 100 MHz
	configPLL(clock);
	//Realizamos la configuracuion inicial
	int_Hardware();

	//Activamos el punto flotante por medio del registro especifico
	SCB->CPACR |= 0xF <<20;
	//Obtenemos los valores de configuracion
	valueconfigacel();
	//Definimos para el PIN un 1 logico,
	GPIO_writePin (&handler_BlinkyPin, SET);

	while(1)
	{
		if(stateSampling == 1)
		{

			GPIO_writePin (&handler_GPIO_Prueba, SET);
			//Realizamos el muestreo de los 3 ejes de accel
			acelerometro_I2C();
			//Cambiamos el valor del dutty deacuerdo al valor del accel x
			duttyAccel(&handler_PWM_1, accel[0]);
			duttyAccel(&handler_PWM_2, accel[1]);
			duttyAccel(&handler_PWM_3, accel[2]);
			//Almacenamos o enviamos los datos muestreados
			conditionTxSampling();
			//Reiniciamos la variable
			GPIO_writePin (&handler_GPIO_Prueba, RESET);
			stateSampling = 0;
		}
		else if (charRead != '\0' && charRead != 'a')
		{
			//Transmitimos de acuerdo al caracter dado
			transmitAccel(charRead);
			//Definimos el elemento nulo
			charRead = '\0';
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

	//---------------------------USB--------------------------------
	//---------------PIN: PA9----------------
	//------------AF7: USART1_TX----------------
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

	//---------------------------PWM--------------------------------

	//---------------PIN: PB4----------------
	//------------AF2: TIM3_CH1----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_PWM_1.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_PWM_1.GPIO_PinConfig.GPIO_PinNumber = PIN_4; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_PWM_1, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF2);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_PWM_1);

	//---------------PIN: PB5----------------
	//------------AF2: TIM3_CH2----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_PWM_2.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_PWM_2.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_PWM_2, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF2);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_PWM_2);

	//---------------PIN: PB1----------------
	//------------AF2: TIM3_CH4----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_PWM_3.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_PWM_3.GPIO_PinConfig.GPIO_PinNumber = PIN_1; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_PWM_3, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF2);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_PWM_3);

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
	handler_TIMER_Muestreo.ptrTIMx = TIM2;
	//Definimos la configuracion del TIMER seleccionado
	handler_TIMER_Muestreo.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_10us; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handler_TIMER_Muestreo.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handler_TIMER_Muestreo.TIMx_Config.TIMX_period = 100;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handler_TIMER_Muestreo);

	//---------------TIM4----------------
	//Definimos el TIMx a usar
	handler_BlinkyTimer.ptrTIMx = TIM4;
	//Definimos la configuracion del TIMER seleccionado
	handler_BlinkyTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
	handler_BlinkyTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
	handler_BlinkyTimer.TIMx_Config.TIMX_period = 250;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	//Cargamos la configuracion del TIMER especifico
	BasicTimer_Config(&handler_BlinkyTimer);

	//-------------------Fin de Configuracion TIMx-----------------------


	//-------------------Inicio de Configuracion PWM_Channelx----------------------

	//---------------TIM3_Channel_1----------------
	//Definimos el TIMx a usar
	handler_PWM_1.ptrTIMx = TIM3;
	//Definimos la configuracion para el PWM
	handler_PWM_1.config.periodcnt = BTIMER_PCNT_1us; //BTIMER_PCNT_xus x->1,10,100/ BTIMER_PCNT_1ms
	handler_PWM_1.config.periodo = 16382;             //Al definir 1us, 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	handler_PWM_1.config.channel = PWM_CHANNEL_1;     //PWM_CHANNEL_x x->1,2,3,4
	handler_PWM_1.config.duttyCicle = 8191;          //Valor entre 0-100 [%]
	//Cargamos la configuracion
	pwm_Config(&handler_PWM_1);
	//Activar el TIMER y con ello el PWM
	startPwmSignal(&handler_PWM_1);

	//---------------TIM3_Channel_2----------------
	//Definimos el TIMx a usar
	handler_PWM_2.ptrTIMx = TIM3;
	//Definimos la configuracion para el PWM
	handler_PWM_2.config.periodcnt = BTIMER_PCNT_1us; //BTIMER_PCNT_xus x->1,10,100/ BTIMER_PCNT_1ms
	handler_PWM_2.config.periodo = 16382;             //Al definir 1us, 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	handler_PWM_2.config.channel = PWM_CHANNEL_2;     //PWM_CHANNEL_x x->1,2,3,4
	handler_PWM_2.config.duttyCicle = 8191;          //Valor entre 0-100 [%]
	//Cargamos la configuracion
	pwm_Config(&handler_PWM_2);
	//Activar el TIMER y con ello el PWM
	startPwmSignal(&handler_PWM_2);

	//---------------TIM3_Channel_4----------------
	//Definimos el TIMx a usar
	handler_PWM_3.ptrTIMx = TIM3;
	//Definimos la configuracion para el PWM
	handler_PWM_3.config.periodcnt = BTIMER_PCNT_1us; //BTIMER_PCNT_xus x->1,10,100/ BTIMER_PCNT_1ms
	handler_PWM_3.config.periodo = 16382;             //Al definir 1us, 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
	handler_PWM_3.config.channel = PWM_CHANNEL_4;     //PWM_CHANNEL_x x->1,2,3,4
	handler_PWM_3.config.duttyCicle = 8191;          //Valor entre 0-100 [%]
	//Cargamos la configuracion
	pwm_Config(&handler_PWM_3);
	//Activar el TIMER y con ello el PWM
	startPwmSignal(&handler_PWM_3);

	//---------------------Fin de Configuracion PWM_Channelx-----------------------

	//-------------------Inicio de Configuracion I2Cx----------------------

	//---------------I2C1----------------
	//Definimos el I2Cx a usar
	handler_I2C_Acelerometro.prtI2Cx = I2C1;
	//Definimos la configuracion para el I2C
	handler_I2C_Acelerometro.modeI2C = I2C_MODE_FM;               //I2C_MODE_x  x->SM,FM
	handler_I2C_Acelerometro.slaveAddress = ACCEL_ADDRESSS_ACCEL;       //Direccion del Sclave
	//Cargamos la configuracion
	i2c_Config(&handler_I2C_Acelerometro);

	//---------------------Fin de Configuracion I2Cx----------------------
}
//------------------------------Fin Configuracion del microcontrolador------------------------------------------


//----------------------------Inicio de la definicion de las funciones ISR---------------------------------------

//-------------------------BlinkyLed--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
void BasicTimer2_Callback(void)
{
	stateSampling = 1;
}

//-------------------------Acelerometro_Muestreo--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM4
void BasicTimer4_Callback(void)
{
	GPIOxTooglePin(&handler_BlinkyPin);
}

//-------------------------USARTRX--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el USART2
void BasicUSART1_Callback(void)
{
	//Verificamos si el elemento nulo esta activo
	if (charRead == '\0')
	{
		//Guardamos el caracter recibido
		charRead = getRxData();
		//Verificamos si el carater recibido es 'a'
		if(charRead == 'a')
		{
			statusTxSampling = 1;
		}
		else
		{
			__NOP();
		}
	}
	else
	{
		__NOP();
	}

}

//----------------------------Fin de la definicion de las funciones ISR----------------------------------------

//----------------------------Inicio de la definicion de las funciones-----------------------------------------


//Funcion que obtenie los valores de la configuracion
void valueconfigacel(void)
{
	//obtenemos la sensibilidad de lacelerometro
	uint8_t afsSel = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, AFS_SEL);
	switch(((afsSel >>3) & 0b11))
	{
	case 0:
	{
		configAccel[0] = 2;
		configAccel[1] = 16384;
		break;
	}
	case 1:
	{
		configAccel[0] = 4;
		configAccel[1] = 8192;
		break;
	}
	case 2:
	{
		configAccel[0] = 8;
		configAccel[1] = 4096;
		break;
	}
	case 3:
	{
		configAccel[0] = 16;
		configAccel[1] = 2048;
		break;
	}
	default:
		configAccel[0] = 2;
		break;
	}

}

void acelerometro_I2C(void)
{
	//Reseteamos el registro PWR_MGHT_L con el fin de asegurar que no se presente en el acelerometro en modo SLEEP o CYCLE
	i2c_WriteSingleRegister(&handler_I2C_Acelerometro, PWR_MGMT_l, 0x00);
	//Aceleracion del eje x
	uint8_t AccelX_high = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, ACCEL_XOUT_H);
	uint8_t AccelX_low = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, ACCEL_XOUT_L);
	uint16_t AccelX = AccelX_high<<8 | AccelX_low;
	accel[0] = (int16_t) AccelX;
	//Aceleracion del eje y
	uint8_t AccelY_high = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, ACCEL_YOUT_H);
	uint8_t AccelY_low = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, ACCEL_YOUT_L);
	uint16_t AccelY = AccelY_high<<8 | AccelY_low;
	accel[1] = (int16_t) AccelY;
	//Aceleracion del eje z
	uint8_t AccelZ_high = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, ACCEL_ZOUT_H);
	uint8_t AccelZ_low = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, ACCEL_ZOUT_L);
	uint16_t AccelZ = AccelZ_high<<8 | AccelZ_low;
	accel[2] = (int16_t) AccelZ;
}

//Funcion que almacena los datos capturados por 2s y luego los envia por comunicacion serial.
void conditionTxSampling(void)
{
	if(statusTxSampling == 1)
	{
		//Verificamos el valor del contador
		if(countingTXSampling<2000)
		{
			//Almaceno los datos de los ejes en el arreglo especifico para realizar la captura de datos
			accelTxSampling[countingTXSampling][0] = accel[0];
			accelTxSampling[countingTXSampling][1] = accel[1];
			accelTxSampling[countingTXSampling][2] = accel[2];
			//Aunmento el valor del contador
			countingTXSampling++;
		}
		else
		{
			countingTXSampling = 0;        //Reiniciamos el contador
			statusTxSampling = 2;          //Definimos el estado dos del muestreo
		}
	}
	else if (statusTxSampling==2)
	{
		//Por medio de una variable controlamos el tiempo para enviar exitosamente el mesanje con los muestreos
		if(timerTXSampling>10)
		{
			//Verificamos el valor del contador
			if(countingTXSampling<2000)
			{
				//Transmitimos de acuerdo al caracter dado
				transmitAccel(charRead);
				//Aunmento el valor del contador
				countingTXSampling++;
				//reiniciamos la variable
				timerTXSampling = 0;
			}
			else
			{
				//reiniciamos las variables
				timerTXSampling = 0;
				countingTXSampling = 0;
				statusTxSampling = 0;
				charRead = '\0';
			}
		}
		else
		{
			timerTXSampling++;
		}
	}
	else
	{
		__NOP();
	}
}


//Funcion que actualiza el valor del dutty de acuerdo al valor del accel correnpondiente a un eje
void duttyAccel(PWM_Handler_t *prtPwmHandler, int16_t valueAccel)
{
	int32_t value = 8191 + (valueAccel/4);
	uint16_t dutty = (uint16_t) value;
	updateDuttyCycle(prtPwmHandler, dutty);
}

//Funcion que transmite por el puerto serial la informacion contenida en el arreglo del accel
void transmitAccel(char dataRead)
{
	char bufferMsg[64] = {0};                      //Variable que almacena un strings

	//Dependiendo del caso envia un string diferente
	switch(dataRead)
	{
	case 'x':
	{
		//Creo un string con el valor de la aceleracion
		sprintf(bufferMsg,"Aceleracion Eje X: %#.2f m/s^2 \n", convAccel(accel[0]));
		//Envio el string por comunicacion Serial
		writeMsgForTXE(&handler_USART_USB, bufferMsg);
		break;
	}
	case 'y':
	{
		//Creo un string con el valor de la aceleracion
		sprintf(bufferMsg,"Aceleracion Eje Y: %#.2f m/s^2 \n", convAccel(accel[1]));
		//Envio el string por comunicacion Serial
		writeMsgForTXE(&handler_USART_USB, bufferMsg);
		break;
	}
	case 'z':
	{
		//Creo un string con el valor de la aceleracion
		sprintf(bufferMsg,"Aceleracion Eje Z: %#.2f m/s^2 \n", convAccel(accel[2]));
		//Envio el string por comunicacion Serial
		writeMsgForTXE(&handler_USART_USB, bufferMsg);
		break;
	}
	case 'a':
	{
	   //Creo un string con el valor de la aceleracion
		sprintf(bufferMsg,"Dato #%d: (%#.2f; %#.2f; %#.2f) [m/s^2] \n",  (countingTXSampling+1), convAccel(accelTxSampling[countingTXSampling][0]), convAccel(accelTxSampling[countingTXSampling][1]), convAccel(accelTxSampling[countingTXSampling][2]));
	   //Envio el string por comunicacion Serial
		writeMsgForTXE(&handler_USART_USB, bufferMsg);

		break;
	}
	case 's':
	{
		//Creo un string con el valor de la aceleracion
		sprintf(bufferMsg,"Rango de sensibilidad: %d g, %d LSB/g \n", configAccel[0],configAccel[1]);
		//Envio el string por comunicacion Serial
		writeMsgForTXE(&handler_USART_USB, bufferMsg);
		break;
	}
	case 'w':
	{
		//Leemos el registro deseado del Sclave
		uint8_t i2cBuffer = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, WHO_AM_I);
		//Definimos un string
		sprintf(bufferMsg,"Direccion Acelerometro: 0x%x \n", (unsigned int) i2cBuffer);
		//Enviamos por puerto serial dicho string
		writeMsg(&handler_USART_USB, bufferMsg);
		break;
	}
	case 'p':
	{
		//Leemos el registro deseado del Sclave
		uint8_t i2cBuffer = i2c_ReadSingleRegister(&handler_I2C_Acelerometro, PWR_MGMT_l);
		//Definimos un string
		sprintf(bufferMsg,"Estado PWR_MGMT_l: 0x%x \n", (unsigned int) i2cBuffer);
		//Enviamos por puerto serial dicho string
		writeMsg(&handler_USART_USB, bufferMsg);
		break;
	}
	default:
	{
		__NOP();
		break;
	}
	}
}

//Funcion que convierte los valores guardados entregados por el acelerometro en valores en unidades m/s2
float32_t convAccel(int16_t accelx)
{
	float32_t value = (accelx*9.8/16384);

	return value;
}


//--------------------------  --Fin de la definicion de las funciones------------------------------------------

