/*
 * InterfazDriver.c
 *
 *  Created on: 7/05/2023
 *      Author: julian
 */

#include <InterfazDriver.h>

//Definicion de variables
uint8_t boolInterface = 1;       //Variable para el control del ciclo while de la interfaz
uint8_t boolsection = 1;         //Variable para el control del ciclo while de la seccion
uint8_t wait_Value = 1;          //Variable para el control del ciclo while de nuevo caracter leido
uint8_t charRead = '\0';         //Variable que guarda el caracter entregado por el Keypad
uint8_t section_Interface = 0;   //Variable que establece la posicion de la seccion de la interface
uint8_t digit_Position = 0;      //Variable que establece la posicion de los digitos
uint8_t containers= 0;           //Variable que determina la cantidad de recipientes a usar
uint8_t number_containers = 0;   //recipiente para ser configurado
uint8_t unidades = 0;            //unidades de la cantidad final a cargar
uint8_t decenas = 0;             //decenas de la cantidad final a cargar

//funcion que deacuerdo al caracter leido ejecuta una operacion de la interface
void executeChar(USART_Handler_t *prthandlerUSART, char data)
{
	//Guardamos el caracter leido en una variable
	charRead = data;

	//Deacuerdo al caracter enviado se ejecutada una accion determinada
	switch(charRead)
	{
	case 'E':
	{
		//Verificamos que no estamos en la seccion 1 y no se a superado el numero de secciones existentes
		if(section_Interface!=1 && section_Interface<3)
		{
			section_Interface++;  //Sumamos a la variable de la seccion
			wait_Value = 0; 	  //Establecemos un valor falso para la variable de dicho ciclo while
			boolsection = 0;
		}
		//verificamos se la seccion es 1
		else if(section_Interface==1)
		{
			//En caso de superar el limite de recipientes se envia una avertencia
			if(containers>6)
			{
				writeMsg(prthandlerUSART, "Recipientes Excedidos \n");  //Enviamos mensaje
			}
			//En caso de no definir un numero de recipiente se envia una avertencia
			else if(containers==0)
			{
				writeMsg(prthandlerUSART, "No se a seleccionado recipientes \n");  //Enviamos mensaje
			}
			else
			{
				section_Interface++;      //Sumamos a la variable de la seccion
				wait_Value = 0;           //Establecemos un valor falso para la variable de dicho ciclo while
				boolsection = 0;
			}

		}
		else
		{
			wait_Value = 0;              //Establecemos un valor falso para la variable de dicho ciclo while
			boolsection = 0;
			boolInterface = 0;
		}

		break;
	}
	case 'N':
	{
		//verificamos si la seccion es superior a 0
		if(section_Interface>0)
		{
			section_Interface--;   //Restamos a la variable de la seccion
			wait_Value = 0;        //Establecemos un valor falso para la variable de dicho ciclo while
			boolsection = 0;
		}
		else
		{
			__NOP();
		}
		break;
	}
	case 'W':
	{
		//Verificamos que nos encontramos en la seccion 2
		if(section_Interface==2)
		{
			//Verificamos si el numero de recipiente es menor a la cantidad de recipientes
			if(number_containers<(containers-1))
			{
				number_containers++;       //Sumanos a la variable del numero de recipiente
				wait_Value = 0;			  //Establecemos un valor falso para la variable de dicho ciclo while
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
	case 'S':
	{
		//Verificamos que nos encontramos en la seccion 2
		if(section_Interface==2)
		{
			//Verificamos si el numero de recipiente es mayor a 0
			if(number_containers>0)
			{
				number_containers--;      //Restamos a la variable del numero de recipiente
				wait_Value = 0;           //Establecemos un valor falso para la variable de dicho ciclo while
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
	case 'A':
	{
		//Verificamos que nos encontramos en la seccion 2
		if(section_Interface==2)
		{
			digit_Position = 1;  //cambiamos el valor de la posicion del digito
		}
		else
		{
			__NOP();
		}
	}
	case 'D':
	{
		//Verificamos que nos encontramos en la seccion 2
		if(section_Interface==2)
		{
			digit_Position = 0;   //cambiamos el valor de la posicion del digito
		}
		else
		{
			__NOP();
		}
	}
	case '0'||'1'||'2'||'3'||'4'||'5'||'6'||'7'||'8'||'9':
	{
		wait_Value = 0; 	  //Establecemos un valor falso para la variable de dicho ciclo while
	}

	default:
	{
		break;
	}
	}
}

//Funcion que ejecuta la interfaz de usuario
void InterfaceStart(USART_Handler_t *prthandlerUSART,uint8_t *amount_containers)
{
	char bufferMsg[10] = {0};  //Arreglo que guarda el mensaje a enviar por USART

	//Ciclo while que se ejecuta durante la configuracion de las cantidades de los recipientes
	while(boolInterface)
	{

		msgInterface(prthandlerUSART); //se envia la seccion establecida

		//Ciclo while que se ejecuta durante una seccion de la interface
		while(boolsection)
		{
			//---------------------------seccion 1----------------------------
			if(section_Interface==1)
			{
				//Ciclo while que espera un nuevo caracter leido
				while(wait_Value)
				{
					__NOP();
				}
				wait_Value = 1;  //Reiniciamos

				//verificamos que el Caracter es un numero
				if(charRead>='0' && charRead<='9')
				{
					//Reiniciamos variables
					decenas = 0;
					digit_Position = 0;
					//Convertimos y guardamos el caracter leido como un numero entero
					obtainNum(charRead);
					containers = obtainAmount();
					//Enviamos mensaje
					sprintf(bufferMsg,"%u \n",containers);
					writeMsg(prthandlerUSART, bufferMsg);
				}
				else
				{
					__NOP();
				}

				//verificamos que containers no supere el valor desponible de recipientes
				if(containers>6)
				{
					containers = 0;
					writeMsg(prthandlerUSART, "Recipientes Excedidos \n");  //Enviamos mensaje
				}

				else
				{
					__NOP();
				}

			}
			//---------------------------seccion 2----------------------------
			else if(section_Interface==2)
			{
				while(wait_Value)
				{
					__NOP();
				}
				wait_Value = 1;

				if(charRead=='W' || charRead=='S')
				{
					//Reiniciamos variables
					digit_Position = 0;
					//Obtenemos las decenas y unidades de la cantidad asignada para dicho recipiente
					decenas = amount_containers[number_containers]/10;
					unidades = amount_containers[number_containers] - decenas*10;
					//Enviamos mensajes
					msgContainers(prthandlerUSART);
					sprintf(bufferMsg,"%u \n", amount_containers[number_containers]);
					writeMsg(prthandlerUSART, bufferMsg);
				}
				else if(charRead>='0' && charRead<='9')
				{
					//Convertimos y guardamos el caracter leido como un numero entero
					obtainNum(charRead);
					amount_containers[number_containers] = obtainAmount();
					//Enviamos mensaje
					sprintf(bufferMsg,"%u \n",amount_containers[number_containers]);
					writeMsg(prthandlerUSART, bufferMsg);
				}
				else
				{
					__NOP();
				}
			}
			//---------------------------Demas secciones----------------------------
			else
			{
				__NOP();
			}

		}
		//reinicio de variables
		number_containers = 0;
		boolsection = 1;
		digit_Position = 0;
		unidades = 0;
		decenas = 0;

	}
}

//funcion que entrega el mensaje que se debe enviar por cada section
void msgInterface(USART_Handler_t *prthandlerUSART)
{
	switch(section_Interface)
	{
	case 0:
	{
		writeMsg(prthandlerUSART, "Menu de configuracion \n"); //Mensaje a enviar en la seccion 0

		break;
	}
	case 1:
	{
		writeMsg(prthandlerUSART, "N° de recipientes: \n");   //Mensaje a enviar en la seccion 1
		writeMsg(prthandlerUSART, "0 \n");

		break;
	}
	case 2:
	{
		writeMsg(prthandlerUSART, "N° de elementos por recipiente: \n");   //Mensaje a enviar en la seccion 2
		writeMsg(prthandlerUSART, "Recipiente N°1: \n");
		writeMsg(prthandlerUSART, "00 \n");

		break;
	}
	case 3:
	{
		writeMsg(prthandlerUSART, "Desea Comenzar \n");       //Mensaje a enviar en la seccion 3

		break;
	}
	default:
	{
		break;
	}
	}
}


//funcion que entrega el mensaje que se debe enviar cada vez que se seleccione un recipiente
void msgContainers(USART_Handler_t *prthandlerUSART)
{
	switch(number_containers)
	{
	case 0:
	{
		writeMsg(prthandlerUSART, "Recipiente N°1: \n");  //Mensaje a enviar cuando se seleccione el recipiente 1

		break;
	}
	case 1:
	{
		writeMsg(prthandlerUSART, "Recipiente N°2: \n");  //Mensaje a enviar cuando se seleccione el recipiente 2
		break;
	}
	case 2:
	{
		writeMsg(prthandlerUSART, "Recipiente N°3: \n");  //Mensaje a enviar cuando se seleccione el recipiente 3

		break;
	}
	case 3:
	{
		writeMsg(prthandlerUSART, "Recipiente N°4: \n");  //Mensaje a enviar cuando se seleccione el recipiente 4

		break;
	}
	case 4:
	{
		writeMsg(prthandlerUSART, "Recipiente N°5: \n");  //Mensaje a enviar cuando se seleccione el recipiente 5

		break;
	}
	case 5:
	{
		writeMsg(prthandlerUSART, "Recipiente N°6: \n");  //Mensaje a enviar cuando se seleccione el recipiente 6

		break;
	}
	default:
	{
		break;
	}
	}
}

//funcion para obtener el numero a partir del caracter leido
void obtainNum(uint8_t data)
{
	//La posicion en el digito son las unidades
	if(digit_Position==0)
	{
		unidades = data-'0';
	}
	//La posicion en el digito son los decenas
	else if(digit_Position==1)
	{
		decenas = data-'0';
	}
	else
	{
		__NOP();
	}
}

//funcion para obtener la cantidad o digito deaduerdo a las decenas y unidades
uint8_t obtainAmount(void)
{
	uint8_t amount=0;
	amount = decenas*10+unidades;
	return amount;
}

