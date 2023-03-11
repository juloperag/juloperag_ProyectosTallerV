/*
 * Ej2.c
 *
 *  Created on: 9/03/2023
 *      Author: julian
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

uint32_t maximoValor = 0;
uint32_t maximo(uint8_t bits,uint8_t signo);

int main(void)
{

	maximoValor = maximo(8,1);

	return 0;
}

//0 indica sin signo, el 1 indica signo
uint32_t maximo(uint8_t nbits,uint8_t signo)
{
	if (nbits == 8 || nbits == 16 || nbits == 32)
	{
		uint32_t max = 0;

		if(signo==0)
		{
			max = pow(2,nbits)-1;
			return max;
		}
		else if (signo==1)
		{
			max = (pow(2,nbits))/2-1;
			return max;
		}
		else
		{
			return 0;
		}
	}
}
