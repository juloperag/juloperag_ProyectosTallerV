/*
 * InterfazDriver.c
 *
 *  Created on: 7/05/2023
 *      Author: julian
 */

#include <InterfazDriver.h>

uint8_t boolInterface = 1;
uint8_t boolsection = 1;
uint8_t wait_Value = 1;
uint8_t charRead = '\0';
uint8_t section_Interface = 0;
uint8_t digit_Position = 0;
uint8_t containers= 0;
uint8_t unidades = 0;
uint8_t decenas = 0;

void Interface(USART_Handler_t *prthandlerUSART,uint8_t *amount_containers)
{
	while(boolInterface)
	{

		msgInterface(prthandlerUSART);

		while(boolsection)
		{

			if(section_Interface==1)
			{
				while(wait_Value)
				{
					__NOP();
				}
				wait_Value = 1;
				decenas = 0;
				digit_Position = 0;
				obtainNum(charRead);
				containers = obtainAmount(prthandlerUSART);

				if(containers>6)
				{
					containers = 0;
					writeMsg(prthandlerUSART, "Cantidad Excedida \n");
				}
				else
				{
					__NOP();
				}

			}
			else if(section_Interface>1 && section_Interface<8)
			{
				while(wait_Value)
				{
					__NOP();
				}

				wait_Value = 1;
				obtainNum(charRead);
				amount_containers[(section_Interface-2)] = obtainAmount(prthandlerUSART);
			}
			else
			{
				__NOP();
			}

		}
		digit_Position = 0;
		unidades = 0;
		decenas = 0;

	}
}

void msgInterface(USART_Handler_t *prthandlerUSART)
{
	switch(section_Interface)
	{
	case 0:
	{
		writeMsg(prthandlerUSART, "Menu de configuracion \n");

		break;
	}
	case 1:
	{
		writeMsg(prthandlerUSART, "N° de recipientes: \n");
		writeMsg(prthandlerUSART, "0 \n");

		break;
	}
	case 2:
	{
		writeMsg(prthandlerUSART, "N° de elementos R1: \n");
		writeMsg(prthandlerUSART, "00 \n");

		break;
	}
	case 3:
	{
		writeMsg(prthandlerUSART, "N° de elementos R2: \n");
		writeMsg(prthandlerUSART, "00 \n");

		break;
	}
	case 4:
	{
		writeMsg(prthandlerUSART, "N° de elementos R3: \n");
		writeMsg(prthandlerUSART, "00 \n");

		break;
	}
	case 5:
	{
		writeMsg(prthandlerUSART, "N° de elementos R4: \n");
		writeMsg(prthandlerUSART, "00 \n");

		break;
	}
	case 6:
	{
		writeMsg(prthandlerUSART, "N° de elementos R5: \n");
		writeMsg(prthandlerUSART, "00 \n");

		break;
	}
	case 7:
	{
		writeMsg(prthandlerUSART, "N° de elementos R6: \n");
		writeMsg(prthandlerUSART, "00 \n");

		break;
	}
	case 8:
	{
		writeMsg(prthandlerUSART, "Desea Comenzar \n");

		break;
	}
	default:
	{
		break;
	}
	}
}

void obtainNum(uint8_t data)
{
	if(data>'0' && data <'9')
	{
		if(digit_Position==0)
		{
			unidades = data-'0';
		}
		else if(digit_Position==1)
		{
			decenas = data-'0';
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

uint8_t obtainAmount(USART_Handler_t *prthandlerUSART)
{
	uint8_t amount=0;
	char bufferMsg[10] = {0};
	amount = decenas*10+unidades;
	sprintf(bufferMsg,"%u \n",amount);
	writeMsg(prthandlerUSART, bufferMsg);
	return amount;
}
