/**
 ******************************************************************************
 * @file           : main.c
 * @author         : juloperag
 * @brief          : Ejemplo TIMER
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f4xx.h>

#include <GPIOxDriver.h>

GPIO_Handler_t handlerOnBoardLED = {0};

int main(void)
{
	handlerOnBoardLED.pGIOx = GIOA;
	handlerOnBoardLED.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerOnBoardLED.GPIO_PinConfig.GPIO_PinModer = GPIO_MODE_OUT;
	handlerOnBoardLED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	handlerOnBoardLED.GPIO_PinConfig.GPIO_PinOTPype = GPIO_OTYPE_PUSHPULL;

	GPIO_Config(&handlerOnBoardLED);

	GPIO_writePin(&handlerOnBoardLED, SET);


	RCC->APB1ENR |= (0b1<<0); //o con RCC_APB1ENR_TIM2EN;

	//Recuerda GPIOC->MODER &= ~(0b11<<10);
	TIM2->CR1 &=TIM_CR1_DIR;  //Configuramos DIR
	TIM2->PSC = 16000; //1 milisegundo
	TIM2->CNT = 0;
	TIM2->ARR =250;		//autoreloar
	TIM2->CR1 = TIM_CR1_CEN;

	while(1)
	{
		if(TIM2->SR & TIME_SR_U1F)
		{
			GPIO.Tooglepin(&handlerOnBoardLED);
			TIM2->SR &= ~TIM_SR_U1F;
		}
	}

	return 0;
}
