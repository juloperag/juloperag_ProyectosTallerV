
#include <stdint.h>
char hola=0;
char saludos = 0;

int main(void)
{
	if (hola==0)
	{
		for(char e;e<=2;e++)
		{
			saludos++;
		if(saludos==2)
		{
			saludos = 0;
		}
		}
	}
}
