/*
 * PwmDriver.c
 *
 *  Created on: 3/05/2023
 *      Author: julian
 */

#include <PwmDriver.h>
#include <PLLDriver.h>

void pwm_Config(PWM_Handler_t *prtPwmHandler)
{
	//---------------------------1) Activamos la señal de reloj al periferico----------------------------------
	//Registro: APB1ENR

	//Verificamos para TIM2
	if(prtPwmHandler->ptrTIMx==TIM2)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		* del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	}
	//Verificamos para TIM3
	else if(prtPwmHandler->ptrTIMx==TIM3)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	}
	//Verificamos para TIM4
	else if(prtPwmHandler->ptrTIMx==TIM4)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	}
	//Verificamos para TIM5
	else if(prtPwmHandler->ptrTIMx==TIM5)
	{
		/*Activamos el periferico escribiendo un 1 deacuerdo a la posicion
		 * del periferico en el registro*/
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	}
	else
	{
		__NOP();
	}

	//---------------------------2) Cargamos la frecuencia deseada----------------------------------
	//Registro: PSC
	//Registro: ARR

	setFrequency(prtPwmHandler);

	//---------------------------3) Cargamos el valor del dutty------------------------------------
	//Registro: CCRx

	setDuttyCycle(prtPwmHandler);

	//---------------------------4) Configuracion de CNT------------------------------------------
	//Registro: CR1
	//Registro: CNT

	//-------a)Definimos la direccion para el conteo-------------
	//Mode up
	prtPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_DIR;
	//-------b)Reinicio del registro counter----------------------
	prtPwmHandler->ptrTIMx->CNT = 0;

	//---------------------------5) Configuracion del CCMRx------------------------------------------
	//Registro: CCMRx

	switch(prtPwmHandler->config.channel)
	{
	//Configuracion del CCMR1_Channel 1
	case PWM_CHANNEL_1:
	{
		//Selecionamos el canal como salida
		prtPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
		//configuramos el canal como PWM
		prtPwmHandler->ptrTIMx->CCMR1 &= ~(0b111<<TIM_CCMR1_OC1M_Pos);
		prtPwmHandler->ptrTIMx->CCMR1 |= (0b110<<TIM_CCMR1_OC1M_Pos);
		//Activamos la funcionalidad de pre-load
		prtPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC1PE;
		prtPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC1PE;
		//Configuracion adicional
		prtPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC1FE;
		prtPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC1FE;
		prtPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC1CE;

		break;

	}
	//Configuracion del CCMR1_Channel 2
	case PWM_CHANNEL_2:
	{
		//Selecionamos el canal como salida
		prtPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_CC1S;
		//configuramos el canal como PWM
		prtPwmHandler->ptrTIMx->CCMR1 &= ~(0b111<<TIM_CCMR1_OC2M_Pos);
		prtPwmHandler->ptrTIMx->CCMR1 |= (0b110<<TIM_CCMR1_OC2M_Pos);
		//Activamos la funcionalidad de pre-load
		prtPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC2PE;
		prtPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC2PE;
		//Configuracion adicional
		prtPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC2FE;
		prtPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC2FE;
		prtPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_OC2CE;

		break;
	}
	//Configuracion del CCMR2_Channel 3
	case PWM_CHANNEL_3:
	{
		//Selecionamos el canal como salida
		prtPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_CC3S;
		//configuramos el canal como PWM
		prtPwmHandler->ptrTIMx->CCMR2 &= ~(0b111<<TIM_CCMR2_OC3M_Pos);
		prtPwmHandler->ptrTIMx->CCMR2 |= (0b110<<TIM_CCMR2_OC3M_Pos);
		//Activamos la funcionalidad de pre-load
		prtPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_OC3PE;
		prtPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR2_OC3PE;
		//Configuracion adicional
		prtPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_OC3FE;
		prtPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR2_OC3FE;
		prtPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_OC3CE;

		break;
	}
	//Configuracion del CCMR2_Channel 4
	case PWM_CHANNEL_4:
	{
		//Selecionamos el canal como salida
		prtPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_CC4S;
		//configuramos el canal como PWM
		prtPwmHandler->ptrTIMx->CCMR2 &= ~(0b111<<TIM_CCMR2_OC4M_Pos);
		prtPwmHandler->ptrTIMx->CCMR2 |= (0b110<<TIM_CCMR2_OC4M_Pos);
		//Activamos la funcionalidad de pre-load
		prtPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_OC4PE;
		prtPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR2_OC4PE;
		//Configuracion adicional
		prtPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_OC4FE;
		prtPwmHandler->ptrTIMx->CCMR2 |= TIM_CCMR2_OC4FE;
		prtPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_OC4CE;

		break;
	}
	default:
	{
		break;
	}
	}

	//---------------------------6) Activamos la salida seleccionada------------------------------------------
	//Registro: CCER

	enableOutput(prtPwmHandler);

}

//Funcion para activar el timer y todo el modulo PWM
void startPwmSignal(PWM_Handler_t *prtPwmHandler)
{
	prtPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_CEN;
	prtPwmHandler->ptrTIMx->CR1 |= TIM_CR1_CEN;
}

//Funcion para desactivar el timer y detener todo el modulo PWM
void stopPwmSignal(PWM_Handler_t *prtPwmHandler)
{
	prtPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_CEN;
}

//Activamos el canal especificado
void enableOutput(PWM_Handler_t *prtPwmHandler)
{
	switch(prtPwmHandler->config.channel)
	{
	//Activamos el Canal 1
	case PWM_CHANNEL_1:
	{
		prtPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC1E;
		prtPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC1E;

		break;
	}
	//Activamos el Canal 2
	case PWM_CHANNEL_2:
	{
		prtPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC2E;
		prtPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC2E;

		break;
	}
	//Activamos el Canal 3
	case PWM_CHANNEL_3:
	{
		prtPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC3E;
		prtPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC3E;

		break;
	}
	//Activamos el Canal 4
	case PWM_CHANNEL_4:
	{
		prtPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC4E;
		prtPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC4E;

		break;
	}
	default:
	{
		break;
	}
	}
}

//se estable la frecuencia del PWM
void setFrequency(PWM_Handler_t *prtPwmHandler)
{
	uint8_t clock = getClockAPB1();          //Guardamos la velocidad de reloj entregada al bus APB1

	/*La frecuencia de reloj contador CK_CNT es igual a fck_psc/(psc[15:0]+1)
	* por tanto define la velocidad a la que incrementa el counter y con ello la del TIMER*/
	prtPwmHandler->ptrTIMx->PSC = clock*(prtPwmHandler->config.periodcnt)-1; //(min:0, max:65536)
	/*Cargamos el valor del ARR el cual es e limite de incrementos del TIMER
	 * antes de hacer una update y reload
	 */
	prtPwmHandler->ptrTIMx->ARR = prtPwmHandler->config.periodo+1;
}

//Actualizamos la frecuencia del PWM
void updateFrequency(PWM_Handler_t *prtPwmHandler, uint16_t newPer)
{
	//Establecemos el nuevo valor del periodo en la configuracion del PWM
	prtPwmHandler->config.periodo = newPer;
	//Cargamos la nueva configuracion
	setFrequency(prtPwmHandler);
}

//Configuracion del DuttyCicle
void setDuttyCycle(PWM_Handler_t *prtPwmHandler)
{
	//Selecionamos el canal para configurar el dutty
	switch(prtPwmHandler->config.channel)
	{

	case PWM_CHANNEL_1:
	{
		prtPwmHandler->ptrTIMx->CCR1 = prtPwmHandler->config.duttyCicle;
		break;
	}
	case PWM_CHANNEL_2:
	{
		prtPwmHandler->ptrTIMx->CCR2 = prtPwmHandler->config.duttyCicle;
		break;
	}
	case PWM_CHANNEL_3:
	{
		prtPwmHandler->ptrTIMx->CCR3 = prtPwmHandler->config.duttyCicle;
		break;
	}
	case PWM_CHANNEL_4:
	{
		prtPwmHandler->ptrTIMx->CCR4 = prtPwmHandler->config.duttyCicle;
		break;
	}
	default:
	{
		break;
	}
	}

}


//Actualizacion del Dutty sin porcentaje
void updateDuttyCycle(PWM_Handler_t *prtPwmHandler, uint16_t newDutty)
{
	//Establecemos el nuevo valor del duttycicle en la configuracion del PWM
	prtPwmHandler->config.duttyCicle = newDutty;
	//Cargamos la nueva configuracion
	setDuttyCycle(prtPwmHandler);
}

//Actualizacion del Dutty con porcentaje
void updateDuttyCyclePercentage(PWM_Handler_t *prtPwmHandler, uint8_t newDuttyPercentage)
{
	//Establecemos el nuevo valor del duttycicle en la configuracion del PWM
	prtPwmHandler->config.duttyCicle = ((prtPwmHandler->config.periodo)/100)*(newDuttyPercentage);
	//Cargamos la nueva configuracion
	setDuttyCycle(prtPwmHandler);
}


