

#include <stdint.h>
#include <hal.h>
unsigned int ejemplo = 0;

int main(void)
{
	ejemplo = RCC_AHB1ENR;
}
