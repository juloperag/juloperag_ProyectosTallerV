
 /*
 *  Created on: 18/03/2023
 *      Author: julian
 */

#include <BasicTimer.h>
#include <PLLDriver.h>

//Variable que guarda la referencia del periferico a utilizar
TIM_TypeDef *ptrTimer2Used;
TIM_TypeDef *ptrTimer3Used;
TIM_TypeDef *ptrTimer4Used;
TIM_TypeDef *ptrTimer5Used;

//Funcion para cargar la configuracion del Timer
void BasicTimer_Config(BasicTimer_Handler_t *ptrBTimerHandler)
{

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

		//Guardamos una referencia al periferico que estamos utilizando
		ptrTimer2Used = ptrBTimerHandler->ptrTIMx;

	}
	//Verificamos para TIM3
	else if(ptrBTimerHandler->ptrTIMx==TIM3)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

		//Guardamos una referencia al periferico que estamos utilizando
		ptrTimer3Used = ptrBTimerHandler->ptrTIMx;
	}
	//Verificamos para TIM4
	else if(ptrBTimerHandler->ptrTIMx==TIM4)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

		//Guardamos una referencia al periferico que estamos utilizando
		ptrTimer4Used = ptrBTimerHandler->ptrTIMx;
	}
	//Verificamos para TIM5
	else if(ptrBTimerHandler->ptrTIMx==TIM5)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

		//Guardamos una referencia al periferico que estamos utilizando
		ptrTimer5Used = ptrBTimerHandler->ptrTIMx;
	}
	//------------------------------2) Configurando el pre-escaler-----------------------------------------
	//Registro:TIMx_PSC		//Es un valor de 32 bit

	uint8_t clock = getClockAPB1();          //Guardamos la velocidad de reloj entregada al bus APB1

	/*La frecuencia de reloj contador CK_CNT es igual a fck_psc/(psc[15:0]+1)
	 * por tanto define la velocidad a la que incrementa el counter*/
	ptrBTimerHandler->ptrTIMx->PSC = (clock*2)*(ptrBTimerHandler->TIMx_Config.TIMx_periodcnt)-1; //(min:0, max:65536)

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
		ptrBTimerHandler->ptrTIMx->ARR = ptrBTimerHandler->TIMx_Config.TIMX_period+1;
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

//Definimos las funciones para cuando se genera una interrupcion del TIM2-3
__attribute__((weak)) void BasicTimer2_Callback(void)
{
	__NOP();
}

__attribute__((weak)) void BasicTimer3_Callback(void)
{
	__NOP();
}

__attribute__((weak)) void BasicTimer4_Callback(void)
{
	__NOP();
}

__attribute__((weak)) void BasicTimer5_Callback(void)
{
	__NOP();
}


/* Cuando se produce una interrupcion en el NVIC debido a uno de los TIMER apuntara a una de
 * estas funciones en el vector de interrupciones respectivamente
 */
void TIM2_IRQHandler(void)
{
	//Registro:TIMx_SR    Es un registro de almacenamiento del TIMx
	//limpiamos la bandera que indica que la interrupcion se a generado
	ptrTimer2Used->SR &= ~TIM_SR_UIF;

	//Ejecute la funcion correspondiente a la interupccion
	BasicTimer2_Callback();

}

void TIM3_IRQHandler(void)
{
	//Registro:TIMx_SR    Es un registro de almacenamiento del TIMx
	//limpiamos la bandera que indica que la interrupcion se a generado
	ptrTimer3Used->SR &= ~TIM_SR_UIF;

	//Ejecute la funcion correspondiente a la interupccion
	BasicTimer3_Callback();

}

void TIM4_IRQHandler(void)
{
	//Registro:TIMx_SR    Es un registro de almacenamiento del TIMx
	//limpiamos la bandera que indica que la interrupcion se a generado
	ptrTimer4Used->SR &= ~TIM_SR_UIF;

	//Ejecute la funcion correspondiente a la interupccion
	BasicTimer4_Callback();

}

void TIM5_IRQHandler(void)
{
	//Registro:TIMx_SR    Es un registro de almacenamiento del TIMx
	//limpiamos la bandera que indica que la interrupcion se a generado
	ptrTimer5Used->SR &= ~TIM_SR_UIF;

	//Ejecute la funcion correspondiente a la interupccion
	BasicTimer5_Callback();

}




