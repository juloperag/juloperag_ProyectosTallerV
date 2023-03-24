/*
 * BasicTimer.c
 *
 *  Created on: 18/03/2023
 *      Author: julian
 */

#include <BasicTimer.h>

//Variable que guarda la referencia de periferico a utilizar
TIM_TypeDef *ptrTimerUsed;

//Funcion para cargar la configuracion del Timer
void BasicTimer_Config(BasicTimer_Handler_t *ptrBTimerHandler)
{
	//Guardamos una referencia al periferico que estamos utilizando

	ptrTimerUsed = ptrBTimerHandler->ptrTIMx;

	//----------------------------0)Desactivamos las interrupcciones globales-----------------------------
	__disable_irq();
	//---------------------------------1) Activamos el periferico------------------------------------------
	//Registro: APB1ENR

	//Verificamos para TIM2
	if(ptrBTimerHandler->ptrTIMx==TIM2)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	//Verificamos para TIM3
	else if(ptrBTimerHandler->ptrTIMx==TIM3)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	//Verificamos para TIM4
	else if(ptrBTimerHandler->ptrTIMx==TIM4)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	//Verificamos para TIM5
	else if(ptrBTimerHandler->ptrTIMx==TIM5)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	//------------------------------2) Configurando el pre-escaler-----------------------------------------
	//Registro:TIMx_PSC		//Es un valor de 32 bit

	/*La frecuencia de reloj contador CK_CNT es igual a fck_psc/(psc[15:0]+1)
	 * por tanto define la velocidad a la que incrementa el counter*/
	ptrBTimerHandler->ptrTIMx->PSC = 16*(ptrBTimerHandler->TIMx_Config.TIMx_periodcnt)-1; //(min:0, max:65536)

	//----------------------3) Configurando de la direccion del counter(up/down)---------------------------
	//Registro:TIMx_CR1		Es un registro de configuracion del TIMx
	//Registro:TIMx_ARR		Es un valor de 32 bit
	//Registro:TIMx_CNT/	Es un valor de 32 bit

	//verificamos si el timer se configuro como up o dowm
	if(ptrBTimerHandler->TIMx_Config.TIMx_mode==BTIMER_MODE_UP)
	{
		//-------a)Definimos la direccion para el conteo-------------
		ptrBTimerHandler->ptrTIMx->CR1 &= ~(0b1<<4);
		//-------b)Configuracion del Auto-Reload---------------------
		ptrBTimerHandler->ptrTIMx->ARR = ptrBTimerHandler->TIMx_Config.TIMX_period;
		//-------c)Reinicio del registro counter----------------------
		ptrBTimerHandler->ptrTIMx->CNT = 0;
	}
	else
	{
		//-------a)Definimos la direccion para el conteo-------------
		ptrBTimerHandler->ptrTIMx->CR1 &= ~(0b1<<4); //limpiamos
		ptrBTimerHandler->ptrTIMx->CR1 |= (0b1<<4);
		//-------b)Configuracion del Auto-Reload---------------------
		ptrBTimerHandler->ptrTIMx->ARR = 0;
		//-------c)Reinicio del registro counter----------------------
		ptrBTimerHandler->ptrTIMx->CNT = ptrBTimerHandler->TIMx_Config.TIMX_period;
	}

	//----------------------------4) Activamos el timer por medio de CEN---------------------------------------
	//Registro:TIMx_CR1		Es un registro de configuracion del TIMx

	ptrBTimerHandler->ptrTIMx->CR1 |= TIM_CR1_CEN;

	//----------------------5) Activamos la interrupcion generada por el timer---------------------------------
	//Registro:TIMx_DIER		Es un registro de configuracion del TIMx

	ptrBTimerHandler->ptrTIMx->DIER |=TIM_DIER_UIE;

	//------------------6) Activamos el canal de sistema NVIC para indicar la interrupcion---------------------

	if(ptrBTimerHandler->ptrTIMx==TIM2)
	{
		//Activamos el NVIC para la interrupcion del TIM2
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(ptrBTimerHandler->ptrTIMx==TIM3)
	{
		//Activamos el NVIC para la interrupcion del TIM3
		NVIC_EnableIRQ(TIM3_IRQn);
	}
	else if(ptrBTimerHandler->ptrTIMx==TIM4)
	{
		//Activamos el NVIC para la interrupcion del TIM4
		NVIC_EnableIRQ(TIM4_IRQn);
	}
	else if(ptrBTimerHandler->ptrTIMx==TIM5)
	{
		//Activamos el NVIC para la interrupcion del TIM5
		NVIC_EnableIRQ(TIM5_IRQn);
	}

	//-----------------------------7)Activamos las interrupcciones globales--------------------------------
	__enable_irq();
}

//Funcion para ser sobreescrita en el archivo main
__attribute__((weak)) void BasicTimer_Callback(void)
{
	__NOP();
}

/*Cuando genera la interrupcion de algun timer(en este caso de TIM2), esta funcion es la que
 * apunta el NVIC en el vector de interrupciones*/
void TIM2_IRQHandler(void)
{
	//Registro:TIMx_SR    Es un registro de almacenamiento del TIMx
	//limpiamos la bandera que indica que la interrupcion se a generado
	ptrTimerUsed->SR &= ~TIM_SR_UIF;

	//Ejecute la funcion correspondiente a la interupccion
	BasicTimer_Callback();

}





