#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void	free_num(int num, ...)
{
	va_list	args;

	va_start(args, num);
	while (num--)
	{
		free(va_arg(args, void *));
	}
	va_end(args);
}

int main()
{
	char *cp = malloc(sizeof(char) * 5);
	int *ip = malloc(sizeof(int) * 5);
	char *np = 0;
	free_num(3, cp, ip, np);
	
}
