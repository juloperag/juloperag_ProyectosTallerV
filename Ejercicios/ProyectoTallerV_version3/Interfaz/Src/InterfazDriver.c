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

uint8_t auxNum = 0;              //Variable que guarda el numero seleccionado en la pantalla
uint8_t unidades = 0;            //unidades de la cantidad final a cargar
uint8_t decenas = 0;             //decenas de la cantidad final a cargar

uint8_t executeOSC = 0;           //Bandera que indica la ejecucion de la operacion mecanica de la seleccion de recipientes

//funcion que deacuerdo al caracter leido ejecuta una operacion de la interface
void executeChar(I2C_Handler_t *prthandlerI2C, char data)
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
			wait_Value = 0;            //Establecemos un valor falso para la variable de dicho ciclo while
			break;
		}
		case 2:
		{
			stage=3;             //Cambiamos la etapa
			wait_Value = 0; 	  //Establecemos un valor falso para la variable de dicho ciclo while
			boolstage = 0;
			//Desactivamos el modo parpadeante del cursor
			lcd_i2c_cursor_blinky_Enable(prthandlerI2C);

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
			//Posicionamos el cursor en la posicion deacuerdo a la variable digit_Position
			lcd_i2c_gotoxy(prthandlerI2C, 1, (13-digit_Position));
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
			//Posicionamos el cursor en la posicion deacuerdo a la variable digit_Position
			lcd_i2c_gotoxy(prthandlerI2C, 1, (13-digit_Position));
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
		if(stage==4 || stage==5)
		{
			stopOperation=1;   //cambiamos el valor a 1
		}
		else
		{
			__NOP();
		}

		break;
	}
	case 'G':
	{
		if(stage==4 || stage==5)
		{
			stopOperation=0;   //cambiamos el valor a cero
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

void InterfaceStart(I2C_Handler_t *prthandlerI2C)
{
	msgInterface(prthandlerI2C); //se envia la seccion establecida
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
			//Impiamos fila inferior
			lcd_i2c_gotoxy(prthandlerI2C, 1, 0);
			lcd_i2c_putc(prthandlerI2C, "                ");
			//Imprimimos mensaje
			switch (auxstage) {
			case 1:
			{
				lcd_i2c_gotoxy(prthandlerI2C, 1, 1);
				lcd_i2c_putc(prthandlerI2C, "Et: Separacion");
				break;
			}
			case 5:
			{
				lcd_i2c_gotoxy(prthandlerI2C, 1, 0);
				lcd_i2c_putc(prthandlerI2C, "Et: Seleccion");
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
			__NOP();
		}
	}

}

//Funcion que ejecuta la interfaz inicial de usuario
void InterfaceConfigContainer(I2C_Handler_t *prthandlerI2C,uint8_t *items_Containers)
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

		msgInterface(prthandlerI2C); //se envia la seccion establecida

		//Mostramos la cantidad actual deacuerdo a la etapa en que se esta actualmente
		if(stage==1)
		{
			//Enviamos mensaje
			sprintf(bufferMsg,"%u",amount_Containers);
			lcd_i2c_gotoxy(prthandlerI2C, 1, 8);
			lcd_i2c_putc(prthandlerI2C, bufferMsg);
		}
		else if(stage==2)
		{
			//Mostramos por pantalla la cantidad definidad
			msgNumContainer(prthandlerI2C,items_Containers[selected_Containers]);
			//Posicionamos el cursor en la posicion deacuerdo a la variable digit_Position
			lcd_i2c_gotoxy(prthandlerI2C, 1, (13-digit_Position));
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
					sprintf(bufferMsg,"%u",amount_Containers);
					lcd_i2c_gotoxy(prthandlerI2C, 1, 8);
					lcd_i2c_putc(prthandlerI2C, bufferMsg);
				}
				else
				{
					__NOP();
				}

				//Verificamos que el valor asignado sea el correcto, de lo contrario se envia un mensaje de avertencia
				if(amount_Containers>6 || amount_Containers==0)
				{
					//En caso de superar el limite de recipientes se envia una avertencia
					if(amount_Containers>6)
					{
						//limpiamos la pantalla
						lcd_i2c_clear(prthandlerI2C);
						//Envio de mensaje
						lcd_i2c_gotoxy(prthandlerI2C, 0, 1);
						lcd_i2c_putc(prthandlerI2C, "Recipientes");
						lcd_i2c_gotoxy(prthandlerI2C, 1, 2);
						lcd_i2c_putc(prthandlerI2C, "Excedidos");
					}
					//En caso de no definir un numero de recipiente se envia una avertencia
					else
					{
						//limpiamos la pantalla
						lcd_i2c_clear(prthandlerI2C);
						//Envio de mensaje
						lcd_i2c_gotoxy(prthandlerI2C, 0, 4);
						lcd_i2c_putc(prthandlerI2C, "Cantidad");
						lcd_i2c_gotoxy(prthandlerI2C, 1, 2);
						lcd_i2c_putc(prthandlerI2C, "no definida");
					}
					//Pausa
					delay_ms(1000);
					//se establece de nuevo el mensaje de la seccion
					msgInterface(prthandlerI2C);
					sprintf(bufferMsg,"%u",amount_Containers);
					lcd_i2c_gotoxy(prthandlerI2C, 1, 8);
					lcd_i2c_putc(prthandlerI2C, bufferMsg);
				}
				//Si la cantidad de recipientes cumple y ademas la tecla precionada fue la 'E' entonces se cambia el estado
				else if(charRead=='E')
				{
					stage=2;                   //Cambiamos la etapa
					boolstage = 0;
					//Activamos el modo parpadeante del cursor
					lcd_i2c_cursor_blinky_Enable(prthandlerI2C);
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
					//Mostramos por pantalla la cantidad definidad
					msgNumContainer(prthandlerI2C, items_Containers[selected_Containers]);
					//Posicionamos el cursor en la posicion deacuerdo a la variable digit_Position
					lcd_i2c_gotoxy(prthandlerI2C, 1, (13-digit_Position));
				}
				//verificamos que el Caracter es un numero
				else if(charRead>='0' && charRead<='9')
				{
					//Convertimos y guardamos el caracter leido como un numero entero
					obtainNum(charRead);
					items_Containers[selected_Containers] = obtainAmount();
					//Enviamos mensajes
					sprintf(bufferMsg,"%u", charRead);
					lcd_i2c_putc(prthandlerI2C, bufferMsg);
					//Posicionamos el cursor en la posicion deacuerdo a la variable digit_Position
					lcd_i2c_gotoxy(prthandlerI2C, 1, (13-digit_Position));
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
void InterfaceOpeCounting(I2C_Handler_t *prthandlerI2C, uint8_t caseOper, uint8_t disco, uint8_t parameter)
{

	switch(caseOper)
	{
	case 1:
	{
		//Cargamos la posicion de los recipiente al selected_Containers
		defineSelectedContainers(parameter);
		//mostramos el recipiente actual
		msgContainers(prthandlerI2C);

		break;
	}
	case 2:
	{
		msgNumContainer(prthandlerI2C, parameter);
		break;
	}
	case 3:
	{
		executeStopOperaction(prthandlerI2C, disco ,parameter);
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
	case 5:
	{
		//Limpiamos la pantalla
		lcd_i2c_clear(prthandlerI2C);
		//Enviamos mensajes
		lcd_i2c_gotoxy(prthandlerI2C, 0, 0);
		lcd_i2c_putc(prthandlerI2C, "DI.Moviendose...");
		break;
	}
	default:
	{
		break;
	}
	}
}

//Funcion que ejecuta la interfaz final de usuario
uint8_t InterfaceOpeSelectedCon(I2C_Handler_t *prthandlerI2C,  uint8_t caseOper, uint8_t parameter)
{
	switch (caseOper) {
	case 1:
	{
		//Enviamos un mensaje que indica el inicio de la seleccion y indica en que recipiente se seleccion
		msgInterface(prthandlerI2C);
		//mostramos la cantidad de elementos almacenados en el recipiente
		msgNumContainer(prthandlerI2C, parameter);
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
			//Limpiamos la pantalla
			lcd_i2c_clear(prthandlerI2C);
			//Enviamos mensajes
			lcd_i2c_gotoxy(prthandlerI2C, 0, 0);
			lcd_i2c_putc(prthandlerI2C, "DI.Moviendose...");
		}

		else
		{
			__NOP();
		}
	break;
	}
	case 3:
	{
		executeStopOperaction(prthandlerI2C, 0, 0);
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
void msgInterface(I2C_Handler_t *prthandlerI2C)
{
	//Limpiamos la pantalla
	lcd_i2c_clear(prthandlerI2C);
	//Definimos la etapa
	switch(stage)
	{
	case 0:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 0, 6);
		lcd_i2c_putc(prthandlerI2C, "Menu");
		break;
	}
	case 1:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 0, 2);
		lcd_i2c_putc(prthandlerI2C, "N°Recipientes:");
		break;
	}
	case 2:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 0, 2);
		lcd_i2c_putc(prthandlerI2C, "N°Elementos:");
		//mensaje recipiente seleccionado
		msgContainers(prthandlerI2C);
		break;
	}
	case 3:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 0, 5);
		lcd_i2c_putc(prthandlerI2C, "¿Desea");
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 1, 3);
		lcd_i2c_putc(prthandlerI2C, "Comenzar?");
		break;
	}
	case 4:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 0, 3);
		lcd_i2c_putc(prthandlerI2C, "Separacion");
		//mensaje recipiente seleccionado
		msgContainers(prthandlerI2C);
		break;
	}
	case 5:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 0, 5);
		lcd_i2c_putc(prthandlerI2C, "Seleccion");
		//mensaje recipiente seleccionado
		msgContainers(prthandlerI2C);
		break;
	}
	default:
	{
		break;
	}
	}
}


//funcion que entrega el mensaje que se debe enviar cada vez que se seleccione un recipiente
void msgContainers(I2C_Handler_t *prthandlerI2C)
{
	//Impiamos fila inferior
	lcd_i2c_gotoxy(prthandlerI2C, 1, 0);
	lcd_i2c_putc(prthandlerI2C, "                ");

	switch(selected_Containers)
	{
	case 0:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 1, 3);
		lcd_i2c_putc(prthandlerI2C, "Rec N°1:");
		break;
	}
	case 1:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 1, 3);
		lcd_i2c_putc(prthandlerI2C, "Rec N°2:");
		break;
	}
	case 2:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 1, 3);
		lcd_i2c_putc(prthandlerI2C, "Rec N°3:");
		break;
	}
	case 3:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 1, 3);
		lcd_i2c_putc(prthandlerI2C, "Rec N°4:");
		break;
	}
	case 4:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 1, 3);
		lcd_i2c_putc(prthandlerI2C, "Rec N°5:");
		break;
	}
	case 5:
	{
		//Enviamos mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 1, 3);
		lcd_i2c_putc(prthandlerI2C, "Rec N°6:");
		break;
	}
	default:
	{
		break;
	}
	}
}

//Funcion empleada para mostrar por pantalla LCD los elementos de los recipientes
void msgNumContainer(I2C_Handler_t *prthandlerI2C, uint8_t num)
{
	char bufferMsg[20] = {0};  //Definimos un string
	//Obtenemos las decenas y unidades de la variabe
	uint8_t decnum = num/10;
	uint8_t uninum = num - decnum*10;
	//Enviamos mensajes
	sprintf(bufferMsg,"%u", decnum);
	lcd_i2c_gotoxy(prthandlerI2C, 1, 12);
	lcd_i2c_putc(prthandlerI2C, bufferMsg);
	sprintf(bufferMsg,"%u", uninum);
	lcd_i2c_gotoxy(prthandlerI2C, 1, 13);
	lcd_i2c_putc(prthandlerI2C, bufferMsg);
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

//Funcion que se ejecuta cuando se detiene la operacion
void executeStopOperaction(I2C_Handler_t *prthandlerI2C, uint8_t disco, uint8_t data)
{
	//Variable de control
	uint8_t status = 1;
	while(stopOperation)
	{
		if(status==1)
		{
			//limpiamos la pantalla
			lcd_i2c_clear(prthandlerI2C);
			//Envio de mensaje
			lcd_i2c_gotoxy(prthandlerI2C, 0, 4);
			lcd_i2c_putc(prthandlerI2C, "Proceso");
			lcd_i2c_gotoxy(prthandlerI2C, 1, 2);
			lcd_i2c_putc(prthandlerI2C, "Interrumpido");
			//reiniciamos variable
			status=0;
		}
		else
		{
			__NOP();
		}
	}
	if(status==0)
	{
		//Envio de mensaje
		lcd_i2c_gotoxy(prthandlerI2C, 0, 4);
		lcd_i2c_putc(prthandlerI2C, "Proceso");
		lcd_i2c_gotoxy(prthandlerI2C, 1, 3);
		lcd_i2c_putc(prthandlerI2C, "Reiniciado");
		//Generamos una pausa
		delay_ms(1000);
		if(disco==1)
		{
			//Enviamos un mensaje que indica la seleccion
			msgInterface(prthandlerI2C);
			//Mostramos los elmentos de los recipientes
			msgNumContainer(prthandlerI2C, data);
		}
		else
		{
			//limpiamos la pantalla
			lcd_i2c_clear(prthandlerI2C);
			//Enviamos mensaje
			lcd_i2c_gotoxy(prthandlerI2C, 0, 0);
			lcd_i2c_putc(prthandlerI2C, "DI.Moviendose...");
		}
	}
	else
	{
		__NOP();
	}
}



