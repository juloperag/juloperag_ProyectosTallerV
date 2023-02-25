
char hola=10;
char f=0;
char saltos = 0;

#include <stdint.h>

int main(void)
{
	for(char e=0;e<8;e++)
	{
		saltos++;

		if(saltos>=7)
		{
			saltos=0;
		}
	}

}
