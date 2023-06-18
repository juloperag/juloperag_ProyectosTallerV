/*
 * InterfazDriver.c
 *
 *  Created on: 15/06/2023
 *      Author: julian
 */

#include <InterfazDriver.h>
#include <SysTickDriver.h>

//Definicion de variables
uint8_t charRead = '\0';         //Variable que guarda el caracter entregado por el Keypad

uint8_t auxstage = 1;            //Variable auxilar para establecer la etapa en el menu inicial

uint8_t stopOperation = 0;       //Variable para el control del ciclo while de la parada de la operacion
uint8_t boolInterface = 1;       //Variable para el control del ciclo while de la interfaz
uint8_t boolstage = 1;           //Variable para el control del ciclo while de la seccion

uint8_t wait_Value = 1;          //Variable para el control del ciclo while de nuevo caracter leido

uint8_t stage = 0;   //Variable que establece la posicion de la seccion de la interface
uint8_t digit_Position = 0;      //Variable que establece la posicion de los digitos

uint8_t amount_Containers = 0;           //Variable que determina la cantidad de recipientes a usar
uint8_t selected_Containers = 0;   //recipiente para ser configurado

uint8_t unidades = 0;            //unidades de la cantidad final a cargar
uint8_t decenas = 0;             //decenas de la cantidad final a cargar

uint8_t executeOSC = 0;           //Bandera que indica la ejecucion de la operacion mecanica de la seleccion de recipientes

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
		//Deacuerdo a la etapa se ejecuta una accion
		switch (stage) {
		case 0:
		{
			//cargamos la etapa
			stage = auxstage;
			//Reiniciamos variables
			boolInterface = 0;
			wait_Value = 0;

			break;
		}
		case 1:
		{
			//En caso de superar el limite de recipientes se envia una avertencia
			if(amount_Containers>6)
			{
				writeMsg(prthandlerUSART, "Recipientes Excedidos \n");  //Enviamos mensaje
			}
			//En caso de no definir un numero de recipiente se envia una avertencia
			else if(amount_Containers==0)
			{
				writeMsg(prthandlerUSART, "No se a seleccionado recipientes \n");  //Enviamos mensaje
			}
			else
			{
				stage=2;                   //Cambiamos la etapa
				wait_Value = 0;            //Establecemos un valor falso para la variable de dicho ciclo while
				boolstage = 0;
			}

			break;
		}
		case 2:
		{
			stage=3;             //Cambiamos la etapa
			wait_Value = 0; 	  //Establecemos un valor falso para la variable de dicho ciclo while
			boolstage = 0;

			break;
		}
		case 3:
		{
			stage=4;                     //Cambiamos la etapa
			boolstage = 0;              //Establecemos un valor falso para la variable de dicho ciclo while
			boolInterface = 0;

			break;
		}
		case 4:
		{
			if(executeOSC == 0)
			{
				stage=5;                     //Cambiamos la etapa
			}
			else
			{
				__NOP();
			}
			break;
		}
		case 5:
		{
			if(executeOSC == 0)
			{
				stage=0;                     //Cambiamos la etapa
				wait_Value = 0;              //Establecemos un valor falso para la variable de dicho ciclo while

			}
			break;
		}

		default:
		{
			__NOP();
		break;
		}
		}

		break;
	}
	case 'N':
	{
		//verificamos si la seccion es superior a 0
		if(stage>0 && stage<4)
		{
			stage--;   //Restamos a la variable de la seccion
			wait_Value = 0;        //Establecemos un valor falso para la variable de dicho ciclo while
			boolstage = 0;
		}
		else
		{
			__NOP();
		}
		break;
	}
	case 'W':
	{
		//Verificamos que nos encontramos en la etapa 2 y 5
		if(stage==2 || stage==5)
		{
			//Verificamos si el numero de recipiente es menor a la cantidad de recipientes
			if(selected_Containers<(amount_Containers-1) && executeOSC == 1)
			{
				selected_Containers++;       //Sumanos a la variable selector de recipiente
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
		break;
	}
	case 'S':
	{
		//Verificamos que nos encontramos en la etapa 2 y 5
		if(stage==2 || stage==5)
		{
			//Verificamos si el numero de recipiente es mayor a 0
			if(selected_Containers>0  && executeOSC == 1)
			{
				selected_Containers--;      //Restamos a la variable del numero de recipiente
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
		break;
	}
	case 'A':
	{
		//Verificamos que nos encontramos en la etapa 2 0 0
		if(stage==2)
		{
			digit_Position = 1;  //cambiamos el valor de la posicion del digito
		}
		else if (stage==0)
			{
				amount_Containers = 0; //Definimos la cantidad minima de recipientes
				auxstage = 1;      //Guardamos la etapa seleccinada en una variable auxiliar
				wait_Value = 0;   //Reiniciamos variable
			}
		else
		{
			__NOP();
		}
		break;
	}
	case 'D':
	{
		//Verificamos que nos encontramos en la etapa 2 o 0
		if(stage==2)
		{
			digit_Position = 0;   //cambiamos el valor de la posicion del digito
		}
		else if (stage==0)
		{
			amount_Containers = 6; //Definimos la cantidad maxima de recipientes
			auxstage = 5;      //Guardamos la etapa seleccinada en una variable auxiliar
			wait_Value = 0;   //Reiniciamos variable
		}
		else
		{
			__NOP();
		}
		break;
	}
	case 'F':
	{
		if(stage==4)
		{
			stopOperation=1;   //cambiamos el valor a 1
			writeMsg(prthandlerUSART, "Proceso Interrumpido \n");  //Enviamos mensaje
		}
		else
		{
			__NOP();
		}

		break;
	}
	case 'G':
	{
		if(stage==4)
		{
			stopOperation=0;   //cambiamos el valor a cero
			writeMsg(prthandlerUSART, "Proceso en Ejecucion \n");  //Enviamos mensaje
		}
		else
		{
			__NOP();
		}

		break;
	}
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
		wait_Value = 0; 	  //Establecemos un valor falso para la variable de dicho ciclo while
		break;
	}
	default:
	{
		break;
	}
	}
}

void InterfaceStart(USART_Handler_t *prthandlerUSART)
{
	char bufferMsg[30] = {0};  //Arreglo que guarda el mensaje a enviar por USART

	msgInterface(prthandlerUSART); //se envia la seccion establecida
	boolInterface = 1; //Reinicio de varibles
	wait_Value = 1;
	stopOperation = 0;

	while(boolInterface)
	{
		//Ciclo while que espera un nuevo caracter leido
		while(wait_Value)
		{
			__NOP();
		}
		wait_Value = 1;  //Reiniciamos

		//verificamos que el Caracter es arriba o abajo
		if (charRead == 'A' && charRead == 'D')
		{
			//Imprimimos mensaje
			sprintf(bufferMsg,"Etapa establecida: %u \n", auxstage);
			writeMsg(prthandlerUSART, bufferMsg);
		}
		else
		{
			__NOP();
		}
	}

}

//Funcion que ejecuta la interfaz inicial de usuario
void InterfaceConfigContainer(USART_Handler_t *prthandlerUSART,uint8_t *items_Containers)
{
	char bufferMsg[30] = {0};  //Arreglo que guarda el mensaje a enviar por USART

	//Restablecemos los valores predeterminados
	boolInterface = 1;
	boolstage = 1;
	wait_Value = 1;
	charRead = '\0';
	stage = 0;
	digit_Position = 0;
	amount_Containers= 0;
	selected_Containers = 0;
	unidades = 0;
	decenas = 0;
	for(uint8_t i=0;i<6;i++)
	{
		items_Containers[i]=0;
	}

	//Ciclo while que se ejecuta durante la configuracion de las cantidades de los recipientes
	while(boolInterface)
	{
		delay_ms(30);		//Breve pausa

		msgInterface(prthandlerUSART); //se envia la seccion establecida

		//Mostramos la cantidad actual deacuerdo a la etapa en que se esta actualmente
		if(stage==1)
		{
			//Enviamos mensaje
			sprintf(bufferMsg,"%u \n",amount_Containers);
			writeMsg(prthandlerUSART, bufferMsg);
		}
		else if(stage==2)
		{
			//Enviamos mensaje
			sprintf(bufferMsg,"%u \n", items_Containers[selected_Containers]);
			writeMsg(prthandlerUSART, bufferMsg);
		}
		else
		{
			__NOP();
		}


		//Ciclo while que se ejecuta durante una seccion de la interface
		while(boolstage)
		{

			//---------------------------seccion 1----------------------------
			if(stage==1)
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
					amount_Containers = obtainAmount();
					//Enviamos mensaje
					sprintf(bufferMsg,"%u \n",amount_Containers);
					writeMsg(prthandlerUSART, bufferMsg);
				}
				else
				{
					__NOP();
				}

				//verificamos que amount_Containers no supere el valor desponible de recipientes
				if(amount_Containers>6)
				{
					amount_Containers = 0;
					writeMsg(prthandlerUSART, "Recipientes Excedidos \n");  //Enviamos mensaje
				}

				else
				{
					__NOP();
				}

			}
			//---------------------------seccion 2----------------------------
			else if(stage==2)
			{
				//Ciclo while que espera un nuevo caracter leido
				while(wait_Value)
				{
					__NOP();
				}
				wait_Value = 1;  //Reiniciamos

				//verificamos que el Caracter es arriba o abajo
				if(charRead=='W' || charRead=='S')
				{
					//Reiniciamos variables
					digit_Position = 0;
					//Obtenemos las decenas y unidades de la cantidad asignada para dicho recipiente
					decenas = items_Containers[selected_Containers]/10;
					unidades = items_Containers[selected_Containers] - decenas*10;
					//Enviamos mensajes
					msgContainers(prthandlerUSART);
					sprintf(bufferMsg,"%u \n", items_Containers[selected_Containers]);
					writeMsg(prthandlerUSART, bufferMsg);
				}
				//verificamos que el Caracter es un numero
				else if(charRead>='0' && charRead<='9')
				{
					//Convertimos y guardamos el caracter leido como un numero entero
					obtainNum(charRead);
					items_Containers[selected_Containers] = obtainAmount();
					//Enviamos mensaje
					sprintf(bufferMsg,"%u \n",items_Containers[selected_Containers]);
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
		selected_Containers = 0;
		boolstage = 1;
		digit_Position = 0;
		unidades = 0;
		decenas = 0;

	}
}

//Funcion que ejecuta interfaz de operacion
void InterfaceOpeCounting(USART_Handler_t *prthandlerUSART, uint8_t caseOper, uint8_t parameter)
{
	char bufferMsg[10] = {0};  //Arreglo que guarda el mensaje a enviar por USART

	switch(caseOper)
	{
	case 1:
	{
		//Cargamos la posicion de los recipiente al selected_Containers
		defineSelectedContainers(parameter);
		//mostramos el recipiente actual
		msgContainers(prthandlerUSART);

		break;
	}
	case 2:
	{
		//Mostramos la cantidad faltante de elementos para el especifico recipiente
		sprintf(bufferMsg,"%u \n", parameter);
		writeMsg(prthandlerUSART, bufferMsg);

		break;
	}
	case 3:
	{
		while(stopOperation)
		{
			__NOP();
		}
	}
	case 4:
	{
		if(parameter==0)
		{
			executeOSC=0;
		}
		else
		{
			executeOSC=1;
		}
	}
	default:
	{
		break;
	}
	}
}

//Funcion que ejecuta la interfaz final de usuario
uint8_t InterfaceOpeSelectedCon(USART_Handler_t *prthandlerUSART,  uint8_t caseOper)
{
	switch (caseOper) {
	case 1:
	{
		//mostramos el recipiente actual
		msgContainers(prthandlerUSART);
	break;
	}
	case 2:
	{
		//Bajamos la bandera
		executeOSC = 0;
		//Ciclo while que espera unos determinados caracteres
		while(wait_Value)
		{
			__NOP();
		}
		//Reiniciamos la variable
		wait_Value = 1;
		//verificamos que el caracter es arriba o abajo
		if(charRead=='W' || charRead=='S')
		{
			//Activamos la bandera
			executeOSC = 1;
			//Enviamos mensajes
			msgContainers(prthandlerUSART);
		}

		else
		{
			__NOP();
		}
	break;
	}
	case 3:
	{
		while(stopOperation)
		{
			__NOP();
		}
	}
	default:
	{
		__NOP();
		break;
	}
	}

	return selected_Containers;
}

//funcion que entrega el mensaje que se debe enviar por cada section
void msgInterface(USART_Handler_t *prthandlerUSART)
{
	switch(stage)
	{
	case 0:
	{
		writeMsg(prthandlerUSART, "Menu de configuracion \n"); //Mensaje a enviar en la etapa 0

		break;
	}
	case 1:
	{
		writeMsg(prthandlerUSART, "N° de recipientes: \n");   //Mensaje a enviar en la etapa1

		break;
	}
	case 2:
	{
		writeMsg(prthandlerUSART, "N° de elementos por recipiente: \n");   //Mensaje a enviar en la etapa 2
		writeMsg(prthandlerUSART, "Recipiente N°1: \n");

		break;
	}
	case 3:
	{
		writeMsg(prthandlerUSART, "¿Desea Comenzar? \n");       //Mensaje a enviar en la etapa 3

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
	switch(selected_Containers)
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

//funcion que retorna el numero de recipientes asignados
uint8_t amountContainers(void)
{
	return amount_Containers;
}

//funcion psts retorna la etapa actual
uint8_t obtainStage(void)
{
	return stage;
}

//funcion que define el recipientes a seleccionar
void defineSelectedContainers(uint8_t value)
{
	selected_Containers = value;
}


