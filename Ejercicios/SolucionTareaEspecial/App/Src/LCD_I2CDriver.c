/*
 * LCD_I2CDriver.c
 *
 *  Created on: 27/05/2023
 *      Author: julian
 */

#include <LCD_I2CDriver.h>

// Direcciones de memoria de una pantalla LCD de 16 x 4
const int lcdAddresses[4][16] = {
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
    {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F},
    {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F},
    {0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F}
};

//Funcion para inicializar la lcd por medio de comunicacion I2C
void lcd_i2c_init(I2C_Handler_t *ptrHandlerLCDI2C)
{
	delay_ms(100);
	//Establecer Valores iniciales
	i2c_WriteSingle(ptrHandlerLCDI2C, (LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));

	//-----------------------------------1) Modo de 4 bit---------------------------------------------------
	//Envio de comando 3 veces
	i2c_WriteSingle(ptrHandlerLCDI2C, (ENTER_MODE_4_BIT | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, (ENTER_MODE_4_BIT | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(5);
	i2c_WriteSingle(ptrHandlerLCDI2C, (ENTER_MODE_4_BIT | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, (ENTER_MODE_4_BIT | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(1);
	i2c_WriteSingle(ptrHandlerLCDI2C, (ENTER_MODE_4_BIT | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, (ENTER_MODE_4_BIT | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(1);
	//Configuramos la LCD en el modo de 4 bit
	i2c_WriteSingle(ptrHandlerLCDI2C, (RF_MODE_4_BIT | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, (RF_MODE_4_BIT | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);

	//---------------------------2) Lineas logicas y tamaño de caracteres----------------------------------
	i2c_WriteSingle(ptrHandlerLCDI2C, ((RF_LINE_CHARACTER & BITS_H) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, ((RF_LINE_CHARACTER & BITS_H) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);
	i2c_WriteSingle(ptrHandlerLCDI2C, ((RF_LINE_CHARACTER & BITS_L) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, ((RF_LINE_CHARACTER & BITS_L) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);

	//---------------------------3) Definimos el desplazamiento del Cursor---------------------------------
	i2c_WriteSingle(ptrHandlerLCDI2C, ((EM_CURSOR_RIGHT & BITS_H) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, ((EM_CURSOR_RIGHT & BITS_H) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);
	i2c_WriteSingle(ptrHandlerLCDI2C, ((EM_CURSOR_RIGHT & BITS_L) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, ((EM_CURSOR_RIGHT & BITS_L) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);

	//---------------------------4) Encender LCD----------------------------------
	i2c_WriteSingle(ptrHandlerLCDI2C, ((TURN_ON_DISPLAY & BITS_H) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, ((TURN_ON_DISPLAY & BITS_H) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);
	i2c_WriteSingle(ptrHandlerLCDI2C, ((TURN_ON_DISPLAY & BITS_L) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, ((TURN_ON_DISPLAY & BITS_L) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);

	//---------------------------5) Limpiamos pantalla----------------------------------
	lcd_i2c_clear(ptrHandlerLCDI2C);
}

//Funcion para limpiar la pantalla
void lcd_i2c_clear(I2C_Handler_t *ptrHandlerLCDI2C)
{
	//Establecer Valores iniciales
	i2c_WriteSingle(ptrHandlerLCDI2C, (LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));
	//Envio de comandos para limpiar la LCD
	i2c_WriteSingle(ptrHandlerLCDI2C, ((CLEAR_DISPLAY & BITS_H) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, ((CLEAR_DISPLAY & BITS_H) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);
	i2c_WriteSingle(ptrHandlerLCDI2C, ((CLEAR_DISPLAY & BITS_L) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, ((CLEAR_DISPLAY & BITS_L) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);
}

//funcion para posicionar el cursor en una de las posiciones del display
void lcd_i2c_gotoxy(I2C_Handler_t *ptrHandlerLCDI2C, uint8_t row, uint8_t column)
{
	//Establecer Valores iniciales
	i2c_WriteSingle(ptrHandlerLCDI2C, (LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));
	//Envio de comandos para limpiar la LCD
	i2c_WriteSingle(ptrHandlerLCDI2C, (ADD | (lcdAddresses[row][column] & BITS_H) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, (ADD | (lcdAddresses[row][column] & BITS_H) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);
	i2c_WriteSingle(ptrHandlerLCDI2C, (ADD | (lcdAddresses[row][column] & BITS_L) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_ID));   //Enviamos comando y activamos EN
	i2c_WriteSingle(ptrHandlerLCDI2C, (ADD | (lcdAddresses[row][column] & BITS_L) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_ID));   //Desactivamos EN
	delay_ms(3);
}

//Funcion para escribir un string en la pantalla
void lcd_i2c_putc(I2C_Handler_t *ptrHandlerLCDI2C, char *MsgtoSend)
{
	uint8_t i = 0;      //Variable para recorrer las posciones del string
	//Establecer Valores iniciales
	i2c_WriteSingle(ptrHandlerLCDI2C, (LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_DR));
	//Recorremos el string hasta aparecer el elemento nulo y enviamos dicho caracter por I2C
	while(MsgtoSend[i] != '\0')
	{
		i2c_WriteSingle(ptrHandlerLCDI2C, ((MsgtoSend[i] & BITS_H) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_DR));   //Enviamos comando y activamos EN
		i2c_WriteSingle(ptrHandlerLCDI2C, ((MsgtoSend[i] & BITS_H) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_DR));   //Desactivamos EN
		delay_ms(3);
		i2c_WriteSingle(ptrHandlerLCDI2C, ((MsgtoSend[i] & BITS_L) | LED_ENABLE | EN_ENABLE | RW_SELECT_WRITE | RS_SELECT_DR));   //Enviamos comando y activamos EN
		i2c_WriteSingle(ptrHandlerLCDI2C, ((MsgtoSend[i] & BITS_L) | LED_ENABLE | EN_DISABLED | RW_SELECT_WRITE | RS_SELECT_DR));   //Desactivamos EN
		delay_ms(3);
		i++;           //incrementamos la posicion
	}
}

