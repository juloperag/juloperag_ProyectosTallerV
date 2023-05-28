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
#include <USARTxDriver.h>
#include <I2CDriver.h>

//-----------------------------------Fin de definicion de librerias------------------------------------------


//---------------------------Inicio de definicion de funciones y variables base----------------------------------

//GPIO_Handler_t handler_BlinkyPin = {0};         //Definimos un elemento del tipo GPIO_Handler_t (Struct) para el LED
//BasicTimer_Handler_t handler_BlinkyTimer ={0};  //Definimos un elemento del tipo GPIO_Handler_t (Struct)
void int_Hardware(void);                        //Definimos la cabecera para la configuracion

//---------------------------Fin de definicion de funciones y variables base----------------------------------

//
////--------------------------USART-------------------------------
//GPIO_Handler_t handler_GPIO_USB_TX = {0};       //Definimos un elemento del tipo GPIO_Handler_t (Struct) y USART_Handler_t para el uso del USB
//GPIO_Handler_t handler_GPIO_USB_RX = {0};
//USART_Handler_t handler_USART_USB = {0};
char charRead = 'w';                            //Variable que almacena el caracter leido
//char bufferMsg[64] = {0};


//-------------------------I2C--------------------------
GPIO_Handler_t handler_GPIO_SCL_Acelerometro = {0};   //Definimos un elemento del tipo GPIO_Handler_t (Struct) y I2C_Handler_t para la comunicacion I2C
GPIO_Handler_t handler_GPIO_SDA_Acelerometro = {0};
I2C_Handler_t handler_I2C_Acelerometro = {0};
//-------Acelerometro
#define ACCEL_ADDRESSS  0b1101000;                   //Definicion de la direccion del Sclave
#define ACCEL_XOUT_H  0x3B                           //Definicion de la direccion de los registros del Sclave a usar
#define ACCEL_XOUT_L  0x3C
#define ACCEL_YOUT_H  0x3D
#define ACCEL_YOUT_L  0x3E
#define ACCEL_ZOUT_H  0x3F
#define ACCEL_ZOUT_L  0x40

#define PWR_MGMT_l  107                            //Hay un bit en dicho registro para aplicar reset
#define WHO_AM_I    117                            //Registro para verificar la identidad del equipo

void acelerometro_I2C(void);                       //Cabecera para la comunicacion I2C

int main(void)
{
	//Realizamos la configuracuion inicial
	int_Hardware();
	//Definimos para el PIN un 1 logico,
//	GPIO_writePin (&handler_BlinkyPin, SET);

	while(1)
	{
		if(charRead != '\0')
		{
			acelerometro_I2C();
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

	//---------------------------BlinkyLed--------------------------------
	//---------------PIN: PA5----------------
//	//Definimos el periferico GPIOx a usar.
//	handler_BlinkyPin.pGPIOx = GPIOA;
//	//Definimos el pin a utilizar
//	handler_BlinkyPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5; 						//PIN_x, 0-15
//	//Definimos la configuracion de los registro para el pin seleccionado
//	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
//	GPIO_PIN_Config(&handler_BlinkyPin, GPIO_MODE_OUT, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF0);
//	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
//	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
//	//Cargamos la configuracion del PIN especifico
//	GPIO_Config(&handler_BlinkyPin);

	//---------------------------USART--------------------------------
	//---------------PIN: PA2----------------
	//------------AF7: USART2_TX----------------
//	//Definimos el periferico GPIOx a usar.
//	handler_GPIO_USB_TX.pGPIOx = GPIOA;
//	//Definimos el pin a utilizar
//	handler_GPIO_USB_TX.GPIO_PinConfig.GPIO_PinNumber = PIN_2; 						//PIN_x, 0-15
//	//Definimos la configuracion de los registro para el pin seleccionado
//	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
//	GPIO_PIN_Config(&handler_GPIO_USB_TX, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF7);
//	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
//	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
//	//Cargamos la configuracion del PIN especifico
//	GPIO_Config(&handler_GPIO_USB_TX);

	//---------------PIN: PA3----------------
	//------------AF7: USART2_RX----------------
	//Definimos el periferico GPIOx a usar.
//	handler_GPIO_USB_RX.pGPIOx = GPIOA;
//	//Definimos el pin a utilizar
//	handler_GPIO_USB_RX.GPIO_PinConfig.GPIO_PinNumber = PIN_3; 						//PIN_x, 0-15
//	//Definimos la configuracion de los registro para el pin seleccionado
//	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
//	GPIO_PIN_Config(&handler_GPIO_USB_RX, GPIO_MODE_ALTFN, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_MEDIUM, GPIO_PUPDR_NOTHING, AF7);
//	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
//	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
//	//Cargamos la configuracion del PIN especifico
//	GPIO_Config(&handler_GPIO_USB_RX);


	//---------------------------I2C--------------------------------
	//---------------PIN: PB10---------------
	//------------AF4: I2C2_SCL----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_SCL_Acelerometro.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_SCL_Acelerometro.GPIO_PinConfig.GPIO_PinNumber = PIN_10; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_SCL_Acelerometro, GPIO_MODE_ALTFN, GPIO_OTYPER_OPENDRAIN, GPIO_OSPEEDR_HIGH, GPIO_PUPDR_PULLUP, AF4);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_SCL_Acelerometro);

	//---------------PIN: PB3----------------
	//------------AF9: I2C2_SDA----------------
	//Definimos el periferico GPIOx a usar.
	handler_GPIO_SDA_Acelerometro.pGPIOx = GPIOB;
	//Definimos el pin a utilizar
	handler_GPIO_SDA_Acelerometro.GPIO_PinConfig.GPIO_PinNumber = PIN_3; 						//PIN_x, 0-15
	//Definimos la configuracion de los registro para el pin seleccionado
	// Orden de elementos: (Struct, Mode, Otyper, Ospeedr, Pupdr, AF)
	GPIO_PIN_Config(&handler_GPIO_SDA_Acelerometro, GPIO_MODE_ALTFN, GPIO_OTYPER_OPENDRAIN, GPIO_OSPEEDR_HIGH, GPIO_PUPDR_PULLUP, AF9);
	/*Opciones: GPIO_Tipo_x, donde x--->||IN, OUT, ALTFN, ANALOG ||| PUSHPULL, OPENDRAIN |||
	 * ||| LOW, MEDIUM, FAST, HIGH ||| NOTHING, PULLUP, PULLDOWN, RESERVED |||  AFx, 0-15 |||*/
	//Cargamos la configuracion del PIN especifico
	GPIO_Config(&handler_GPIO_SDA_Acelerometro);

	//-------------------Fin de Configuracion GPIOx-----------------------

	//-------------------Inicio de Configuracion USARTx-----------------------

//	//---------------USART2----------------
//	//Definimos el periferico USARTx a utilizar
//	handler_USART_USB.ptrUSARTx = USART2;
//	//Definimos la configuracion del USART seleccionado
//	handler_USART_USB.USART_Config.USART_mode = USART_MODE_RXTX ;           //USART_MODE_x  x-> TX, RX, RXTX, DISABLE
//	handler_USART_USB.USART_Config.USART_baudrate = USART_BAUDRATE_9600;  //USART_BAUDRATE_x  x->9600, 19200, 115200
//	handler_USART_USB.USART_Config.USART_parity= USART_PARITY_NONE;       //USART_PARITY_x   x->NONE, ODD, EVEN
//	handler_USART_USB.USART_Config.USART_stopbits=USART_STOPBIT_1;         //USART_STOPBIT_x  x->1, 0_5, 2, 1_5
//	handler_USART_USB.USART_Config.USART_enableIntRX = USART_RX_INTERRUP_ENABLE;   //USART_RX_INTERRUP_x  x-> DISABLE, ENABLE
//	handler_USART_USB.USART_Config.USART_enableIntTX = USART_TX_INTERRUP_DISABLE;   //USART_TX_INTERRUP_x  x-> DISABLE, ENABLE
//	//Cargamos la configuracion del USART especifico
//	USART_Config(&handler_USART_USB);

	//-------------------Fin de Configuracion USARTx-----------------------

	//-------------------Inicio de Configuracion TIMx-----------------------

//	//---------------TIM2----------------
//	//Definimos el TIMx a usar
//	handler_BlinkyTimer.ptrTIMx = TIM2;
//	//Definimos la configuracion del TIMER seleccionado
//	handler_BlinkyTimer.TIMx_Config.TIMx_periodcnt = BTIMER_PCNT_1ms; //BTIMER_PCNT_xus x->10,100/ BTIMER_PCNT_1ms
//	handler_BlinkyTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP; // BTIMER_MODE_x x->UP, DOWN
//	handler_BlinkyTimer.TIMx_Config.TIMX_period = 250;//Al definir 10us,100us el valor un multiplo de ellos, si es 1ms el valor es en ms
//	//Cargamos la configuracion del TIMER especifico
//	BasicTimer_Config(&handler_BlinkyTimer);

	//-------------------Fin de Configuracion TIMx-----------------------

	//-------------------Inicio de Configuracion I2Cx----------------------

	//Definimos el I2Cx a usar
	handler_I2C_Acelerometro.ptrI2Cx = I2C2;
	//Definimos la configuracion para el I2C
	handler_I2C_Acelerometro.modeI2C = I2C_MODE_FM;               //I2C_MODE_x  x->SM,FM
	handler_I2C_Acelerometro.slaveAddress = ACCEL_ADDRESSS;       //Direccion del Sclave
	handler_I2C_Acelerometro.freq = 16;
	//Cargamos la configuracion
	i2c_config(&handler_I2C_Acelerometro);

	//---------------------Fin de Configuracion I2Cx----------------------
}
//------------------------------Fin Configuracion del microcontrolador------------------------------------------


//----------------------------Inicio de la definicion de las funciones ISR---------------------------------------

//-------------------------BlinkyLed--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el TIM2
//void BasicTimer2_Callback(void)
//{
//	GPIOxTooglePin(&handler_BlinkyPin);
//}

//-------------------------USARTRX--------------------------------
//Definimos la funcion que se desea ejecutar cuando se genera la interrupcion por el USART2
//void BasicUSART2_Callback(void)
//{
//	charRead = getRxData();
//}


//----------------------------Fin de la definicion de las funciones ISR----------------------------------------

//----------------------------Inicio de la definicion de las funciones-----------------------------------------

void acelerometro_I2C(void)
{
	uint8_t i2cBuffer = 0;

	switch(charRead)
	{
	case 'w':
	{
		//Definimos un string
//		sprintf(bufferMsg,"WHO AM I (r)?\n");
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);
		//Leemos el registro deseado del Sclave
		i2cBuffer = i2c_readSingleRegister(&handler_I2C_Acelerometro, WHO_AM_I);
		//Definimos un string
//		sprintf(bufferMsg,"dataRead = 0x%x \n", (unsigned int) i2cBuffer);
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);
		charRead = '\0';
		break;
	}
	case 'p':
	{
		//Definimos un string
//		sprintf(bufferMsg,"PWR_MGMT_l state (r)\n");
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);
		//Leemos el registro deseado del Sclave
		i2cBuffer = i2c_readSingleRegister(&handler_I2C_Acelerometro, PWR_MGMT_l);
		//Definimos un string
//		sprintf(bufferMsg,"dataRead = 0x%x \n", (unsigned int) i2cBuffer);
		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);
		charRead = '\0';
		break;
	}
	case 'r':
	{
		//Definimos un string
//		sprintf(bufferMsg,"PWR_MGMT_l reset (r)\n");
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);
		//Escribimos en el registro deseado del Sclave
		i2c_writeSingleRegister(&handler_I2C_Acelerometro, PWR_MGMT_l, 0x00);

		charRead = '\0';
		break;
	}
	case 'x':
	{
		//Definimos un string
//		sprintf(bufferMsg,"Axis X data (r)\n");
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);
		//Leemos el registro correspondiente a la poscicion en X del acelerometro
		uint8_t AccelX_low = i2c_readSingleRegister(&handler_I2C_Acelerometro, ACCEL_XOUT_L);
		uint8_t AccelX_high = i2c_readSingleRegister(&handler_I2C_Acelerometro, ACCEL_XOUT_H);
		uint16_t AccelX = AccelX_high<<8 | AccelX_low;
		//Definimos un string
//		sprintf(bufferMsg,"AccelX = %d \n", (int) AccelX );
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);

		charRead = '\0';
		break;
	}
	case 'y':
	{
		//Definimos un string
//		sprintf(bufferMsg,"Axis Y data (r)\n");
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);
		//Leemos el registro correspondiente a la poscicion en X del acelerometro
		uint8_t AccelY_low = i2c_readSingleRegister(&handler_I2C_Acelerometro, ACCEL_YOUT_L);
		uint8_t AccelY_high = i2c_readSingleRegister(&handler_I2C_Acelerometro, ACCEL_YOUT_H);
		uint16_t AccelY = AccelY_high<<8 | AccelY_low;
		//Definimos un string
//		sprintf(bufferMsg,"AccelY = %d \n", (int) AccelY);
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);

		charRead = '\0';
		break;
	}
	case 'z':
	{
		//Definimos un string
//		sprintf(bufferMsg,"Axis Z data (r)\n");
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);
		//Leemos el registro correspondiente a la poscicion en X del acelerometro
		uint8_t AccelZ_low = i2c_readSingleRegister(&handler_I2C_Acelerometro, ACCEL_ZOUT_L);
		uint8_t AccelZ_high = i2c_readSingleRegister(&handler_I2C_Acelerometro, ACCEL_ZOUT_H);
		uint16_t AccelZ = AccelZ_high<<8 | AccelZ_low;
		//Definimos un string
//		sprintf(bufferMsg,"AccelZ = %d \n", (int) AccelZ );
//		//Enviamos por puerto serial dicho string
//		writeMsg(&handler_USART_USB, bufferMsg);

		charRead = '\0';
		break;
	}
	default:
	{
		charRead = '\0';
		break;
	}
	}
}
//--------------------------  --Fin de la definicion de las funciones------------------------------------------

