/*
 * PwmDriver.h
 *
 *  Created on: 3/05/2023
 *      Author: julian
 */

#ifndef PWMDRIVER_H_
#define PWMDRIVER_H_

#include <stm32f4xx.h>

//Definicion de macros

#define PWM_CHANNEL_1   0
#define PWM_CHANNEL_2   1
#define PWM_CHANNEL_3   2
#define PWM_CHANNEL_4   3

#define PWM_DUTTY_0_PERCENT      0
#define PWM_DUTTY_100_PERCENT    100

//Estructura que contiene la configuracion minima para el manejo del PWM
typedef struct
{
	uint8_t     channel;        //Canal PWM relacionado con el TIMER
	uint16_t	periodcnt;		//A que velocidad se incrementa el TIMER,o valor en us del periodo del Clock Counter (min: 1, max:4096)
	uint16_t	periodo;        //Numero de veces que el TIMER se incrementa,o valor en ms o uS(Depende) del periodo del timer o valor del autoreloar
	uint16_t	duttyCicle;		//Valor en porcentaje (%) del tiempo que la señal esta en alto
}PWM_Config_t;

//Definicion del Handler del PWM
typedef struct
{
	PWM_Config_t 	config;   //Configuracion inicial del PWM
	TIM_TypeDef	  *ptrTIMx;   // TIMER al que esta asociado el PWM
}PWM_Handler_t;

//Definicion de las cabeceras de las funciones para el uso del PWM
void pwm_Config(PWM_Handler_t *prtPwmHandler);
//Frecuencia
void setFrequency(PWM_Handler_t *prtPwmHandler);
void updateFrequency(PWM_Handler_t *prtPwmHandler, uint16_t newPer);
//Dutty
void setDuttyCycle(PWM_Handler_t *prtPwmHandler);
void updateDuttyCycle(PWM_Handler_t *prtPwmHandler, uint16_t newDutty);
//Enable
void enableOutput(PWM_Handler_t *prtPwmHandler);
//Empezar PWM
void startPwmSignal(PWM_Handler_t *prtPwmHandler);
//Parar PWM
void stopPwmSignal(PWM_Handler_t *prtPwmHandler);

#endif /* PWMDRIVER_H_ */
